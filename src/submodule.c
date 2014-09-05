#include <git2/buffer.h>
#include <git2/submodule.h>

#include "submodule.h"
#include "luagi.h"

int luagi_submodule_lookup( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *name = luaL_checkstring( L, 2 );

   git_submodule **sub = lua_newuserdata( L, sizeof( git_submodule * ) );
   if( git_submodule_lookup( sub, *repo, name ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_SUBMODULE_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}
int sub_foreach( git_submodule *sm, const char *name, void *payload )
{
   luagi_foreach_t *p = payload;
   lua_pushvalue( p->L, p->callback_pos );
   git_submodule **sub = lua_newuserdata( p->L, sizeof( git_submodule * ) );
   *sub = sm;
   luaL_getmetatable( p->L, LUAGI_SUBMODULE_FUNCS );
   lua_setmetatable( p->L, -2 );

   lua_pushstring( p->L, name );

   if( lua_pcall( p->L, 2, 1, 0 ) )
   {
      luaL_error( p->L, "error calling the submodule callback" );
      return 1;
   }
   int ret = luaL_checkinteger( p->L, -1 );
   lua_pop( p->L, 1 );
   return ret;
}
int luagi_submodule_foreach( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   luaL_checktype( L, 2, LUA_TFUNCTION );
   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );
   p->L = L;
   p->callback_pos = 2;

   if( git_submodule_foreach( *repo, sub_foreach, p ) )
   {
      ERROR_ABORT( L )
   }
   free( p );
   return 0;
}
   
int luagi_submodule_add_setup( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *url = luaL_checkstring( L, 2 );
   const char *path = luaL_checkstring( L, 3 );
   int use_gitlink = lua_toboolean( L, 4 );
   
   git_submodule **sub = lua_newuserdata( L, sizeof( git_submodule * ) ); 
   if( git_submodule_add_setup( sub, *repo, url, path, use_gitlink ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_SUBMODULE_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_submodule_resolve_url( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *url = luaL_checkstring( L, 2 );

   git_buf buf;
   if( git_submodule_resolve_url( &buf, *repo, url ) )
   {
      ERROR_PUSH( L )
   }

   lua_pushlstring( L, buf.ptr, buf.size );
   return 1;
}

int luagi_submodule_reload_all( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   int force = lua_toboolean( L, 2 );

   if( git_submodule_reload_all( *repo, force ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_submodule_open( lua_State *L )
{
   git_submodule **sub = checksubmodule_at( L, 1 );

   git_repository **out = lua_newuserdata( L, sizeof( git_repository* ) );

   if( git_submodule_open( out, *sub ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, REPO_NAME );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_submodule_free( lua_State *L )
{
   git_submodule **sub = checksubmodule_at( L, 1 );
   git_submodule_free( *sub );
   return 0;
}

int luagi_submodule_add_finalize( lua_State *L )
{
   git_submodule **sub = checksubmodule_at( L, 1 );
   if( git_submodule_add_finalize( *sub ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_submodule_owner( lua_State *L )
{
   git_submodule **sub = checksubmodule_at( L, 1 );
   git_repository **out = lua_newuserdata( L, sizeof( git_repository* ) );

   *out = git_submodule_owner( *sub );

   luaL_getmetatable( L, REPO_NAME );
   lua_setmetatable( L, -2 );
   return 1;
}

static int luagi_sub_get_string( lua_State *L, const char * (*func)( git_submodule *sub ) )
{
   git_submodule **sub = checksubmodule_at( L, 1 );

   lua_pushstring( L, func( *sub ) );
   return 1;
}

int luagi_submodule_name( lua_State *L )
{
   return luagi_sub_get_string( L, git_submodule_name );
}

int luagi_submodule_path( lua_State *L )
{
   return luagi_sub_get_string( L, git_submodule_path );
}
int luagi_submodule_url( lua_State *L )
{
   return luagi_sub_get_string( L, git_submodule_url );
}

int luagi_submodule_branch( lua_State *L );
int luagi_submodule_set_url( lua_State *L );
int luagi_submodule_index_id( lua_State *L );
int luagi_submodule_head_id( lua_State *L );
int luagi_submodule_wd_id( lua_State *L );
int luagi_submodule_ignore( lua_State *L );
int luagi_submodule_set_ignore( lua_State *L );
int luagi_submodule_update( lua_State *L );
int luagi_submodule_set_update( lua_State *L );
int luagi_submodule_fetch_recurse_submodules( lua_State *L );
int luagi_submodule_set_fetch_recurse_submodules( lua_State *L );
int luagi_submodule_init( lua_State *L );
int luagi_submodule_sync( lua_State *L );
int luagi_submodule_reload( lua_State *L );
int luagi_submodule_status( lua_State *L );
int luagi_submodule_location( lua_State *L );
int luagi_submodule_add_to_index( lua_State *L );
int luagi_submodule_save( lua_State *L );
