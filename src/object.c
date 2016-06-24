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
#include "object.h"

#include <git2/object.h>
#include <git2/oid.h>

#include "ltk.h"
#include "luagi.h"
#include "oid.h"

int luagi_object_lookup( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   git_oid id;
   if(luagi_check_oid( &id, L, 2 ) )
   {
      return ltk_push_git_error( L );
   }

   const char *typestr = luaL_checkstring( L, 3 );
   git_otype type = git_object_string2type( typestr );
  
   git_object **out = lua_newuserdata( L, sizeof( git_object *) );
   if( git_object_lookup( out, *repo, &id, type ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_OBJECT_FUNCS );

   return 1;
}

int luagi_object_lookup_bypath( lua_State *L )
{
   git_object **obj = checkobject_at( L, 1 );
   const char *path = luaL_checkstring( L, 2 );
   const char *typestr = luaL_checkstring( L, 3 );

   git_otype type = git_object_string2type( typestr );
  
   git_object **out = lua_newuserdata( L, sizeof( git_object *) );
   if( git_object_lookup_bypath( out, *obj, path, type ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_OBJECT_FUNCS );

   return 1;
}

//use
int luagi_object_id( lua_State *L )
{
   git_object **obj = checkobject_at( L, 1 );

   const git_oid *oid = git_object_id( *obj );
   if( oid == NULL )
   {
      return ltk_push_git_error( L );
   }

   return luagi_push_oid( L, oid );
}

int luagi_object_type( lua_State *L )
{
   git_object **obj = checkobject_at( L, 1 );

   git_otype type = git_object_type( *obj );
   lua_pushstring( L, git_object_type2string( type ) );

   return 1;
}

int luagi_object_free( lua_State *L )
{

   git_object **obj = checkobject_at( L, 1 );
   git_object_free( *obj );
   return 0;   
}
