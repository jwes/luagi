#include <git2/repository.h>

#include <lauxlib.h>

#include "repository.h"
#include "luagi.h"

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

int luagi_repository_wrap_odb( lua_State *L );
int luagi_repository_discover( lua_State *L );
int luagi_repository_open_ext( lua_State *L );
int luagi_repository_open_bare( lua_State *L );
int luagi_repository_init( lua_State *L );
int luagi_repository_init_ext( lua_State *L );
int luagi_repository_head( lua_State *L );
int luagi_repository_head_detatched( lua_State *L );
int luagi_repository_head_unborn( lua_State *L );
int luagi_repository_is_empty( lua_State *L );
int luagi_repository_path( lua_State *L );
int luagi_repository_workdir( lua_State *L );
int luagi_repository_set_workdir( lua_State *L );
int luagi_repository_is_bare( lua_State *L );
int luagi_repository_config( lua_State *L );
int luagi_repository_config_snapshot( lua_State *L );
int luagi_repository_odb( lua_State *L );
int luagi_repository_refdb( lua_State *L );
int luagi_repository_index( lua_State *L );
int luagi_repository_message( lua_State *L );
int luagi_repository_message_remove( lua_State *L );
int luagi_repository_state_cleanup( lua_State *L );
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


