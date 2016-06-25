/*
 * MIT License
 * 
 * Copyright (c) 2016 Johannes Westhuis, Alexander Schulz
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "blob.h"

#include <git2/blob.h>
#include <string.h>

#include "ltk.h"
#include "luagi.h"
#include "oid.h"

int luagi_blob_lookup( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid oid;
   int len = 0;
   if( luagi_check_oid_prefix( &oid, &len, L, 2 ) )
   {
      ltk_error_abort( L );
      return 0;
   }
   git_blob **out = lua_newuserdata( L, sizeof( git_blob * ) ); 

   int ret = 0;
   if( len == GIT_OID_HEXSZ )
   {
      ret = git_blob_lookup( out, *repo, &oid );
   }
   else
   {
      ret = git_blob_lookup_prefix( out, *repo, &oid, len );
   }

   if( ret )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_BLOB_FUNCS );
   return 1;
}

int luagi_blob_create_fromworkdir( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *path = luaL_checkstring( L, 2 );

   git_oid oid;
   if( git_blob_create_fromworkdir( &oid, *repo, path ) )
   {
      return ltk_push_git_error( L );
   }

   return luagi_push_oid( L, &oid );
}

int luagi_blob_create_fromdisk( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *path = luaL_checkstring( L, 2 );

   git_oid oid;
   if( git_blob_create_fromdisk( &oid, *repo, path ) )
   {
      return ltk_push_git_error( L );
   }

   return luagi_push_oid( L, &oid );
}

int luagi_blob_create_frombuffer( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   int len = luaL_len( L, 2 );
   const char *buffer = luaL_checkstring( L, 2 );

   git_oid oid;
   if( git_blob_create_frombuffer( &oid, *repo, buffer, len ) )
   {
      return ltk_push_git_error( L );
   }

   return luagi_push_oid( L, &oid );
}

// blob
int luagi_blob_free( lua_State *L )
{
   git_blob **blob = check_blob_at( L, 1 );
   git_blob_free( *blob );
   return 0;
}

int luagi_blob_id( lua_State *L )
{
   git_blob **blob = check_blob_at( L, 1 );
   const git_oid *oid = git_blob_id( *blob );
   return luagi_push_oid( L, oid );
}

int luagi_blob_rawsize( lua_State *L )
{
   git_blob **blob = check_blob_at( L, 1 );
   lua_pushinteger( L, git_blob_rawsize( *blob ) );
   return 1;
}

int luagi_blob_rawcontent( lua_State *L )
{
   git_blob **blob = check_blob_at( L, 1 );
   const void *raw = git_blob_rawcontent( *blob );
   git_off_t size = git_blob_rawsize( *blob );
   lua_pushlstring( L, raw, size );
   return 1;
}

int luagi_blob_filtered_content( lua_State *L )
{
   git_blob **blob = check_blob_at( L, 1 );
   const char *as_path = luaL_checkstring( L, 2 );
   int check_for_binary = lua_toboolean( L, 3 );

   git_buf out = GIT_BUF_INIT_CONST(NULL, 0);
   if( git_blob_filtered_content( &out, *blob, as_path, check_for_binary ) )
   {
      return ltk_push_git_error( L );
   }

   lua_pushlstring( L, out.ptr, out.size );
   return 1;
}

int luagi_blob_is_binary( lua_State *L )
{
   git_blob **blob = check_blob_at( L, 1 );
   int b = git_blob_is_binary( *blob );
   lua_pushboolean( L, b );
   return 1;
}

