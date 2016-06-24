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
#include "revwalk.h"

#include <git2/revwalk.h>

#include "ltk.h"
#include "luagi.h"
#include "oid.h"

int luagi_revwalk_new( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   git_revwalk **out = lua_newuserdata( L, sizeof( git_revwalk * ) );

   if( git_revwalk_new( out, *repo ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_REVWALK_FUNCS );
   return 1;
}

int luagi_revwalk_reset( lua_State *L )
{
   git_revwalk **revwalk = checkrevwalk( L );

   git_revwalk_reset( *revwalk );
   return 0;
}

int luagi_revwalk_push( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );

   git_oid oid;
   luagi_check_oid( &oid, L, 2 );

   if( git_revwalk_push( *rev, &oid ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_revwalk_push_glob( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );

   const char *glob = luaL_checkstring( L, 2 );
   if( git_revwalk_push_glob( *rev, glob ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_revwalk_push_head( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );

   if( git_revwalk_push_head( *rev ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_revwalk_hide( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );
   git_oid oid;
   luagi_check_oid( &oid, L, 2 );

   if( git_revwalk_hide( *rev, &oid ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_revwalk_hide_glob( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );
   const char *glob = luaL_checkstring( L, 2 );

   if( git_revwalk_hide_glob( *rev, glob ) )
   {
        ltk_error_abort( L );
   }
   return 0;
}

int luagi_revwalk_hide_head( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );

   if( git_revwalk_hide_head( *rev ) )

   {
      ltk_error_abort( L );
   }
   return 0;
}


int luagi_revwalk_push_ref( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );
   const char *ref = luaL_checkstring( L, 2 );

   if( git_revwalk_push_ref( *rev, ref ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}
   
int luagi_revwalk_hide_ref( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );
   const char *ref = luaL_checkstring( L, 2 );

   if( git_revwalk_hide_ref( *rev, ref ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_revwalk_next( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );
   git_oid out;

   if( git_revwalk_next( &out, *rev ) )
   {
      return ltk_push_git_error( L );
   }

   return luagi_push_oid( L, &out );
}
static unsigned int checkmode( lua_State *L, int idx )
{
   luaL_checktype( L, idx, LUA_TTABLE );
   unsigned int mode = GIT_SORT_NONE;
   add_flag( mode, L, idx, TIME, GIT_SORT_TIME );
   add_flag( mode, L, idx, TOPOLOGICAL, GIT_SORT_TOPOLOGICAL );
   add_flag( mode, L, idx, REVERSE, GIT_SORT_REVERSE );
   return mode;
}
int luagi_revwalk_sorting( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );
   unsigned int sorting_mode = checkmode( L, 2 );

   git_revwalk_sorting( *rev, sorting_mode );

   return 0;
}

int luagi_revwalk_push_range( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );
   const char *range = luaL_checkstring( L, 2 );

   if( git_revwalk_push_range( *rev, range ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_revwalk_simplify_first_parent( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );

   git_revwalk_simplify_first_parent( *rev );

   return 0;
}

int luagi_revwalk_free( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );

   git_revwalk_free( *rev );

   return 0;
}

int luagi_revwalk_repository( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );

   git_repository **repo = lua_newuserdata( L, sizeof( git_repository * ) );

   *repo = git_revwalk_repository( *rev );
   if( *repo == NULL )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, REPO_NAME );
   return 1;
}
   
int luagi_revwalk_add_hide_callback( lua_State *L )
{
   //TODO callbacks
   luaL_error( L, "not yet implemented" );
   return 0;
}

