#ifndef IS_LUAGI_PACK
#define IS_LUAGI_PACK
#include <lua.h>
#include <lauxlib.h>

int luagi_packbuilder_new( lua_State *L );

int luagi_packbuilder_set_threads( lua_State *L );
int luagi_packbuilder_insert( lua_State *L );
int luagi_packbuilder_insert_tree( lua_State *L );
int luagi_packbuilder_insert_commit( lua_State *L );
int luagi_packbuilder_write_buf( lua_State *L );
int luagi_packbuilder_write( lua_State *L );
int luagi_packbuilder_hash( lua_State *L );
int luagi_packbuilder_foreach( lua_State *L );
int luagi_packbuilder_object_count( lua_State *L );
int luagi_packbuilder_written( lua_State *L );
int luagi_packbuilder_set_callbacks( lua_State *L );
int luagi_packbuilder_free( lua_State *L );

#define LUAGI_PACK_FUNCS "is.luagi.pack"
#define checkpack( L ) \
   luaL_checkudata( L, 1, LUAGI_PACK_FUNCS );

static const struct luaL_Reg luagi_pack_funcs [] = {
   { "set_threads", luagi_packbuilder_set_threads },
   { "insert", luagi_packbuilder_insert },
   { "insert_tree", luagi_packbuilder_insert_tree },
   { "insert_commit", luagi_packbuilder_insert_commit },
   { "write",  luagi_packbuilder_write_buf },
   { "write_file", luagi_packbuilder_write },
   { "hash", luagi_packbuilder_hash },
   { "foreach", luagi_packbuilder_foreach },
   { "count", luagi_packbuilder_object_count },
   { "written", luagi_packbuilder_written },
   { "set_callbacks", luagi_packbuilder_set_callbacks },
   { "__gc", luagi_packbuilder_free },
   { NULL, NULL }
};

#endif

