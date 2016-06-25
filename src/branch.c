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
#include "branch.h"

#include <git2/commit.h>
#include <git2/errors.h>
#include <git2/refs.h>
#include <git2/signature.h>
#include <git2/branch.h>
#include <git2/buffer.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>

#include "ltk.h"
#include "luagi.h"
#include "commit.h"
#include "reference.h"

#define checkbranch(L) \
      (git_reference**) luaL_checkudata( L, 1, LUAGI_BRANCH_FUNCS )

int luagi_create_branch( lua_State *L )
{
   git_repository** repo = checkrepo(L, 1);
   const char* branch_name = luaL_checkstring(L, 2);
   git_commit** target = ( git_commit** ) luaL_checkudata( L, 3, LUAGI_COMMIT_FUNCS);
   int force = lua_toboolean(L, 5);

   git_reference** out = (git_reference**) lua_newuserdata(L, sizeof(git_reference*) );

   if( git_branch_create( out, *repo, branch_name, *target, force ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_BRANCH_FUNCS );
   return 1;      
}

int luagi_delete_branch( lua_State *L )
{
   git_reference** out = checkbranch(L);
   int ret = git_branch_delete( *out );
   if( ret != 0 )
   {
      /* severe error */
      luaL_error( L, "couldn't delete branch %d", ret );
   }
   return 0;      
}

int luagi_move_branch( lua_State *L )
{
   git_reference** ref = checkbranch(L);
   const char* to_name = luaL_checkstring(L, 2);
   if(! lua_istable(L, 3) )
   {
      luaL_error( L, "signature is no table" );
      return 0;
   }
   int force = lua_toboolean(L, 5 );

   git_reference** out = (git_reference**) lua_newuserdata(L, sizeof(git_reference*) );
   if( git_branch_move( out, *ref, to_name, force ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_BRANCH_FUNCS );
   return 1;

}

static int branch_iter( lua_State *L);

int luagi_branches( lua_State *L )
{
   git_repository** repo = checkrepo(L, 1);
   const char* modus = luaL_checkstring(L, 2);
   if( modus == NULL )
   { 
      modus = "";
   }

   git_branch_t flags = GIT_BRANCH_LOCAL;
   if( *modus == 'a' )
   {
      flags = flags | GIT_BRANCH_REMOTE;
   }
   else if ( *modus == 'r' )
   {
      flags = GIT_BRANCH_REMOTE;
   }

   /* create iterator userdata and open iterator */
   git_branch_iterator** iter =(git_branch_iterator**) lua_newuserdata(L, sizeof(git_branch_iterator*));
   ltk_setmetatable( L, LUAGI_BRANCH_STATICS );

   int ret = git_branch_iterator_new( iter, *repo, flags);
   if( ret != 0 )
   { 
      ltk_error_abort( L );
      return 0;
   }

   lua_pushcclosure( L, branch_iter, 1 );
   return 1;      
}

static int branch_iter( lua_State *L )
{
   git_branch_iterator* iter = *(git_branch_iterator**) lua_touserdata( L, lua_upvalueindex( 1 ) );

   git_reference** out = (git_reference**) lua_newuserdata(L, sizeof(git_reference*) );
   git_branch_t type;
   int ret = git_branch_next( out, &type, iter );
   if( ret == 0 )
   {
      ltk_setmetatable( L, LUAGI_BRANCH_FUNCS );

      lua_pushboolean(L, type == GIT_BRANCH_REMOTE );

      return 2;
   } 
   else if( ret == GIT_ITEROVER )
   {
      return 0;
   }
   else 
   {
      luaL_error(L, "iteration failed, error %d", ret);
      return 0;
   }
}

int luagi_branch_iter_gc( lua_State *L )
{
   git_branch_iterator** iter = (git_branch_iterator**) lua_touserdata( L, lua_upvalueindex( 1 ) );
   if( iter != NULL )
   {
      git_branch_iterator_free( *iter );
   }
   return 0;
}

int luagi_branch_upstream_get( lua_State *L )
{
   git_reference** ref = checkbranch(L);
   git_reference** out = (git_reference**) lua_newuserdata(L, sizeof(git_reference*) );
   int ret = git_branch_upstream( out, *ref );
   if( ret != 0 )
   {
      return ltk_push_git_error( L );
   }
   
   ltk_setmetatable( L, LUAGI_BRANCH_FUNCS );
   return 1;
}

int luagi_branch_upstream_set( lua_State *L )
{
   git_reference** ref = checkbranch(L);
   const char *upstream = luaL_checkstring( L, 2 );
   if( git_branch_set_upstream( *ref, upstream ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_branch_lookup( lua_State *L )
{
   git_repository** repo = checkrepo( L, 1);
   const char* branch_name = luaL_checkstring( L, 2 );
   const char* btype = luaL_optstring( L, 3, "l" );
   git_branch_t type = GIT_BRANCH_LOCAL;
   if( *btype == 'r' )
   {
      type = GIT_BRANCH_REMOTE;
   }

   git_reference** out = (git_reference**) lua_newuserdata(L, sizeof(git_reference*) );
   int ret = git_branch_lookup( out, *repo, branch_name, type );
   if( ret != 0 )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_BRANCH_FUNCS );
   return 1;
}

int luagi_branch_is_head( lua_State *L )
{
   git_reference** ref = checkbranch(L);
   int ret = git_branch_is_head( *ref );
   lua_pushboolean( L, ret );
   return 1;
}

int luagi_branch_name( lua_State *L )
{
   git_reference** ref = checkbranch(L);
   const char* out;
   int ret = git_branch_name( &out, *ref );
   if( ret != 0 )
   {
      luaL_error( L, "name failed %d", ret );
      return 0;
   }
   lua_pushstring( L, out );
   return 1;
}

int luagi_reference_branch_target( lua_State *L )
{
   return luagi_reference_gen_target( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_target_peel( lua_State *L )
{
   return luagi_reference_gen_target_peel( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_symbolic_target( lua_State *L )
{
   return luagi_reference_gen_symbolic_target( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_type( lua_State *L )
{
   return luagi_reference_gen_type( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_name( lua_State *L )
{
   return luagi_reference_gen_name( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_resolve( lua_State *L )
{
   return luagi_reference_gen_resolve( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_symbolic_set_target( lua_State *L )
{
   return luagi_reference_gen_symbolic_set_target( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_set_target( lua_State *L )
{
   return luagi_reference_gen_set_target( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_rename( lua_State *L )
{
   return luagi_reference_gen_rename( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_delete( lua_State *L )
{
   return luagi_reference_gen_delete( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_is_branch( lua_State *L )
{
   return luagi_reference_gen_is_branch( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_is_remote( lua_State *L )
{
   return luagi_reference_gen_is_remote( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_is_tag( lua_State *L )
{
   return luagi_reference_gen_is_tag( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_is_note( lua_State *L )
{
   return luagi_reference_gen_is_note( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_peel( lua_State *L )
{
   return luagi_reference_gen_peel( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_shorthand( lua_State *L )
{
   return luagi_reference_gen_shorthand( L, LUAGI_BRANCH_FUNCS );
}

int luagi_reference_branch_free( lua_State *L )
{
   return luagi_reference_gen_free( L, LUAGI_BRANCH_FUNCS );
}

