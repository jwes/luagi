#include <lua.h>
#include <lauxlib.h>
#include <git2/repository.h>	
#include <git2/signature.h>	

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
	{ "id", lgit_commit_id },
	{ "summary", lgit_commit_summary },
	{ "message", lgit_commit_message },
	{ "encoding", lgit_commit_encoding },
	{ "committer", lgit_commit_committer },
	{ "author", lgit_commit_author },
	{ "tree", lgit_commit_tree },
	{ "parent_count", lgit_commit_parentcount },
	{ "parent", lgit_commit_parent },
	{ "parent_id", lgit_commit_parent_id },
	{ "ancestor", lgit_commit_nth_gen_ancestor },
	{ "amend", lgit_commit_amend },
	{ "__gc", lgit_commit_gc },
	{ NULL, NULL }
};

static const struct luaL_Reg lgit_tree_entry_funcs [] = {
};

static const struct luaL_Reg lgit_tree_builder_funcs [] = {
};

static const struct luaL_Reg lgit_tree_funcs [] = {
	{ "id", lgit_tree_id },
	{ "entry_count", lgit_tree_entrycount },
	{ "__gc", lgit_tree_gc },
	{ NULL, NULL }
};

static const struct luaL_Reg repofuncs [] = {
	{ "branch", lgit_create_branch },
	{ "branches", lgit_branches }, 
	{ "lookup_branch", lgit_branch_lookup }, 
	{ "lookup_commit", lgit_commit_lookup },
	{ "lookup_tree", lgit_tree_lookup }, 
	{ "treebuilder", lgit_tree_builder_create },
	{ "commit", lgit_commit_create }, 
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

int signature_to_table( lua_State *L, const git_signature* sig )
{
	lua_newtable( L );
	lua_pushstring( L, sig->name );
	lua_setfield( L, -2, SIG_NAME );
	lua_pushstring( L, sig->email );
	lua_setfield( L, -2, SIG_EMAIL );
	git_time time = sig->when;

	lua_pushnumber( L, time.time );
	lua_setfield( L, -2, SIG_TIME );

	lua_pushnumber( L, time.offset );
	lua_setfield( L, -2, SIG_TIME_OFF );
	return 0;
}
int table_to_signature( lua_State *L, git_signature* sig, int tablepos )
{
	lua_getfield( L, tablepos, SIG_NAME );
	const char* name = luaL_checkstring( L, -1 );
	lua_getfield( L, tablepos, SIG_EMAIL );
	const char* email = luaL_checkstring( L, -1 );
	lua_getfield( L, tablepos, SIG_TIME );
	git_time_t time = (git_time_t ) lua_tointeger( L, -1 );
	lua_getfield( L, tablepos, SIG_TIME_OFF );
	int offset = lua_tointeger( L, -1 );

	if( time == 0 )
	{
		return git_signature_now( &sig, name, email );
	}
	else 
	{
		return git_signature_new( &sig, name, email, time, offset );
	}
}
