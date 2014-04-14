#include <lauxlib.h>
#include <git2/commit.h>
#include "commit.h"
#include "../lgit.h"

#define checkcommit( L ) \
	(git_commit**) luaL_checkudata( L, 1, LGIT_COMMIT_FUNCS )
int lgit_commit_lookup( lua_State *L )
{
	git_repository** repo = checkrepo( L, 1 );
	const char* ref = luaL_checkstring( L, 2 );

	git_oid oid;
	int ret = git_oid_fromstr( &oid, ref);
	if( ret != 0 )
	{
		lua_pushnil( L );
		lua_pushstring(L, "the given string is no valid git oid");
	}

	git_commit** out_commit = (git_commit**)lua_newuserdata( L, sizeof( git_commit* ));
	ret = git_commit_lookup( out_commit, *repo, &oid );
	if( ret != 0 )
	{
		lua_pushnil( L );
		lua_pushfstring( L, "lookup failed - %d", ret );
	}
	luaL_getmetatable( L, LGIT_COMMIT_FUNCS );
	lua_setmetatable( L, -2 );
	return 1;
}

int lgit_commit_gc( lua_State *L )
{
	git_commit** commit = checkcommit( L );
	git_commit_free( *commit );
	return 0;
}
