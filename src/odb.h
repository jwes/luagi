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
#ifndef IS_LUAGI_ODB
#define IS_LUAGI_ODB

#include <lua.h>
#include <lauxlib.h>

extern int luagi_indexer_new( lua_State *L );
//general
int luagi_odb_new( lua_State *L );
int luagi_odb_open( lua_State *L );

//odb functions
int luagi_odb_add_disk_alternate( lua_State *L );
int luagi_odb_free( lua_State *L );
int luagi_odb_read( lua_State *L );
int luagi_odb_read_header( lua_State *L );
int luagi_odb_exists( lua_State *L );
int luagi_odb_refresh( lua_State *L );
int luagi_odb_foreach( lua_State *L );
int luagi_odb_write( lua_State *L );

int luagi_odb_open_wstream( lua_State *L );
int luagi_odb_open_rstream( lua_State *L );

int luagi_odb_write_pack( lua_State *L );
int luagi_odb_hash( lua_State *L );
int luagi_odb_hashfile( lua_State *L );

int luagi_odb_add_backend( lua_State *L );
int luagi_odb_add_alternate( lua_State *L );
int luagi_odb_num_backends( lua_State *L );
int luagi_odb_get_backend( lua_State *L );

#define LUAGI_ODB_FUNCS "is.luagi.odb"
#define checkodb_at( L, N ) \
   (git_odb **) luaL_checkudata( L, N, LUAGI_ODB_FUNCS )

static const struct luaL_Reg luagi_odb_funcs [] = {
   { "add_disk_alternate", luagi_odb_add_disk_alternate },
   { "__gc", luagi_odb_free },
   { "read", luagi_odb_read },
   { "read_header", luagi_odb_read_header },
   { "exists", luagi_odb_exists },
   { "refresh", luagi_odb_refresh },
   { "foreach", luagi_odb_foreach },
   { "write", luagi_odb_write },

   { "open_wstream", luagi_odb_open_wstream },
   { "open_rstream", luagi_odb_open_rstream },

   { "write_pack", luagi_odb_write_pack },

   { "add_backend", luagi_odb_add_backend },
   { "add_alternate", luagi_odb_add_alternate },
   { "num_backends", luagi_odb_num_backends },
   { "get_backend", luagi_odb_get_backend },

   { "indexer", luagi_indexer_new },
   { NULL, NULL }
};

int luagi_odb_stream_write( lua_State *L );
int luagi_odb_stream_finalize_write( lua_State *L );
int luagi_odb_stream_read( lua_State *L );
int luagi_odb_stream_free( lua_State *L );

#define LUAGI_ODB_STREAM_FUNCS "is.luagi.odb.stream"
#define checkodbstream_at( L, N ) \
   (git_odb_stream **) luaL_checkudata( L, N, LUAGI_ODB_STREAM_FUNCS )

static const struct luaL_Reg luagi_odb_stream_funcs [] = {
   { "__gc", luagi_odb_stream_free },
   { "read", luagi_odb_stream_read },
   { "write", luagi_odb_stream_write },
   { "finalize_write", luagi_odb_stream_finalize_write },
   { NULL, NULL }
};

int luagi_odb_object_free( lua_State *L );
int luagi_odb_object_id( lua_State *L );
int luagi_odb_object_data( lua_State *L );
int luagi_odb_object_size( lua_State *L );
int luagi_odb_object_type( lua_State *L );

#define LUAGI_ODB_OBJECT_FUNCS "is.luagi.odb.object"
#define checkodbobject_at( L, N ) \
   (git_odb_object **) luaL_checkudata( L, N, LUAGI_ODB_OBJECT_FUNCS )

static const struct luaL_Reg luagi_odb_object_funcs [] = {
   { "__gc", luagi_odb_object_free },
   { "id", luagi_odb_object_id },
   { "data", luagi_odb_object_data },
   { "size", luagi_odb_object_size },
   { "type", luagi_odb_object_type },
   { NULL, NULL }
};

#endif
