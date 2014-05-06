#include <lauxlib.h>
#include <git2/clone.h>
#include "../lgit.h"
#include "clone.h"

static int parse_options( git_clone_options* options, lua_State *L, const int startIndex )
{
	git_clone_init_options( options, GIT_CLONE_OPTIONS_VERSION ); 
	luaL_error( L, "not yet, implemented %d", startIndex );
	return 0;
}

int lgit_clone( lua_State *L )
{
	const char *url = luaL_checkstring( L, 1 );
	const char *local_path = luaL_checkstring( L, 2 );
	git_clone_options options;

	parse_options( &options, L, 3 );

	git_repository **repo = (git_repository **) lua_newuserdata( L, sizeof(git_repository *) );

	if( git_clone( repo, url, local_path, &options) )
	{
		lua_pushnil( L );
		lua_pushfstring( L, "unable to clone repository from %s to %s", url, local_path );
	}
	luaL_getmetatable( L, REPO_NAME );
	lua_setmetatable( L, -2 );
	return 0;
}
int lgit_clone_into( lua_State *L )
{
	luaL_error(L, "not yet implemented");
	return 0;
}
