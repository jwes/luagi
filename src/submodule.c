#include "submodule.h"

int luagi_submodule_lookup( lua_State *L );
int luagi_submodule_foreach( lua_State *L );
int luagi_submodule_open( lua_State *L );
int luagi_submodule_add_setup( lua_State *L );
int luagi_submodule_resolve_url( lua_State *L );
int luagi_submodule_reload_all( lua_State *L );

int luagi_submodule_free( lua_State *L );
int luagi_submodule_add_finalize( lua_State *L );
int luagi_submodule_owner( lua_State *L );
int luagi_submodule_name( lua_State *L );
int luagi_submodule_path( lua_State *L );
int luagi_submodule_url( lua_State *L );
int luagi_submodule_branch( lua_State *L );
int luagi_submodule_set_url( lua_State *L );
int luagi_submodule_index_id( lua_State *L );
int luagi_submodule_head_id( lua_State *L );
int luagi_submodule_wd_id( lua_State *L );
int luagi_submodule_ignore( lua_State *L );
int luagi_submodule_set_ignore( lua_State *L );
int luagi_submodule_update( lua_State *L );
int luagi_submodule_set_update( lua_State *L );
int luagi_submodule_fetch_recurse_submodules( lua_State *L );
int luagi_submodule_set_fetch_recurse_submodules( lua_State *L );
int luagi_submodule_init( lua_State *L );
int luagi_submodule_sync( lua_State *L );
int luagi_submodule_reload( lua_State *L );
int luagi_submodule_status( lua_State *L );
int luagi_submodule_location( lua_State *L );
int luagi_submodule_add_to_index( lua_State *L );
int luagi_submodule_save( lua_State *L );
