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
#ifndef IS_LUAGI_PATCH
#define IS_LUAGI_PATCH
#include <lua.h>
#include <lauxlib.h>

int luagi_patch_from_diff( lua_State *L );
int luagi_patch_from_blobs( lua_State *L );
int luagi_patch_from_blob_and_buffer( lua_State *L );
int luagi_patch_from_buffers( lua_State *L );

int luagi_patch_free( lua_State *L );
int luagi_patch_get_delta( lua_State *L );
int luagi_patch_num_hunks( lua_State *L );
int luagi_patch_line_stats( lua_State *L );
int luagi_patch_get_hunk( lua_State *L );
int luagi_patch_num_lines_in_hunk( lua_State *L );
int luagi_patch_get_line_in_hunk( lua_State *L );
int luagi_patch_size( lua_State *L );
int luagi_patch_print( lua_State *L );
int luagi_patch_to_buf( lua_State *L );

#define LUAGI_PATCH_FUNCS "is.luagi.patch"

#define checkpatch( L ) \
   luaL_checkudata( L, 1, LUAGI_PATCH_FUNCS )

static const struct luaL_Reg luagi_patch_funcs [] = {
   { "__gc", luagi_patch_free },
   { "get_delta", luagi_patch_get_delta },
   { "num_hunks", luagi_patch_num_hunks },
   { "line_stats", luagi_patch_line_stats },
   { "get_hunk",  luagi_patch_get_hunk },
   { "num_lines_in_hunk", luagi_patch_num_lines_in_hunk },
   { "get_line_in_hunk", luagi_patch_get_line_in_hunk },
   { "size", luagi_patch_size },
   { "print", luagi_patch_print },
   { "to_buf",  luagi_patch_to_buf },
   { NULL, NULL }
};

#endif

