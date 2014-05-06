#include <lauxlib.h>
#include <git2/oid.h>
#include <git2/errors.h>
#define __USE_BSD
#include <string.h>
#undef __USE_BSD

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
	int ret = lgit_oid_fromstr( &oid, id_str);
	if( ret != 0 )
	{
		lua_pushnil( L );
		lua_pushstring(L, "the given string is no valid git oid");
		return 2;
	}

	git_tree** out = (git_tree**)lua_newuserdata( L, sizeof(git_tree*) );
	if( git_tree_lookup( out, *repo, &oid ) )
	{
		const git_error* err = giterr_last();
		lua_pushnil( L );
		lua_pushfstring(L, "failed to lookup tree: %s", err->message );
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
static int tree_entry_by_final( lua_State *L, const git_tree_entry* entry )
{
	git_tree_entry** e = (git_tree_entry**) lua_newuserdata(L, sizeof(git_tree_entry*) );

	int ret = git_tree_entry_dup( e, entry );
	if( ret != 0 )
	{
		lua_pushnil( L );
		lua_pushfstring( L, "dup failed, %d", ret );
		return 2;
	}
	luaL_getmetatable( L, LGIT_TREE_ENTRY_FUNCS );
	lua_setmetatable( L, -2 );
	return 1;
}

int lgit_tree_entry_byname( lua_State *L )
{
	git_tree** tree = checktree( L );
	const char* filename = luaL_checkstring( L , 2 );

	const git_tree_entry* e = git_tree_entry_byname( *tree, filename );
	if( e == NULL )
	{
		lua_pushnil( L );
		lua_pushfstring( L, "file: %s not found", filename );
		return 2;
	}
	return tree_entry_by_final( L, e );
}

int lgit_tree_entry_byindex( lua_State *L )
{
	git_tree** tree = checktree( L );
	unsigned int index = luaL_checkunsigned( L , 2 );

	const git_tree_entry* e = git_tree_entry_byindex( *tree, index );
	if( e == NULL )
	{
		lua_pushnil( L );
		lua_pushfstring( L, "file: index %d not found", index );
		return 2;
	}
	return tree_entry_by_final(L, e );
}

int lgit_tree_entry_byid( lua_State *L )
{
	git_tree** tree = checktree( L );
	const char* idstring = luaL_checkstring( L , 2 );

	git_oid oid;
	int ret = lgit_oid_fromstr( &oid, idstring );
	if( ret != 0 )
	{
		lua_pushnil( L );
		lua_pushfstring( L, "%s is no valid id", idstring );
		return 2;
	}

	const git_tree_entry* e = git_tree_entry_byid( *tree, &oid );
	if( e == NULL )
	{
		lua_pushnil( L );
		lua_pushfstring( L, "file: index %s not found", idstring );
		return 2;
	}
	return tree_entry_by_final(L, e );
}

/* user owned tree entries */
int lgit_tree_entry_bypath( lua_State *L )
{
	git_tree** tree = checktree( L );
	const char* path = luaL_checkstring( L , 2 );

	git_tree_entry** entry = (git_tree_entry**) lua_newuserdata(L, sizeof(git_tree_entry*) );

	int ret = git_tree_entry_bypath( entry, *tree, path );
	if( ret != 0 )
	{
		lua_pushnil( L );
		if( ret == GIT_ENOTFOUND )
		{
			lua_pushfstring( L, "file: %s not found", path );
		}
		else 
		{
			lua_pushfstring( L, "unknown error bypath %d", ret );
		}
		return 2;
	}
	luaL_getmetatable( L, LGIT_TREE_ENTRY_FUNCS );
	lua_setmetatable( L, -2 );
	return 1;
}

int lgit_tree_entry_gc( lua_State *L )
{
	git_tree_entry** entry = checktreeentry( L, 1 );
	git_tree_entry_free( *entry );
	return 0;
}

/* entry stuff */
int lgit_tree_entry_name( lua_State *L )
{
	git_tree_entry** entry = checktreeentry( L, 1 );
	lua_pushstring( L, git_tree_entry_name( *entry ));
	return 1;
}
int lgit_tree_entry_id( lua_State *L )
{
	git_tree_entry** entry = checktreeentry( L, 1 );	
	const git_oid* oid = git_tree_entry_id( *entry );

	char array [GIT_OID_HEXSZ + 1];
	lua_pushstring( L, git_oid_tostr( array, sizeof(array), oid ));
	return 1;
	
}
int lgit_tree_entry_type( lua_State *L )
{
	git_tree_entry** entry = checktreeentry( L, 1 );
	git_otype type = git_tree_entry_type( *entry );
	lua_pushstring( L, lgit_otype_to_string( type ) );
	return 1;
}
int lgit_tree_entry_filemode( lua_State *L )
{
	git_tree_entry** entry = checktreeentry( L, 1 );
	git_filemode_t filemode =  git_tree_entry_filemode( *entry );
	char array[10];
	int ret = snprintf( array, 10, "%o", filemode );
	if( ret < 0)
	{
		lua_pushnil( L );
	}
	else
	{
		lua_pushstring( L, array );
	}
	return 1;
}

int lgit_tree_entry_cmp( lua_State *L )
{
	git_tree_entry** e1 = checktreeentry( L, 1 );	
	git_tree_entry** e2 = checktreeentry( L, 2 );	
	lua_pushinteger( L, git_tree_entry_cmp( *e1, *e2 ));
	return 1;
}

int lgit_tree_entry_to_object( lua_State *L )
{
	//TODO
	lua_pushnil( L );
	return 1;
}

struct walk_info_element {
	char *root;
	git_tree_entry *entry;
	struct walk_info_element *next;
};

struct walk_info {
	struct walk_info_element *next;
	struct walk_info_element *last;
	git_tree **tree;
	git_treewalk_mode mode;
};

static void free_elem( struct walk_info_element *elem )
{
	free( elem->root );
	git_tree_entry_free( elem->entry );
	free( elem );
}

static int treewalk_callback( const char *root, const git_tree_entry *entry,
				void *payload)
{
	struct walk_info* info = (struct walk_info*) payload;

	struct walk_info_element *elem = (struct walk_info_element*) malloc( sizeof( struct walk_info_element ));
	elem->root = strdup( root );
	elem->next = NULL;
	int ret = git_tree_entry_dup(&elem->entry, entry );
	if( ret != 0 )
	{
		return -1;
	}
	if( info->last == NULL )
	{
		info->next = elem;
		info->last = elem;
	}
	else
	{
		info->last->next = elem;
		info->last = elem;
	}
	return 1;
}

static int lgit_walker( lua_State *L );

int lgit_tree_walk( lua_State *L )
{
	git_tree** tree = checktree( L );
	const char* direction = luaL_optstring( L, 2, LGIT_TREE_PRE );
	git_treewalk_mode mode;
	if( strncmp( direction, LGIT_TREE_PRE, 3 ) == 0 )
	{
		mode = GIT_TREEWALK_PRE;
	}
	else if ( strncmp( direction, LGIT_TREE_POST, 4 ) == 0 )
	{
		mode = GIT_TREEWALK_POST;
	} else {
		luaL_error( L, "wrong order, user 'pre' or 'post'" );
	}
	struct walk_info* info = (struct walk_info*) lua_newuserdata(L, sizeof(struct walk_info ) );
	luaL_getmetatable(L, LGIT_TREE_WALK_STATICS);
	lua_setmetatable(L, -2);

	info->next = NULL;
	info->last = NULL;
	info->mode = mode;
	info->tree = tree;

	git_tree_walk( *info->tree, info->mode, &treewalk_callback, info );

	// TODO
	// start thread, that does the actual walking
	// create semaphore, which allows waiting for walkresults.
	// read in walker from semaphore and than from info
	// write to info in the thread
	lua_pushcclosure( L, lgit_walker, 1 );
	return 1;
}

static int lgit_walker( lua_State *L )
{
	struct walk_info* info = (struct walk_info*) lua_touserdata( L, lua_upvalueindex( 1 ) );
	struct walk_info_element *elem;
	if ( info->next != NULL )
	{
		elem = info->next;
		info->next = elem->next;
		lua_pushstring( L, elem->root );
		git_tree_entry** e = (git_tree_entry**) lua_newuserdata( L, sizeof( git_tree_entry* ) );
		int ret = git_tree_entry_dup( e, elem->entry );
		if( ret != 0 )
		{
			lua_pushnil( L );
			lua_pushstring( L, "out of memory - dup failed" );
		}	
		
		luaL_getmetatable( L, LGIT_TREE_ENTRY_FUNCS );
		lua_setmetatable( L, -2 );
		free_elem( elem );
		return 2;
	}//iteration finished
	return 0;
}
