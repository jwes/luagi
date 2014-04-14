#include <lua.h>
#include <lauxlib.h>
#include <git2/repository.h>	

#include "lgit.h"
#include "src/common.h"
#include "src/branch.h"
#include "src/tree.h"
#include "src/commit.h"


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
	{ "is_head", lgit_branch_is_head }, 
	{ "__gc", lgit_branch_gc },
	{ NULL, NULL }
};

static const struct luaL_Reg lgit_commit_funcs [] = {
	{ "__gc", lgit_commit_gc },
	{ NULL, NULL }
};

static const struct luaL_Reg lgit_tree_entry_funcs [] = {
};

static const struct luaL_Reg lgit_tree_builder_funcs [] = {
};

static const struct luaL_Reg lgit_tree_funcs [] = {
	{ "owner", lgit_tree_owner },
	{ "id", lgit_tree_id },
	{ "entry_count", lgit_tree_entrycount },
	{ "__gc", lgit_tree_gc },
	{ NULL, NULL }
};

static const struct luaL_Reg repofuncs [] = {
	{ "create_branch", lgit_create_branch },
	{ "branches", lgit_branches }, 
	{ "branch_lookup", lgit_branch_lookup }, 
	{ "commit_lookup", lgit_commit_lookup },
	{ "tree_lookup", lgit_tree_lookup }, 
	{ "treebuilder", lgit_tree_builder_create },
	{ "__gc", lgit_gc },
	{ NULL, NULL },
};

static const struct luaL_Reg mylib [] = {
	{ "version", lgit_version },
	{ "features", lgit_features },
	{ "open", lgit_open },
	{ NULL, NULL } /*sentinel*/
};

void setup_funcs( lua_State *L, const char *meta_name, const luaL_Reg *funcs )
{
	luaL_newmetatable( L, meta_name );
	lua_pushvalue( L, -1 );
	lua_setfield( L, -2, "__index");
	luaL_setfuncs( L, funcs, 0);
}

int luaopen_lgit(lua_State *L)
{
	/* metatable for the branch iterator */
	luaL_newmetatable( L, LGIT_BRANCH_STATICS );
	lua_pushcfunction( L, lgit_branch_iter_gc);
	lua_setfield( L, -2, "__gc" );

	setup_funcs(L, LGIT_TREE_FUNCS, lgit_tree_funcs);
	setup_funcs(L, LGIT_BRANCH_FUNCS, lgit_branch_funcs);
	setup_funcs(L, LGIT_COMMIT_FUNCS, lgit_commit_funcs);
	setup_funcs(L, REPO_NAME, repofuncs);

	luaL_newlib( L, mylib );
	return 1;
}

