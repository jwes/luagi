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
#ifndef IS_LUAGI_H
#define IS_LUAGI_H
#include <git2/types.h>
#include <git2/oid.h>
#include <git2/strarray.h>
#include <git2/errors.h>

#include "defines.h"

#define REPO_NAME "is.luagi.repo"
#define LUAGI_TREE_FUNCS "is.luagi.tree"
#define LUAGI_TREE_ENTRY_FUNCS "is.luagi.tree.entry"
#define LUAGI_TREE_BUILDER_FUNCS "is.luagi.tree.builder"
#define LUAGI_TREE_WALK_STATICS "is.luagi.tree.walk"
#define LUAGI_REMOTE_FUNCS "is.luagi.remote"

#define checkrepo(L, n) \
      (git_repository**) luaL_checkudata( L, n, REPO_NAME )
#define checktree_at(L, n) \
      (git_tree**) luaL_checkudata( L, n, LUAGI_TREE_FUNCS )
#define checktree(L) \
        checktree_at( L, 1 )
#define checkremote(L) \
      (git_remote**) luaL_checkudata( L, 1, LUAGI_REMOTE_FUNCS )

int signature_to_table( lua_State *L, const git_signature* sig);
int table_to_signature( lua_State *L, git_signature **sig, int position );

int luagi_push_transfer_stats( lua_State *L, const git_transfer_progress *stats );
typedef struct luagi_foreach_payload
{
   lua_State *L;
   int callback_pos;
} luagi_foreach_t;

#define add_flag( out,  L, idx, name, flag ) \
   lua_getfield( L, idx, name ); \
   if( lua_toboolean( L, -1 ) ) \
   { \
      out |= flag; \
   } \
   lua_pop( L, 1 );
#endif
