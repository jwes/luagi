#include <lauxlib.h>
#include <git2/tree.h>
#include <git2/oid.h>
#include "tree.h"
#include "../lgit.h"

#include <stdio.h>

/*#define checktree(L) \
		(git_tree**) luaL_checkudata( L, 1, LGIT_TREE_FUNCS )
*/
int lgit_tree_lookup( lua_State *L )
{
	git_repository** repo = checkrepo( L, 1 );
	const char* id_str = luaL_checkstring( L, 2 );

	git_oid oid;
	int ret = git_oid_fromstr( &oid, id_str);
	if( ret != 0 )
	{
		lua_pushnil( L );
		lua_pushstring(L, "the given string is no valid git oid");
		return 2;
	}

	git_tree** out = (git_tree**)lua_newuserdata( L, sizeof(git_tree*) );
	ret = git_tree_lookup( out, *repo, &oid );
	if( ret != 0 )
	{
		lua_pushnil( L );
		lua_pushfstring(L, "failed to lookup tree %d", ret );
		return 2;
	}

	luaL_getmetatable( L, LGIT_TREE_FUNCS );
	lua_setmetatable( L, -2 );
	return 1;
}

int lgit_tree_lookup_prefix( lua_State *L )
{
	lua_pushnil( L );
	lua_pushstring(L, "not yet implemented" );
	return 2;
}

int lgit_tree_id( lua_State *L )
{
	git_tree** tree = checktree( L );
	const git_oid* oid = git_tree_id( *tree );
	char out[ GIT_OID_HEXSZ+1];
	lua_pushstring( L, git_oid_tostr( out, sizeof(out), oid ));
	return 1;
}

int lgit_tree_owner( lua_State *L )
{
	git_tree** tree = checktree( L );
	
	git_repository** repo = (git_repository**) lua_newuserdata( L, sizeof( git_repository* )); 
	
	*repo = git_tree_owner( *tree );

	luaL_getmetatable( L, REPO_NAME );
	lua_setmetatable( L, -2 );
	return 1;
}
int lgit_tree_entrycount( lua_State *L )
{
	git_tree** tree = checktree( L );
	size_t count = git_tree_entrycount( *tree );
	lua_pushinteger( L, count );
	return 1;
}

int lgit_tree_gc( lua_State *L )
{
	git_tree** tree = checktree( L );
	git_tree_free( *tree );
	return 0;
}

/* tree entry genereation */

int lgit_tree_entry_byname( lua_State *L )
{
	lua_pushnil( L );
	return 1;
}
int lgit_tree_entry_byindex( lua_State *L )
{
	lua_pushnil( L );
	return 1;
}
int lgit_tree_entry_byid( lua_State *L )
{
	lua_pushnil( L );
	return 1;
}

/* user owned tree entries */
int lgit_tree_entry_bypath( lua_State *L )
{
	lua_pushnil( L );
	return 1;
}
int lgit_tree_entry_dup( lua_State *L )
{
	lua_pushnil( L );
	return 1;
}
int lgit_tree_entry_gc( lua_State *L )
{
	lua_pushnil( L );
	return 1;
}

/* entry stuff */
int lgit_tree_entry_name( lua_State *L )
{
	lua_pushnil( L );
	return 1;
}
int lgit_tree_entry_id( lua_State *L )
{
	lua_pushnil( L );
	return 1;
}
int lgit_tree_entry_type( lua_State *L )
{
	lua_pushnil( L );
	return 1;
}
int lgit_tree_entry_filemode( lua_State *L )
{
	lua_pushnil( L );
	return 1;
}
int lgit_tree_entry_filemode_raw( lua_State *L )
{
	lua_pushnil( L );
	return 1;
}

int lgit_tree_entry_cmp( lua_State *L )
{
	lua_pushnil( L );
	return 1;
}
// to_object


/* treebuilder stuff */
int lgit_tree_builder_create( lua_State *L )
{
	lua_pushnil( L );
	return 1;
}

