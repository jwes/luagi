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
#ifndef IS_LUAGI_BRANCH
#define IS_LUAGI_BRANCH

#include <lua.h>
#include <lauxlib.h>

#define LUAGI_BRANCH_FUNCS "is.westh.luagi.branch"
#define LUAGI_BRANCH_STATICS "is.westhu.luagi.branch.iter"

int luagi_reference_branch_target( lua_State *L );
int luagi_reference_branch_target_peel( lua_State *L );
int luagi_reference_branch_symbolic_target( lua_State *L );
int luagi_reference_branch_type( lua_State *L );
int luagi_reference_branch_resolve( lua_State *L );
int luagi_reference_branch_symbolic_set_target( lua_State *L );
int luagi_reference_branch_set_target( lua_State *L );
int luagi_reference_branch_rename( lua_State *L );
int luagi_reference_branch_foreach( lua_State *L );
int luagi_reference_branch_is_branch( lua_State *L );
int luagi_reference_branch_is_remote( lua_State *L );
int luagi_reference_branch_is_tag( lua_State *L );
int luagi_reference_branch_is_note( lua_State *L );
int luagi_reference_branch_peel( lua_State *L );
int luagi_reference_branch_shorthand( lua_State *L );
int luagi_reference_branch_free( lua_State *L );


int luagi_create_branch( lua_State *L );
int luagi_branch_lookup( lua_State *L );

int luagi_branch_name( lua_State *L );
int luagi_delete_branch( lua_State *L );
int luagi_move_branch( lua_State *L );

int luagi_branch_upstream_get( lua_State *L );
int luagi_branch_upstream_set( lua_State *L );

int luagi_branches( lua_State *L );
int luagi_branch_iter_gc( lua_State *L );

int luagi_branch_is_head( lua_State *L );

static const struct luaL_Reg luagi_branch_funcs [] = {
   { "delete", luagi_delete_branch },
   { "move", luagi_move_branch },
   { "get_upstream", luagi_branch_upstream_get },
   { "set_upstream", luagi_branch_upstream_set },
   { "name", luagi_branch_name },
   { "is_head", luagi_branch_is_head },
   { "target", luagi_reference_branch_target },
   { "target_peel", luagi_reference_branch_target_peel },
   { "symbolic_target", luagi_reference_branch_symbolic_target },
   { "type", luagi_reference_branch_type },
   { "resolve", luagi_reference_branch_resolve },
   { "set_symbolic_target", luagi_reference_branch_symbolic_set_target },
   { "set_target", luagi_reference_branch_set_target },
   { "rename", luagi_reference_branch_rename },
   { "is_branch", luagi_reference_branch_is_branch },
   { "is_remote", luagi_reference_branch_is_remote },
   { "is_tag",  luagi_reference_branch_is_tag },
   { "is_note", luagi_reference_branch_is_note },
   { "peel", luagi_reference_branch_peel },
   { "shorthand", luagi_reference_branch_shorthand },
   { "__gc", luagi_reference_branch_free },
   { NULL, NULL }
};
#endif
