#ifndef IS_LUAGI_REVERT
#define IS_LUAGI_REVERT
#include <lua.h>
int luagi_revert( lua_State *L );
int luagi_revert_commit( lua_State *L );
#endif
