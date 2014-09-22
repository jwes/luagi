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
