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
#ifndef IS_LUAGI_BLAME
#define IS_LUAGI_BLAME
#include <lua.h>
#include <lauxlib.h>
//repository
int luagi_blame_file( lua_State *L );

int luagi_blame_buffer( lua_State *L );
int luagi_blame_get_hunk_count( lua_State *L );
int luagi_blame_get_hunk_byindex( lua_State *L );
int luagi_blame_get_hunk_byline( lua_State *L );

int luagi_blame_free( lua_State *L );

#define LUAGI_BLAME_FUNCS "is.luagi.blame"

#define checkblame_at( L, N ) \
   (git_blame **) luaL_checkudata( L, N, LUAGI_BLAME_FUNCS )

static const struct luaL_Reg luagi_blame_funcs [] = {
   { "__gc", luagi_blame_free },
   { "buffer", luagi_blame_buffer },
   { "count", luagi_blame_get_hunk_count },
   { "byindex", luagi_blame_get_hunk_byindex },
   { "byline", luagi_blame_get_hunk_byline },
   { NULL, NULL }
};

#endif
