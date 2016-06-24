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
#include "clone.h"

#include <git2/clone.h>
#include <git2/errors.h>
#include <lauxlib.h>

#include "checkout.h"
#include "ltk.h"
#include "luagi.h"

static void parse_options( git_clone_options* options, lua_State *L, const int tableIndex )
{
   if( ! lua_istable( L, tableIndex ) )
   {
      git_clone_init_options( options, GIT_CLONE_OPTIONS_VERSION );
      return;
   }

   lua_getfield( L, tableIndex, LUAGI_CLONE_OPTS_VERSION );
   int version = luaL_optinteger( L, -1, GIT_CLONE_OPTIONS_VERSION );

   git_clone_init_options( options, version );

   lua_getfield( L, tableIndex, LUAGI_CLONE_OPTS_CHECKOUT_OPTS );
   if( lua_istable( L, -1 ) )
   {
      git_checkout_options opts;
      luagi_parse_checkout_options( &opts, L, -1 );
      options->checkout_opts = opts;
   }
   else
   {
      lua_pop( L, -1 );
   }

   lua_getfield( L, tableIndex, LUAGI_CLONE_OPTS_BARE );
   options->bare = luaL_optinteger( L, -1, 0 );
   lua_getfield( L, tableIndex, LUAGI_CLONE_OPTS_CHECKOUT_BRANCH );
   options->checkout_branch = luaL_optstring(L, -1, NULL );
}

int luagi_clone( lua_State *L )
{
   const char *url = luaL_checkstring( L, 1 );
   const char *local_path = luaL_checkstring( L, 2 );
   git_clone_options options;

   parse_options( &options, L, 3 );

   git_repository **repo = (git_repository **) lua_newuserdata( L, sizeof(git_repository *) );
   
   if( git_clone( repo, url, local_path, &options) ) 
   {
      const git_error* err = giterr_last();
      lua_pushnil( L );
      lua_pushfstring( L, "unable to clone repository from %s to %s --> %s", url, local_path, err->message );
      return 2;
   }
   ltk_setmetatable( L, REPO_NAME );
   return 1;
}

int luagi_clone_into( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
   return 0;
}
