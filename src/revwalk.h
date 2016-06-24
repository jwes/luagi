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
#ifndef IS_LUAGI_REVWALK
#define IS_LUAGI_REVWALK

#include <lua.h>
#include <lauxlib.h>

int luagi_revwalk_new( lua_State *L );

int luagi_revwalk_reset( lua_State *L );
int luagi_revwalk_push( lua_State *L );
int luagi_revwalk_push_glob( lua_State *L );
int luagi_revwalk_push_head( lua_State *L );
int luagi_revwalk_hide( lua_State *L );
int luagi_revwalk_hide_glob( lua_State *L );
int luagi_revwalk_hide_head( lua_State *L );
int luagi_revwalk_push_ref( lua_State *L );
int luagi_revwalk_hide_ref( lua_State *L );
int luagi_revwalk_next( lua_State *L );
int luagi_revwalk_sorting( lua_State *L );
int luagi_revwalk_push_range( lua_State *L );
int luagi_revwalk_simplify_first_parent( lua_State *L );
int luagi_revwalk_free( lua_State *L );
int luagi_revwalk_repository( lua_State *L );
int luagi_revwalk_add_hide_callback( lua_State *L );

#define LUAGI_REVWALK_FUNCS "is.luagi.revwalk"
#define checkrevwalk( L )\
   luaL_checkudata( L, 1, LUAGI_REVWALK_FUNCS )

static const struct luaL_Reg luagi_revwalk_funcs [] = {
   { "reset", luagi_revwalk_reset },
   { "push", luagi_revwalk_push },
   { "push_glob", luagi_revwalk_push_glob },
   { "push_head", luagi_revwalk_push_head },
   { "hide", luagi_revwalk_hide },
   { "hide_glob", luagi_revwalk_hide_glob },
   { "hide_head", luagi_revwalk_hide_head },
   { "push_ref", luagi_revwalk_push_ref },
   { "hide_ref", luagi_revwalk_hide_ref },
   { "next",  luagi_revwalk_next },
   { "sorting", luagi_revwalk_sorting },
   { "push_range", luagi_revwalk_push_range },
   { "simplify_first_parent", luagi_revwalk_simplify_first_parent },
   { "__gc", luagi_revwalk_free },
   { "repository", luagi_revwalk_repository },
   { "add_hide_callback",  luagi_revwalk_add_hide_callback },
   { NULL, NULL }
};

#endif

