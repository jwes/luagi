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
#ifndef IS_LUAGI_REFLOG
#define IS_LUAGI_REFLOG

#include <lua.h>
#include <lauxlib.h>
#include <git2/reflog.h>

// repo
int luagi_reflog_read( lua_State *L );
int luagi_reflog_rename( lua_State *L );
int luagi_reflog_delete( lua_State *L );

//reflog
int luagi_reflog_write( lua_State *L );
int luagi_reflog_append( lua_State *L );
int luagi_reflog_entrycount( lua_State *L );
int luagi_reflog_entry_byindex( lua_State *L );
int luagi_reflog_drop( lua_State *L );
int luagi_reflog_entry_totable( lua_State *L, const git_reflog_entry *entry );
int luagi_reflog_free( lua_State *L );

#define LUAGI_REFLOG_FUNCS "is.luagi.reflog"
#define checkreflog_at( L, N )\
   luaL_checkudata( L, N, LUAGI_REFLOG_FUNCS )

#define checkreflog( L ) \
   checkreflog_at( L, 1 )

static const struct luaL_Reg luagi_reflog_funcs [] = {
   { "__gc", luagi_reflog_free },
   { "write", luagi_reflog_write },
   { "append", luagi_reflog_append },
   { "count", luagi_reflog_entrycount },
   { "entry_by_index", luagi_reflog_entry_byindex }, // TODO __index ?
   { "drop", luagi_reflog_drop },
   { NULL, NULL }
};

#endif
