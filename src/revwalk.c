#include <git2/revwalk.h>
#include "revwalk.h"

int luagi_revwalk_new( lua_State *L );

int luagi_revwalk_reset( lua_State *L );
int luagi_revwalk_push( lua_State *L );
int luagi_revwalk_push_glob( lua_State *L );
int luagi_revwalk_push_head( lua_State *L );
int luagi_revwalk_hide( lua_State *L );
int luagi_revwalk_hide_glob( lua_State *L );
int luagi_revwalk_hide_head( lua_State *L );
int luagi_revwalk_push_ref( lua_State *L );
int luagi_revwalk_hide_ref( lua_State *L );
int luagi_revwalk_next( lua_State *L );
int luagi_revwalk_sorting( lua_State *L );
int luagi_revwalk_push_range( lua_State *L );
int luagi_revwalk_simplify_first_parent( lua_State *L );
int luagi_revwalk_free( lua_State *L );
int luagi_revwalk_repository( lua_State *L );
int luagi_revwalk_add_hide_callback( lua_State *L );
