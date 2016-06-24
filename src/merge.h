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
#ifndef IS_LUAGI_MERGE
#define IS_LUAGI_MERGE
#include <lua.h>
#include <lauxlib.h>
#include <git2/merge.h>

int luagi_merge_analysis( lua_State *L );
int luagi_merge_base( lua_State *L );
int luagi_merge_base_many( lua_State *L );
int luagi_merge_base_octopus( lua_State *L );

int luagi_merge_file( lua_State *L );
int luagi_merge_file_from_index( lua_State *L );
int luagi_merge_file_result_free( lua_State *L );

int luagi_merge_trees( lua_State *L );
int luagi_merge_commits( lua_State *L );
int luagi_merge( lua_State *L );

int luagi_merge_init_options( lua_State *L, int index, git_merge_options *opts );
#endif
