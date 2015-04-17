#ifndef IS_LUAGI_RESET
#define IS_LUAGI_RESET
#include <lua.h>

int luagi_reset( lua_State *L );
int luagi_reset_default( lua_State *L );
int luagi_reset_index( lua_State *L );

#endif
