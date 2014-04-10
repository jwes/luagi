#include <lua.h>
#include <lauxlib.h>
#include <git2/repository.h>	

#include "lgit.h"
#include "src/common.h"
#include "src/branch.h"


static int lgit_open( lua_State *L )
{
	git_repository** repo;

	const char* path = luaL_checkstring( L, 1 );
	repo = (git_repository**) lua_newuserdata(L, sizeof(git_repository*) );
	
	const int ret = git_repository_open( repo, path );

	if( ret < 0 )
	{
		lua_pushnil(L);
		lua_pushnumber(L, ret);
		return 2;
	}

	luaL_getmetatable( L, REPO_NAME );
	lua_setmetatable( L, -2 );
	return 1;
}

static int lgit_gc( lua_State *L )
{
	git_repository** repo = checkrepo(L, 1);
	git_repository_free( *repo );
	return 0;
}

static const struct luaL_Reg lgit_branch_funcs [] = {
	{ "delete", lgit_delete_branch },
	{ "move", lgit_move_branch },
	{ "get_upstream", lgit_branch_upstream_get },
	{ "set_upstream", lgit_branch_upstream_set },
	{ "name", lgit_branch_name },
	{ "is_head", lgit_branch_is_head }, /* done */
	{ "__gc", lgit_branch_gc },
	{ NULL, NULL }
};

static const struct luaL_Reg repofuncs [] = {
	{ "create_branch", lgit_create_branch }, /*todo*/
	{ "branches", lgit_branches }, 
	{ "branch_lookup", lgit_branch_lookup }, 
	{ "__gc", lgit_gc },
	{ NULL, NULL },
};

static const struct luaL_Reg mylib [] = {
	{ "version", lgit_version },
	{ "features", lgit_features },
	{ "open", lgit_open },
	{ NULL, NULL } /*sentinel*/
};

int luaopen_lgit(lua_State *L)
{
	/* metatable for the branch iterator */
	luaL_newmetatable( L, LGIT_BRANCH_STATICS );
	lua_pushcfunction( L, lgit_branch_iter_gc);
	lua_setfield( L, -2, "__gc" );

	luaL_newmetatable( L, LGIT_BRANCH_FUNCS );
	lua_pushvalue( L, -1 );
	lua_setfield( L, -2, "__index");
	luaL_setfuncs( L, lgit_branch_funcs, 0);

	luaL_newmetatable( L, REPO_NAME );
	lua_pushvalue( L, -1 );
	lua_setfield( L, -2, "__index");
	luaL_setfuncs( L, repofuncs, 0);

	luaL_newlib( L, mylib );
	return 1;
}

