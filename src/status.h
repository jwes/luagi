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
#ifndef IS_LUAGI_STATUS
#define IS_LUAGI_STATUS

#include <lua.h>
#include <lauxlib.h>

//function on repos
int luagi_status_foreach( lua_State *L );
int luagi_status_foreach_ext( lua_State *L );
int luagi_status_file ( lua_State *L );
int luagi_status_list_new ( lua_State *L );
int luagi_status_should_ignore( lua_State *L );

// list operations
int luagi_status_list_entrycount( lua_State *L );
int luagi_status_by_index( lua_State *L );

int luagi_status_list_free( lua_State *L );

static const struct luaL_Reg luagi_status_funcs [] = {
   { "__index", luagi_status_by_index },
   { "__len", luagi_status_list_entrycount },
   { "__gc", luagi_status_list_free },
   { NULL, NULL }
};

#define LUAGI_STATUS_FUNCS "is.westh.luagi.status"
#define LUAGI_STATUS_FOREACH "is.westh.luagi.status.foreach"

#define checkstatuslist(L) \
      (git_status_list **) luaL_checkudata( L, 1, LUAGI_STATUS_FUNCS )
#endif //IS_LUAGI_STATUS
