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
#ifndef IS_LUAGI_BLOB
#define IS_LUAGI_BLOB
#include <lua.h>
#include <lauxlib.h>
//repo
int luagi_blob_lookup( lua_State *L );
int luagi_blob_create_fromworkdir( lua_State *L );
int luagi_blob_create_fromdisk( lua_State *L );
int luagi_blob_create_fromchunks( lua_State *L );
int luagi_blob_create_frombuffer( lua_State *L );

// blob
int luagi_blob_free( lua_State *L );
int luagi_blob_id( lua_State *L );
int luagi_blob_rawsize( lua_State *L );
int luagi_blob_rawcontent( lua_State *L );

int luagi_blob_filtered_content( lua_State *L );
int luagi_blob_is_binary( lua_State *L );

#define LUAGI_BLOB_FUNCS "is.luagi.blob"

#define check_blob_at(L, N) \
   (git_blob **) luaL_checkudata( L, N, LUAGI_BLOB_FUNCS );

extern int luagi_patch_from_blobs( lua_State *L );
extern int luagi_patch_from_blob_and_buffer( lua_State *L );

static const struct luaL_Reg luagi_blob_funcs [] = {
   { "__gc", luagi_blob_free },
   { "__tostring", luagi_blob_id },
   { "id", luagi_blob_id },
   { "filtered_content", luagi_blob_filtered_content },
   { "is_binary", luagi_blob_is_binary },
   { "raw_size", luagi_blob_rawsize },
   { "raw_content", luagi_blob_rawcontent },
   { "patch", luagi_patch_from_blobs },
   { "patch_with_buffer", luagi_patch_from_blob_and_buffer },
   { NULL, NULL }
};

#endif
