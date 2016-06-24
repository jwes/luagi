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
#include "cherrypick.h"

#include <git2/cherrypick.h>
#include <lauxlib.h>

#include "commit.h"
#include "index.h"
#include "ltk.h"
#include "luagi.h"
#include "merge.h"

static int luagi_cherrypick_init_options( lua_State *L __attribute__((unused)), int index __attribute__((unused)), git_cherrypick_options *opts )
{
   int ret = git_cherrypick_init_options( opts, GIT_CHERRYPICK_OPTIONS_VERSION );

   return ret;
}

int luagi_cherrypick_commit( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_commit **cherrypick_commit = checkcommit_at( L, 2 );
   git_commit **our_commit = checkcommit_at( L, 3 );
   luaL_checktype( L, 4, LUA_TBOOLEAN );
   int mainline = lua_toboolean( L, 4 );
   git_merge_options opts;
   luagi_merge_init_options( L, 5, &opts );   

   git_index **out = lua_newuserdata( L, sizeof( git_index * ) );

   if( git_cherrypick_commit( out, *repo, *cherrypick_commit, *our_commit, mainline, &opts ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_INDEX_FUNCS );
   return 1;
}

int luagi_cherrypick( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_commit **commit = checkcommit_at( L, 2 );

   git_cherrypick_options opts;
   luagi_cherrypick_init_options( L, 3, &opts );

   if( git_cherrypick( *repo, *commit, &opts ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}
