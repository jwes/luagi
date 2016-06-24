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
#ifndef LUAGI_ANNOTATED_COMMIT
#define LUAGI_ANNOTATED_COMMIT

#include <git2/annotated_commit.h>

#include <lua.h>
#include <lauxlib.h>

int luagi_annotated_commit_from_ref( lua_State *L );
int luagi_annotated_commit_from_fetchhead( lua_State *L );
int luagi_annotated_commit_lookup( lua_State *L );

int luagi_annotated_commit_id( lua_State *L );
int luagi_annotated_commit_free( lua_State *L );

void fill_annotated_commit( lua_State *L, int index, const git_annotated_commit **commits, int len );

#define LUAGI_ANNOTATED_COMMIT_FUNCS "is.luagi.annotated.commit"

#define check_annotated_commit_at( L, N ) \
   (git_annotated_commit **) luaL_checkudata( L, N, LUAGI_ANNOTATED_COMMIT_FUNCS )

static const struct luaL_Reg luagi_annotated_commit_funcs [] = {
   { "__gc", luagi_annotated_commit_free },
   { "__tostring", luagi_annotated_commit_id },
   { NULL, NULL }
};
#endif
