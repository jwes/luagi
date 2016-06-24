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
#include "ltk.h"

#include <string.h>

/*******************************************************************************
 lua util functions
*******************************************************************************/

extern inline void ltk_setmetatable( lua_State *L, const char *name );

extern inline int ltk_push_error_msg( lua_State *L, const char *msg );

void ltk_dump_stack( lua_State *L )
{
   int top = lua_gettop( L ); /* depth of the stack */
   for ( int i = 1; i <= top; i++ ) /* repeat for each level */
   {
      int t = lua_type( L, i );
      switch ( t )
      {
         case LUA_TSTRING: /* strings */
            printf( "'%s'", lua_tostring( L, i ) );
            break;
         case LUA_TBOOLEAN: /* booleans */
            printf( lua_toboolean( L, i ) ? "true" : "false" );
            break;
         case LUA_TNUMBER: /* numbers */
            printf( "%g", lua_tonumber( L, i ) );
            break;
         default: /* other values */
            printf( "%s", lua_typename( L, t ) );
            break;
      }
      printf( " " ); /* put a separator */
   }
   printf( "\n" ); /* end the listing */
}

/*******************************************************************************
 lua+libgit2 util functions
*******************************************************************************/

extern inline void ltk_error_abort( lua_State *L );

extern inline int ltk_push_git_error( lua_State *L );

git_strarray ltk_check_strarray( lua_State* L, int table_idx )
{
   git_strarray array;
   array.count = luaL_len( L, table_idx );
   
   array.strings = calloc( array.count, sizeof( char * ) );
   for( size_t i = 1; i <= array.count; i++ )
   {
      lua_pushinteger( L, i );
      lua_gettable( L, table_idx );
      const char* srcStr = luaL_checkstring( L, -1 );
      size_t len = strlen( srcStr ) + 1;
      char* dstStr = malloc( len );
      strncpy( dstStr, srcStr, len );
      array.strings[ i - 1 ] = dstStr;
   }

   return array;
}

void ltk_push_strarray( lua_State *L, git_strarray array )
{
   lua_newtable( L );
   for( size_t i = 1; i <= array.count; i++ )
   {
      lua_pushinteger( L, i );
      lua_pushstring( L, array.strings[ i - 1 ]);
      lua_settable( L, -3 );
   }
}
