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
#ifndef IS_LUAGI_CONFIG
#define IS_LUAGI_CONFIG
#include <lua.h>
#include <lauxlib.h>

//general
int luagi_config_find_global( lua_State *L );
int luagi_config_find_xdg( lua_State *L );
int luagi_config_find_system( lua_State *L );

int luagi_config_open_default( lua_State *L );
int luagi_config_new( lua_State *L );
int luagi_config_open_ondisk( lua_State *L );


// config
int luagi_config_add_file_ondisk( lua_State *L );
int luagi_config_open_level( lua_State *L );
int luagi_config_open_global( lua_State *L );
int luagi_config_snapshot( lua_State *L );

int luagi_config_get_entry( lua_State *L );
int luagi_config_get_int32( lua_State *L );
int luagi_config_get_int64( lua_State *L );
int luagi_config_get_bool( lua_State *L );
int luagi_config_get_string( lua_State *L );

int luagi_config_get_multivar_foreach( lua_State *L );

int luagi_config_iterator_new( lua_State *L );
int luagi_config_iterator_glob_new( lua_State *L );
int luagi_config_multivar_iterator( lua_State *L );
int luagi_config_iterator_free( lua_State *L );

int luagi_config_set_int32( lua_State *L );
int luagi_config_set_int64( lua_State *L );
int luagi_config_set_bool( lua_State *L );
int luagi_config_set_string( lua_State *L );
int luagi_config_set_multivar( lua_State *L );
int luagi_config_delete_entry( lua_State *L );
int luagi_config_delete_multivar( lua_State *L );

int luagi_config_foreach( lua_State *L );
int luagi_config_foreach_match( lua_State *L );

//TODO mapping
int luagi_config_get_mapped( lua_State *L );
int luagi_config_lookup_map_value( lua_State *L );

int luagi_config_parse_bool( lua_State *L );
int luagi_config_parse_int32( lua_State *L );
int luagi_config_parse_int64( lua_State *L );

int luagi_config_backend_foreach_match( lua_State *L );

int luagi_config_free( lua_State *L );

#define LUAGI_CONFIG_ITERATOR_FUNCS "is.luagi.config.iterator"
#define LUAGI_CONFIG_FUNCS "is.luagi.config"
#define checkconfig_at( L, N ) \
   luaL_checkudata( L, N, LUAGI_CONFIG_FUNCS )

#define checkconfig( L ) \
    checkconfig_at( L, 1 )

static const struct luaL_Reg luagi_config_funcs [] = {
	{ "add_file_ondisk", luagi_config_add_file_ondisk },
   { "open_level", luagi_config_open_level },
   { "open_global", luagi_config_open_global },
   { "snapshot", luagi_config_snapshot },

   { "get_entry", luagi_config_get_entry },
   { "get_int32", luagi_config_get_int32 },
   { "get_int64", luagi_config_get_int64 },
   { "get_bool", luagi_config_get_bool },
   { "get_string", luagi_config_get_string },

   { "get_multivar", luagi_config_get_multivar_foreach },

   { "iterator", luagi_config_iterator_new },
   { "iterator_glob", luagi_config_iterator_glob_new },
   { "multivar_iterator", luagi_config_multivar_iterator },

   { "set_int32", luagi_config_set_int32 },
   { "set_int64", luagi_config_set_int64 },
   { "set_bool", luagi_config_set_bool },
   { "set_string", luagi_config_set_string },
   { "set_multivar", luagi_config_set_multivar },
   { "delete_entry", luagi_config_delete_entry },
   { "delete_multivar", luagi_config_delete_multivar },

   { "foreach", luagi_config_foreach },
   { "foreach_match", luagi_config_foreach_match },

   { "get_mapped", luagi_config_get_mapped },

   { "backend_foreach_match",luagi_config_backend_foreach_match },
   { "__gc", luagi_config_free },
   { NULL, NULL }
};

#endif
