#include <lua.h>
#include <lauxlib.h>
#include "tree.h"
#include <stdio.h>

#define checktreebuilder(L) \
		(git_treebuilder**) luaL_checkudata( L, 1, LGIT_TREE_BUILDER_FUNCS )
/* treebuilder stuff */
int lgit_tree_builder_create( lua_State *L )
{
	git_tree *tree = NULL;
	if( lua_isuserdata( L, 1) )
	{
		tree = *checktree( L );
	}
	git_treebuilder** builder = (git_treebuilder**) lua_newuserdata( L, sizeof( git_treebuilder* ) );

	int ret = git_treebuilder_create( builder, tree );
	if( ret != 0 )
	{
		lua_pushnil( L );
		lua_pushfstring( L, "could not create builder -- %d", ret );
		return 2;
	}
	
	luaL_getmetatable( L, LGIT_TREE_BUILDER_FUNCS );
	lua_setmetatable( L, -2 );
	return 1;
}

int lgit_tree_builder_gc( lua_State *L )
{
	git_treebuilder** builder = checktreebuilder( L );
	git_treebuilder_free( *builder );
	return 0;
}
int lgit_tree_builder_clear( lua_State *L )
{
	git_treebuilder** builder = checktreebuilder( L );
	git_treebuilder_clear( *builder );
	return 0;
}
int lgit_tree_builder_get( lua_State *L )
{
	git_treebuilder** builder = checktreebuilder( L );
	const char* filename = luaL_checkstring( L, 2 );
	const git_tree_entry* entry = git_treebuilder_get( *builder, filename );
	if( entry == NULL )
	{
		lua_pushnil( L );
		lua_pushstring( L, "git_treebuilder_get failed" );
		return 2;
	}
	git_tree_entry** e = (git_tree_entry**) lua_newuserdata( L, sizeof( git_tree_entry*) );
	int ret = git_tree_entry_dup( e, entry );
	if( ret != 0 )
	{
		lua_pushnil( L );
		lua_pushfstring( L, "git_tree_entry_dup failed %d", ret );
		return 2;
	}
	luaL_getmetatable( L, LGIT_TREE_ENTRY_FUNCS );
	lua_setmetatable( L, -2 );
	return 1;
}
int lgit_tree_builder_insert( lua_State *L )
{
	git_treebuilder** builder = checktreebuilder( L );
	const char* filename = luaL_checkstring( L, 2 );
	const char* ref = luaL_checkstring( L, 3 );
	git_oid oid;
	int ret = lgit_oid_fromstr( &oid, ref );
	if( ret != 0 )
	{
		lua_pushnil( L );
		lua_pushstring( L, "invalid ref" );
		return 2;
	}
	const char* filemode = luaL_checkstring( L, 4 );
	unsigned int value;
	if( 1 != sscanf(filemode, "%x", &value) )
	{
		lua_pushnil( L );
		lua_pushstring( L, "invalid filemode" );
		return 2;
	}
	const git_tree_entry** entry = (const git_tree_entry**) lua_newuserdata( L, sizeof( git_tree_entry* ) );
	ret = git_treebuilder_insert( entry, *builder, filename, &oid, (git_filemode_t)value );
	if( ret != 0 )
	{
		lua_pushnil( L );
		lua_pushfstring( L, "error inserting entry -- %d", ret );
		return 2;
	}
	luaL_getmetatable( L, LGIT_TREE_ENTRY_FUNCS );
	lua_setmetatable( L, -2 );
	return 1;
}
int lgit_tree_builder_remove( lua_State *L )
{
	git_treebuilder** builder = checktreebuilder( L );
	const char* filename = luaL_checkstring( L, 2 );
	int ret = git_treebuilder_remove( *builder, filename );
	lua_pushinteger( L, ret );
	return 1;
}
int lgit_tree_builder_entry_count( lua_State *L )
{
	git_treebuilder** builder = checktreebuilder( L );
	lua_pushinteger( L, git_treebuilder_entrycount( *builder ) );
	return 1;
}
int lgit_tree_builder_write( lua_State *L )
{
	git_treebuilder** builder = checktreebuilder( L );
	git_repository** repo = checkrepo( L, 2 );
	git_oid oid;
	
	git_treebuilder_write( &oid, *repo, *builder );
	char array [ GIT_OID_HEXSZ + 1 ];
	git_oid_tostr( array, sizeof(array), &oid );
	lua_pushstring( L, array );
	return 1;
}

struct filter_payload 
{
	lua_State *L;
	lua_CFunction function;
};

static int builder_filter( const git_tree_entry *entry, void *payload )
{
	struct filter_payload *fp = (struct filter_payload *) payload;

	lua_pushcfunction( fp->L, fp->function);
	git_tree_entry** e = (git_tree_entry**) lua_newuserdata( fp->L, sizeof( git_tree_entry* ) );
	
	if( git_tree_entry_dup( e, entry ))
	{
		luaL_error( fp->L, "dup failed" );
	}

	luaL_getmetatable( fp->L, LGIT_TREE_ENTRY_FUNCS );
	lua_setmetatable( fp->L, -2 );

	if(lua_pcall( fp->L, 2, 1, 0) )
	{
		luaL_error( fp->L, "failure calling function" );
	}
	return lua_toboolean( fp->L, -1 );
}

int lgit_tree_builder_filter( lua_State *L )
{
	git_treebuilder** builder = checktreebuilder( L );
	if( lua_iscfunction( L, 2 ) == 0 )
	{
		luaL_error( L, "function( tree_entry entry ) parameter required" );
	}
	lua_CFunction function = lua_tocfunction( L, 2 );

	struct filter_payload *payload = (struct filter_payload *) malloc( sizeof( struct filter_payload ) );
	payload->L = L;
	payload->function = function;

	git_treebuilder_filter( *builder, builder_filter, payload );

	free( payload );
	return 0;
}
