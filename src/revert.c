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
#include <git2/revert.h>

#include <lauxlib.h>

#include "revert.h"
#include "luagi.h"
#include "commit.h"
#include "merge.h"
#include "index.h"
#include "ltk.h"


int luagi_revert_commit( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_commit **revert_commit = checkcommit_at( L, 2 );
   git_commit **commit = checkcommit_at( L, 3 );

   int mainline = 0;
   int index = 4;
   if( lua_type( L, index) == LUA_TBOOLEAN )
   {
      mainline = lua_toboolean( L, index );
      index++;
   }
   
   git_merge_options opts;
   luagi_merge_init_options( L, index, &opts );

   git_index **out = lua_newuserdata( L, sizeof(git_index *) );
   if( git_revert_commit( out, *repo, *revert_commit, *commit, mainline, &opts ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_INDEX_FUNCS );
   return 1;
}
static int luagi_revert_init_options( lua_State *L __attribute__((unused)),
                                       int index __attribute__((unused)),
                                       git_revert_options *opts )
{
   int ret = git_revert_init_options( opts, GIT_REVERT_OPTIONS_VERSION );

   return ret;
}

int luagi_revert( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_commit **commit = checkcommit_at( L, 2 );

   git_revert_options opts;
   luagi_revert_init_options( L, 3, &opts );

   if( git_revert( *repo, *commit, &opts ) )
   {
      ltk_error_abort( L );
   }

   return 0;
}
