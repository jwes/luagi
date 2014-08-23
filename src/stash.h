#ifndef IS_LUAGI_STASH
#define IS_LUAGI_STASH
#include <lua.h>
#include <lauxlib.h>

int luagi_stash_save( lua_State *L );
int luagi_stash_foreach( lua_State *L );
int luagi_stash_drop( lua_State *L );

#endif
