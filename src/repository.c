#include <git2/repository.h>

#include <lauxlib.h>

#include "repository.h"
#include "luagi.h"
#include "reference.h"
#include "index.h"

int luagi_open( lua_State *L )
{
   git_repository **repo;

   const char *path = luaL_checkstring( L, 1 );
   repo = (git_repository **) lua_newuserdata(L, sizeof(git_repository *) );
   
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

int luagi_gc( lua_State *L )
{
   git_repository **repo = checkrepo(L, 1);
   git_repository_free( *repo );
   return 0;
}

int luagi_repository_wrap_odb( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
   return 0;
}

int luagi_repository_discover( lua_State *L )
{
   const char *start_path = luaL_checkstring( L, 1 );
   int across_fs = lua_toboolean( L, 2 );
   const char *ceiling_dirs = luaL_optstring( L, 3, NULL );

   git_buf out;

   if( git_repository_discover( &out, start_path, across_fs, ceiling_dirs ) )
   {
      ERROR_PUSH( L )
   }

   lua_pushlstring( L, out.ptr, out.size );
   return 1;
}

int luagi_repository_open_ext( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
   return 0;
}

int luagi_repository_open_bare( lua_State *L )
{
   const char *bare_path = luaL_checkstring( L, 1 );
   git_repository **repo = lua_newuserdata( L, sizeof( git_repository *) );

   if( git_repository_open_bare( repo, bare_path ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, REPO_NAME );
   lua_setmetatable( L, -2 );
   return 1;
}
   
int luagi_repository_init( lua_State *L )
{
   const char *path = luaL_checkstring( L, 1 );
   unsigned is_bare = lua_toboolean( L, 2 );

   git_repository **repo = lua_newuserdata( L, sizeof( git_repository *) );

   if( git_repository_init( repo, path, is_bare ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, REPO_NAME );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_repository_init_ext( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
   return 0;
}

int luagi_repository_head( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_reference **reference = lua_newuserdata( L, sizeof( git_reference *) );

   if( git_repository_head( reference, *repo ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_REFERENCE_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}
static int luagi_generic_is( lua_State *L, int (*func)( git_repository *repo ) )
{
   git_repository **repo = checkrepo( L, 1 );

   lua_pushboolean( L, func( *repo ) );
   return 1;
}

int luagi_repository_head_detatched( lua_State *L )
{
   return luagi_generic_is( L, git_repository_head_detached );
}

int luagi_repository_head_unborn( lua_State *L )
{
   return luagi_generic_is( L, git_repository_head_unborn );
}

int luagi_repository_is_empty( lua_State *L )
{
   return luagi_generic_is( L, git_repository_is_empty );
}

int luagi_repository_path( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   lua_pushstring( L, git_repository_path( *repo ) );
   return 1;
}

int luagi_repository_workdir( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   lua_pushstring( L, git_repository_workdir( *repo ) );
   return 1;
}

int luagi_repository_set_workdir( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *workdir = luaL_checkstring( L, 2 );
   int update_gitlink = lua_toboolean( L, 3 );

   if( git_repository_set_workdir( *repo, workdir, update_gitlink ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_repository_is_bare( lua_State *L )
{
   return luagi_generic_is( L, git_repository_is_bare );
}

int luagi_repository_config( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
   return 0;
}

int luagi_repository_config_snapshot( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
   return 0;
}

int luagi_repository_odb( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
   return 0;
}

int luagi_repository_refdb( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
   return 0;
}

int luagi_repository_index( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   git_index **index = lua_newuserdata( L, sizeof( git_index *) );

   if( git_repository_index( index, *repo ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_INDEX_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_repository_message( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   git_buf buf;

   if( git_repository_message( &buf, *repo ) )
   {
      ERROR_PUSH( L )
   }

   lua_pushlstring( L, buf.ptr, buf.size );
   return 1;
}

int luagi_repository_message_remove( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   if( git_repository_message_remove( *repo ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_repository_state_cleanup( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   if( git_repository_state_cleanup( *repo ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_repository_fetchhead_foreach( lua_State *L );
int luagi_repository_mergehead_foreach( lua_State *L );
int luagi_repository_hashfile( lua_State *L );
int luagi_repository_set_head( lua_State *L );
int luagi_repository_set_head_detached( lua_State *L );
int luagi_repository_detach_head( lua_State *L );
int luagi_repository_state( lua_State *L );
int luagi_repository_set_namespace( lua_State *L );
int luagi_repository_get_namespace( lua_State *L );
int luagi_repository_is_shallow( lua_State *L );


