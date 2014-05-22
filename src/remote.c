#include <lauxlib.h>
#include <git2/errors.h>
#include <git2/remote.h>
#include <string.h>
#include "remote.h"
#include "../lgit.h"

int lgit_remote_list( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_load( lua_State *L )
{ 
	git_repository **repo = checkrepo( L, 1 );
	const char *name = luaL_checkstring( L, 2 );

	git_remote **out = (git_remote **) lua_newuserdata( L, sizeof( git_remote *) );

	if( git_remote_load( out, *repo, name ) )
	{
		const git_error* err = giterr_last();
		lua_pushnil( L );
		lua_pushstring( L, err->message );
		return 2;
	}
	luaL_getmetatable( L, LGIT_REMOTE_FUNCS  );
	lua_setmetatable( L, -2 );
	return 1; 
}

int lgit_remote_create_anonymous( lua_State *L )
{ 
	git_repository **repo = checkrepo( L, 1 );
	const char *url = luaL_checkstring( L, 2 );
	const char *fetch = luaL_checkstring( L, 3);

	git_remote **out = (git_remote **) lua_newuserdata( L, sizeof( git_remote *) );

	if( git_remote_create_anonymous( out, *repo, url, fetch ) )
	{
		const git_error* err = giterr_last();
		lua_pushnil( L );
		lua_pushstring( L, err->message );
		return 2;
	}
	luaL_getmetatable( L, LGIT_REMOTE_FUNCS  );
	lua_setmetatable( L, -2 );
	return 1; 
}

int lgit_remote_create_with_fetchspec( lua_State *L )
{ 
	git_repository **repo = checkrepo( L, 1 );
	const char *name = luaL_checkstring( L, 2 );
	const char *url = luaL_checkstring( L, 3 );
	const char *fetch = luaL_optstring( L, 4, NULL );

	git_remote **out = (git_remote **) lua_newuserdata( L, sizeof( git_remote *) );

	if( git_remote_create_with_fetchspec( out, *repo, name, url, fetch ))
	{
		const git_error* err = giterr_last();
		lua_pushnil( L );
		lua_pushstring( L, err->message );
		return 2;
	}
	luaL_getmetatable( L, LGIT_REMOTE_FUNCS  );
	lua_setmetatable( L, -2 );
	return 1; 
}

int lgit_remote_create( lua_State *L )
{ 
	git_repository **repo = checkrepo( L, 1 );
	const char *name = luaL_checkstring( L, 2 );
	const char *url = luaL_checkstring( L, 3 );

	git_remote **out = (git_remote **) lua_newuserdata( L, sizeof( git_remote *) );

	if( git_remote_create( out, *repo, name, url ) )
	{
		const git_error* err = giterr_last();
		lua_pushnil( L );
		lua_pushstring( L, err->message );
		return 2;
	}
	
	luaL_getmetatable( L, LGIT_REMOTE_FUNCS  );
	lua_setmetatable( L, -2 );
	return 1; 
}

int lgit_remote_save( lua_State *L )
{ 
	git_remote** rem = checkremote( L );
	if( git_remote_save( *rem ) )
	{
		const git_error* err = giterr_last();
		luaL_error( L, err->message );
	}
	return 0; 
}

int lgit_remote_owner( lua_State *L )
{ 
	lua_pushnil( L ); 
	lua_pushstring( L, "not yet implemented" );
	return 2;
}
static int get_param( lua_State *L, const char *(*func)( const git_remote *remote ) )
{
	git_remote** rem = checkremote( L );
	const char *res = func( *rem );
	if( res == NULL )
	{
		lua_pushnil( L );
	}
	else 
	{
		lua_pushstring( L , res );
	}
	return 1;
}

static void set_param( lua_State* L, int (*func)(git_remote *remote, const char* param ) )
{
	git_remote** rem = checkremote( L );
	const char *param = luaL_checkstring( L, 1 );

	if( func( *rem, param ) )
	{
		const git_error* err = giterr_last();
		luaL_error( L, err->message );
	}
}

int lgit_remote_name( lua_State *L )
{ 
	return get_param( L, git_remote_name );
}
int lgit_remote_url( lua_State *L )
{ 
	return get_param( L, git_remote_url );
}
int lgit_remote_pushurl( lua_State *L )
{
	return get_param( L, git_remote_pushurl );
}
int lgit_remote_set_url( lua_State *L )
{ 
	set_param( L, git_remote_set_url );
	return 0; 
}
int lgit_remote_set_pushurl( lua_State *L )
{ 
	set_param( L, git_remote_set_pushurl );
	return 0; 
}
int lgit_remote_add_fetch( lua_State *L )
{ 
	set_param( L, git_remote_add_fetch );
	return 0; 
}
int get_refspecs( lua_State *L, int (*func)(git_strarray *array, const git_remote *remote ))
{ 
	git_remote **rem = checkremote( L );

	git_strarray array;

	if( func( &array, *rem ) )
	{
		const git_error *err = giterr_last();
		luaL_error( L, err->message );
	}
	lua_newtable( L );
	int nextstr = 0;
	for( unsigned int i = 0; i < array.count; i++)
	{
		char* next = *(array.strings + nextstr );
		lua_pushstring( L, next );
		lua_pushinteger( L, i + 1 );
		lua_settable( L, -3 );

		nextstr += strlen( next ) + 1;
	}
	git_strarray_free( &array );
	return 1; 
}
int lgit_remote_get_fetch_refspecs( lua_State *L )
{
	return get_refspecs( L, git_remote_get_fetch_refspecs );
}

static int set_refspecs( lua_State *L, int (*func)(git_remote *remote, git_strarray *array ) )
{ 
	git_remote **rem = checkremote( L );
	// get table an build git_strarray	
	luaL_checktype( L, 2, LUA_TTABLE );	
	unsigned int count = luaL_len( L, 2 );
	int size = 0;
	for( unsigned int i = 0; i < count; i++ )
	{
		lua_pushinteger( L, i );
		lua_gettable( L, 2 );
		const char* str = luaL_checkstring( L, -1 );
		size += strlen( str ) + 1;
	}
	char **strings = (char **) malloc( size );
	size_t pos = 0;
	for( unsigned int i = 0; i < count; i++ )
	{
		lua_pushinteger( L, i );
		lua_gettable( L, 2 );
		const char* str = luaL_checkstring( L, -1 );
		size_t len = strlen( str ) + 1;
		strncpy(strings[pos], str, len );
		pos += len;
	}

	git_strarray array;
	array.strings = strings;
	array.count = count;

	if( func( *rem, &array))
	{
		const git_error *err = giterr_last();
		luaL_error( L, err->message );
	}

	return 0; 
}

int lgit_remote_set_fetch_refspecs( lua_State *L )
{
	return set_refspecs( L, git_remote_set_fetch_refspecs );
}

int lgit_remote_add_push( lua_State *L )
{ 
	set_param( L, git_remote_add_push );
	return 0; 
}

int lgit_remote_get_push_refspecs( lua_State *L )
{ 
	return get_refspecs( L, git_remote_get_push_refspecs );
}

int lgit_remote_set_push_refspecs( lua_State *L )
{ 
	return set_refspecs( L, git_remote_set_push_refspecs );
}

int lgit_remote_clear_refspecs( lua_State *L )
{ 
	git_remote** rem = checkremote( L );
	git_remote_clear_refspecs( *rem );
	return 0;
}
int lgit_remote_refspec_count( lua_State *L )
{ 
	git_remote** rem = checkremote( L );
	size_t count = git_remote_refspec_count( *rem );
	lua_pushinteger( L, count );
	return 1; 
}
int lgit_remote_get_refspec( lua_State *L )
{ 
	lua_pushnil( L ); 
	lua_pushstring(L, "not yet implemented");
	return 2; 
}
int lgit_remote_connect( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_ls( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_download( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_fetch( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_connected( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_stop( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_disconnect( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int ligt_remote_free( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_check_cert( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_set_transport( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_set_callbacks( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_stats( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_autotag( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_set_autotag( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_rename( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_update_fetch_head( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_set_update_fetch_head( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_update_tips( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_is_valid_name( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_valid_url( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
int lgit_remote_supported_url( lua_State *L )
{ 
	lua_pushnil( L ); 
	return 1; 
}
