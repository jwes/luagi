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

int luagi_config_refresh( lua_State *L );
int luagi_config_get_entry( lua_State *L );
int luagi_config_get_int32( lua_State *L );
int luagi_config_get_int64( lua_State *L );
int luagi_config_get_bool( lua_State *L );
int luagi_config_get_string( lua_State *L );

int luagi_config_get_mulitvar_foreach( lua_State *L );

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
int luagi_config_delete_mulitvar( lua_State *L );

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

   { "refresk", luagi_config_refresh },
   { "get_entry", luagi_config_get_entry },
   { "get_int32", luagi_config_get_int32 },
   { "get_int64", luagi_config_get_int64 },
   { "get_bool", luagi_config_get_bool },
   { "get_string", luagi_config_get_string },

   { "get_multivar", luagi_config_get_mulitvar_foreach },

   { "iterator_new", luagi_config_iterator_new },
   { "iterator_glob_new", luagi_config_iterator_glob_new },
   { "multivar_iterator", luagi_config_multivar_iterator },

   { "set_int32", luagi_config_set_int32 },
   { "set_int64", luagi_config_set_int64 },
   { "set_bool", luagi_config_set_bool },
   { "set_string", luagi_config_set_string },
   { "set_multivar", luagi_config_set_multivar },
   { "delete_entry", luagi_config_delete_entry },
   { "delete_multivar", luagi_config_delete_mulitvar },

   { "foreach", luagi_config_foreach },
   { "foreach_match", luagi_config_foreach_match },

   { "get_mapped", luagi_config_get_mapped },

   { "backend_foreach_match",luagi_config_backend_foreach_match },
   { "__gc", luagi_config_free },
   { NULL, NULL }
};

#endif
