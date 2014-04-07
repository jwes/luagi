#include <git2/common.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "common.h"

int lgit_version( lua_State *L )
{
	int major;
	int minor;
	int rev;

	git_libgit2_version( &major, &minor, &rev );

	lua_pushnumber( L, major );
	lua_pushnumber( L, minor );
	lua_pushnumber( L, rev);

	return 3;
}

int lgit_features( lua_State *L )
{
	int bitfield = git_libgit2_features();

	/* create new table */
	lua_newtable(L);
	lua_pushboolean(L, (bitfield & GIT_FEATURE_THREADS) > 0 );
	lua_setfield(L, -2, "threads" );
	lua_pushboolean(L, (bitfield & GIT_FEATURE_HTTPS) > 0 );
	lua_setfield(L, -2, "https" );
	lua_pushboolean(L, (bitfield & GIT_FEATURE_SSH) > 0 );
	lua_setfield(L, -2, "ssh" );
	return 1;
}
