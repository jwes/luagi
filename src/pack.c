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
#include "pack.h"

#include <git2/buffer.h>
#include <git2/pack.h>

#include "ltk.h"
#include "luagi.h"
#include "oid.h"

int luagi_packbuilder_new( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_packbuilder **out = lua_newuserdata( L, sizeof( git_packbuilder *) );

   if( git_packbuilder_new( out, *repo ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_PACK_FUNCS );
   return 1;
}

static int generic_insert( lua_State *L, int (*func)( git_packbuilder *pb, const git_oid *oid ) )
{
   git_packbuilder **pack = checkpack( L );
   git_oid oid;
   luagi_check_oid( &oid,  L, 2 );

   if( func( *pack, &oid ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_packbuilder_set_threads( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   unsigned int num = luaL_checkinteger( L, 2 );

   unsigned int used = git_packbuilder_set_threads( *pack, num );

   lua_pushinteger( L, used );
   return 1;
}

int luagi_packbuilder_insert( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   git_oid oid;
   luagi_check_oid( &oid, L, 2 );
   const char *name = luaL_checkstring( L, 3 );

   if( git_packbuilder_insert( *pack, &oid, name ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_packbuilder_insert_tree( lua_State *L )
{
   return generic_insert( L, git_packbuilder_insert_tree );
}
int luagi_packbuilder_insert_commit( lua_State *L )
{
   return generic_insert( L, git_packbuilder_insert_commit );
}

int luagi_packbuilder_write_buf( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   git_buf buf = GIT_BUF_INIT_CONST(NULL, 0);
   if( git_packbuilder_write_buf( &buf, *pack ) )
   {
      return ltk_push_git_error( L );
   }
   lua_pushlstring( L, buf.ptr, buf.size );
   return 1;
}

int luagi_packbuilder_write( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   const char *path = luaL_checkstring( L, 2 );
   unsigned int mode = 0; //TODO mode
   //TODO progress_cb
   
   if( git_packbuilder_write( *pack, path, mode, NULL, NULL ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_packbuilder_hash( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   const git_oid *oid = git_packbuilder_hash( *pack );

   return luagi_push_oid( L, oid );
}

static int packbuilder_cb( void *buf, size_t size, void *payload )
{
   luagi_foreach_t *p = payload;

   lua_pushvalue( p->L, p->callback_pos );
   lua_pushlstring( p->L, buf, size );

   if( lua_pcall( p->L, 1, 1, 0 ) )
   {
      luaL_error( p->L, "error calling callback function" );
      return -1;
   }
   int ret = luaL_optinteger( p->L, -1, 0 );
   lua_pop( p->L, 1 );
   return ret;
}

int luagi_packbuilder_foreach( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   luaL_checktype( L, 2, LUA_TFUNCTION );

   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );
   p->L = L;
   p->callback_pos = 2;

   int ret = git_packbuilder_foreach( *pack, packbuilder_cb, p );
   free( p );
   if( ret )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_packbuilder_object_count( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   lua_pushinteger( L, git_packbuilder_object_count( *pack ) );
   return 1;
}

int luagi_packbuilder_written( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   lua_pushinteger( L, git_packbuilder_written( *pack ) );
   return 1;
}

int luagi_packbuilder_set_callbacks( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
   //TODO callbacks 
   return 0;
}

int luagi_packbuilder_free( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   git_packbuilder_free( *pack );
   return 0;
}

