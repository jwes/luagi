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
#ifndef IS_LUAGI_OBJECT
#define IS_LUAGI_OBJECT
#include <lua.h>
#include <lauxlib.h>

//acquire
int luagi_object_lookup( lua_State *L );
int luagi_object_lookup_bypath( lua_State *L );

//use
int luagi_object_id( lua_State *L );
int luagi_object_type( lua_State *L );
int luagi_object_free( lua_State *L );

//types
int luagi_object_type2string( lua_State *L );
int luagi_object_string2type( lua_State *L );
int luagi_object_typeisloose( lua_State *L );
int luagi_object__size( lua_State *L );
int luagi_object_peel( lua_State *L );

#define LUAGI_OBJECT_FUNCS "is.luagi.object"

#define checkobject_at(L, N) \
      (git_object**) luaL_checkudata( L, N, LUAGI_OBJECT_FUNCS )

static const struct luaL_Reg luagi_object_funcs [] = {
   { "id",   luagi_object_id },
   { "type",   luagi_object_type },
   { "__gc",   luagi_object_free },
   { NULL, NULL }
};
#endif
