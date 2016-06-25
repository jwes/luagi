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
#ifndef IS_LUAGI_SUBMODULE
#define IS_LUAGI_SUBMODULE
#include <lua.h>
#include <lauxlib.h>
#include <git2/types.h>

int luagi_submodule_lookup( lua_State *L );
int luagi_submodule_foreach( lua_State *L );
int luagi_submodule_add_setup( lua_State *L );
int luagi_submodule_resolve_url( lua_State *L );

#define LUAGI_SUBMODULE_FUNCS "is.luagi.submodule"

#define checksubmodule_at( L, N ) \
   (git_submodule **) luaL_checkudata( L, N, LUAGI_SUBMODULE_FUNCS )

int luagi_submodule_open( lua_State *L );
int luagi_submodule_free( lua_State *L );
int luagi_submodule_add_finalize( lua_State *L );
int luagi_submodule_name( lua_State *L );
int luagi_submodule_path( lua_State *L );
int luagi_submodule_url( lua_State *L );
int luagi_submodule_branch( lua_State *L );
int luagi_submodule_set_url( lua_State *L );
int luagi_submodule_index_id( lua_State *L );
int luagi_submodule_head_id( lua_State *L );
int luagi_submodule_wd_id( lua_State *L );
int luagi_submodule_ignore( lua_State *L );
int luagi_submodule_set_ignore( lua_State *L );
int luagi_submodule_update( lua_State *L );
int luagi_submodule_update_strategy( lua_State *L );
int luagi_submodule_set_update( lua_State *L );
int luagi_submodule_fetch_recurse_submodules( lua_State *L );
int luagi_submodule_set_fetch_recurse_submodules( lua_State *L );
int luagi_submodule_init( lua_State *L );
int luagi_submodule_sync( lua_State *L );
int luagi_submodule_reload( lua_State *L );
int luagi_submodule_status( lua_State *L );
int luagi_submodule_location( lua_State *L );
int luagi_submodule_add_to_index( lua_State *L );
int luagi_submodule_repo_init( lua_State *L );

static const struct luaL_Reg luagi_submodule_funcs [] = {
   { "__gc",  luagi_submodule_free },
   { "add_finalize", luagi_submodule_add_finalize },
   { "name", luagi_submodule_name },
   { "__tostring", luagi_submodule_name },
   { "path", luagi_submodule_path },
   { "url", luagi_submodule_url },
   { "branch", luagi_submodule_branch },
   { "index_id", luagi_submodule_index_id },
   { "head_id", luagi_submodule_head_id },
   { "wd_id", luagi_submodule_wd_id },
   { "ignore", luagi_submodule_ignore },
   { "update", luagi_submodule_update },
   { "update_strategy", luagi_submodule_update_strategy },
   { "fetch_recurse", luagi_submodule_fetch_recurse_submodules },
   { "init", luagi_submodule_init },
   { "sync", luagi_submodule_sync },
   { "reload", luagi_submodule_reload },
   { "add_to_index", luagi_submodule_add_to_index },
   { "open_repository", luagi_submodule_open },
   { "location", luagi_submodule_location },
   { NULL, NULL }
};

git_submodule_ignore_t luagi_sub_check_ignore( lua_State *L, int index );
#endif
