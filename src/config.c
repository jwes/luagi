#include <git2/config.h>
#include "config.h"

//general
int luagi_config_find_global( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_find_xdg( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_find_system( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_config_open_default( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_new( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_open_ondisk( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }


// config
int luagi_config_add_file_ondisk( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_open_level( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_open_global( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_snapshot( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_config_refresh( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_get_entry( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_get_int32( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_get_int64( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_get_bool( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_get_string( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_config_get_mulitvar_foreach( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_config_iterator_new( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_iterator_glob_new( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_multivar_iterator( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_iterator_free( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_config_set_int32( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_set_int64( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_set_bool( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_set_string( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_set_multivar( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_delete_entry( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_delete_mulitvar( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_config_foreach( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_foreach_match( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

//TODO mapping
int luagi_config_get_mapped( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_lookup_map_value( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_config_parse_bool( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_parse_int32( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_parse_int64( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_config_backend_foreach_match( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_config_free( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }


