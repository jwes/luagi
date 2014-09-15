#ifndef IS_LUAGI_REFDB
#define IS_LUAGI_REFDB

#include <lua.h>
#include <lauxlib.h>

//repo
int luagi_refdb_new( lua_State *L );
int luagi_refdb_open( lua_State *L );

//refdb
int luagi_refdb_compress( lua_State *L );
int luagi_refdb_free( lua_State *L );

#define LUAGI_REFDB_FUNCS "is.luagi.refdb"

#define checkrefdb_at( L, N ) \
   luaL_checkudata( L, N, LUAGI_REFDB_FUNCS )

static const struct luaL_Reg luagi_refdb_funcs [] = {
   { "compress", luagi_refdb_compress },
   { "__gc", luagi_refdb_free },
   { NULL, NULL }
};

#endif
