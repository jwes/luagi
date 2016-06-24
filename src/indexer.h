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
#ifndef IS_LUAGI_INDEXER
#define IS_LUAGI_INDEXER
#include <lua.h>
#include <lauxlib.h>

int luagi_indexer_new( lua_State *L );
int luagi_indexer_append( lua_State *L );
int luagi_indexer_commit( lua_State *L );
int luagi_indexer_hash( lua_State *L );
int luagi_indexer_free( lua_State *L );

#define LUAGI_INDEXER_FUNCS "is.luagi.indexer"
#define checkindexer( L ) \
   luaL_checkudata( L, 1, LUAGI_INDEXER_FUNCS );

static const struct luaL_Reg luagi_indexer_funcs [] = {
   { "append", luagi_indexer_append },
   { "commit", luagi_indexer_commit },
   { "hash", luagi_indexer_hash },
   { "__gc", luagi_indexer_free },
   { NULL, NULL }
};

#endif
