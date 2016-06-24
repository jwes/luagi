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
#ifndef IS_LUAGI_COMMIT
#define IS_LUAGI_COMMIT

#include <lua.h>
#include <lauxlib.h>

int luagi_commit_lookup( lua_State *L );
int luagi_commit_gc( lua_State *L );
int luagi_commit_id( lua_State *L );
int luagi_commit_encoding( lua_State *L );
int luagi_commit_message( lua_State *L );
int luagi_commit_summary( lua_State *L );
int luagi_commit_author( lua_State *L );
int luagi_commit_committer( lua_State *L );
int luagi_commit_tree( lua_State *L );

int luagi_commit_parentcount( lua_State *L );
int luagi_commit_parent( lua_State *L );
int luagi_commit_parent_id( lua_State *L );
int luagi_commit_nth_gen_ancestor( lua_State *L );
int luagi_commit_create( lua_State *L );
int luagi_commit_amend( lua_State *L );

#define LUAGI_COMMIT_FUNCS "is.westh.luagi.commit"

#define checkcommit_at( L, N ) \
   (git_commit**) luaL_checkudata( L, N, LUAGI_COMMIT_FUNCS )
#define checkcommit( L ) \
   checkcommit_at( L, 1 )
static const struct luaL_Reg luagi_commit_funcs [] = {
   { "id", luagi_commit_id },
   { "summary", luagi_commit_summary },
   { "message", luagi_commit_message },
   { "encoding", luagi_commit_encoding },
   { "committer", luagi_commit_committer },
   { "author", luagi_commit_author },
   { "tree", luagi_commit_tree },
   { "parent_count", luagi_commit_parentcount },
   { "parent", luagi_commit_parent },
   { "parent_id", luagi_commit_parent_id },
   { "ancestor", luagi_commit_nth_gen_ancestor },
   { "amend", luagi_commit_amend },
   { "__gc", luagi_commit_gc },
   { NULL, NULL }
};
#endif
