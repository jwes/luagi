#include <lua.h>
#include <lauxlib.h>
#include "src/common.h"

static const struct luaL_Reg mylib [] = {
	{ "version", lgit_version },
	{ "features", lgit_features },
	{ NULL, NULL } /*sentinel*/
};

int luaopen_lgit(lua_State *L)
{
	luaL_newlib( L, mylib );
	return 1;
}
