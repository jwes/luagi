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
#include "graph.h"

#include <git2/graph.h>
#include <lauxlib.h>

#include "ltk.h"
#include "luagi.h"
#include "oid.h"

int luagi_graph_ahead_behind( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid local, upstream;
   luagi_check_oid( &local, L, 2 );
   luagi_check_oid( &upstream, L, 3 );

   size_t ahead, behind;

   if( git_graph_ahead_behind( &ahead, &behind, *repo, &local, &upstream ) )
   {
      return ltk_push_git_error( L );
   }

   lua_pushinteger( L, ahead );
   lua_pushinteger( L, behind );
   return 2;
}

int luagi_graph_descendant_of( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid commit, ancestor;
   luagi_check_oid( &commit, L, 2 );
   luagi_check_oid( &ancestor, L, 3 );

   lua_pushboolean( L, git_graph_descendant_of( *repo, &commit, &ancestor ) );
   return 1;
}

