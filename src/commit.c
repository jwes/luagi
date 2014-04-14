#include <lauxlib.h>
#include <git2/commit.h>
#include <git2/oid.h>
#include <git2/signature.h>
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

int lgit_commit_id( lua_State *L )
{
	git_commit** commit = checkcommit( L );
	const git_oid* oid = git_commit_id( *commit );
	char out [ GIT_OID_HEXSZ + 1 ];
	lua_pushstring( L, git_oid_tostr(out, sizeof( out ), oid ));
	return 1;
}

int lgit_commit_encoding( lua_State *L )
{
	git_commit** commit = checkcommit( L );
	lua_pushstring( L, git_commit_message_encoding( *commit ));
	return 1;
}

int lgit_commit_message( lua_State *L )
{
	git_commit** commit = checkcommit( L );
	lua_pushstring( L, git_commit_message( *commit ));
	return 1;
}

int lgit_commit_summary( lua_State *L )
{
	git_commit** commit = checkcommit( L );
	lua_pushstring( L, git_commit_summary( *commit ));
	return 1;
}

int lgit_commit_committer( lua_State *L )
{
	git_commit** commit = checkcommit( L );	
	const git_signature* sig = git_commit_committer( *commit );
	int ret = signature_to_table( L, sig );
	if( ret != 0 )
	{
		lua_pushnil( L );
		lua_pushfstring( L, "signature to table failed %d", ret );
		return 2;
	}
	/* signature table is on the stack */
	return 1;
}

int lgit_commit_author( lua_State *L )
{
	git_commit** commit = checkcommit( L );
	const git_signature* sig = git_commit_author( *commit );
	int ret = signature_to_table( L, sig );
	if( ret != 0 )
	{
		lua_pushnil( L );
		lua_pushfstring( L, "signature to table failed %d", ret );
		return 2;
	}
	/* signature table is on the stack */
	return 1;
}

int lgit_commit_tree( lua_State *L )
{
	git_commit** commit = checkcommit( L );
	git_tree** tree = (git_tree**) lua_newuserdata( L, sizeof( git_tree* ) );
	int ret = git_commit_tree( tree, *commit);
	if( ret != 0 )
	{
		lua_pushnil( L );
		lua_pushfstring( L, "can't find tree %d", ret );
		return 2;
	}

	luaL_getmetatable( L, LGIT_TREE_FUNCS );
	lua_setmetatable( L, -2 );
	return 1;
}
