#ifndef IS_LUAGI_IGNORE
#define IS_LUAGI_IGNORE
#include <lua.h>

int luagi_ignore_add_rules( lua_State *L );
int luagi_ignore_clear_internal_rules( lua_State *L );
int luagi_ignore_path_is_ignored( lua_State *L );

#endif
