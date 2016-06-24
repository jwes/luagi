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
#ifndef IS_LUAGI_LTK_H
#define IS_LUAGI_LTK_H

#include <git2/errors.h>
#include <git2/strarray.h>
#include <lua.h>
#include <lauxlib.h>

/*******************************************************************************
 lua util functions
*******************************************************************************/

inline void ltk_setmetatable( lua_State *L, const char *name )
{
   luaL_getmetatable( L, name );
   lua_setmetatable( L, -2 );
}

inline int ltk_push_error_msg( lua_State *L, const char *msg )
{
   lua_pushnil( L );
   lua_pushstring( L, msg );
   return 2;
}

void ltk_dump_stack( lua_State* L );

#ifdef STACK_DEBUG
#define ltk_debug_stack( L ) ltk_dump_stack( L )
#else
#define ltk_debug_stack( L )
#endif

/*******************************************************************************
 lua+libgit2 util functions
*******************************************************************************/

inline void ltk_error_abort( lua_State *L )
{
   const git_error *err = giterr_last();
   luaL_error( L, err ? err->message : "unknown" );
}

inline int ltk_push_git_error( lua_State *L )
{
   const git_error *err = giterr_last();
   return ltk_push_error_msg( L, err ? err->message : "unknown" );
}

void ltk_push_strarray( lua_State *L, git_strarray array );

git_strarray ltk_check_strarray( lua_State* L, int table_idx );

#endif
