#include <git2/buffer.h>
#include <git2/submodule.h>

#include "submodule.h"
#include "luagi.h"
#include "oid.h"

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

int luagi_submodule_branch( lua_State *L )
{
   return luagi_sub_get_string( L, git_submodule_branch );
}

static int luagi_sub_set_string( lua_State *L, int (*func)( git_submodule *sub, const char *str ) )
{
   git_submodule **sub = checksubmodule_at( L, 1 );
   const char *str = luaL_checkstring( L, 2 );

   if( func( *sub, str ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_submodule_set_url( lua_State *L )
{
   return luagi_sub_set_string( L, git_submodule_set_url );
}

static int luagi_sub_get_id( lua_State *L, const git_oid * (*func)(git_submodule *sub ) )
{
   git_submodule **sub = checksubmodule_at( L, 1 );
   const git_oid *oid = func( *sub );
   return luagi_push_oid( L, oid );
}

int luagi_submodule_index_id( lua_State *L )
{
   return luagi_sub_get_id( L, git_submodule_index_id );
}

int luagi_submodule_head_id( lua_State *L )
{
   return luagi_sub_get_id( L, git_submodule_head_id );
}

int luagi_submodule_wd_id( lua_State *L )
{
   return luagi_sub_get_id( L, git_submodule_wd_id );
}

static int luagi_sub_push_ignore( lua_State *L, git_submodule_ignore_t ignore )
{
   switch ( ignore )
   {
      default:
      case GIT_SUBMODULE_IGNORE_NONE: 
         lua_pushstring( L, NONE );
         break;
      case GIT_SUBMODULE_IGNORE_UNTRACKED: 
         lua_pushstring( L, UNTRACKED );
         break;
      case GIT_SUBMODULE_IGNORE_DIRTY: 
         lua_pushstring( L, DIRTY );
         break;
      case GIT_SUBMODULE_IGNORE_ALL: 
         lua_pushstring( L, ALL );
         break;
      case GIT_SUBMODULE_IGNORE_DEFAULT: 
         lua_pushstring( L, DEFAULT );
         break;
   }
   return 1;
}

static git_submodule_ignore_t luagi_sub_check_ignore( lua_State *L, int index )
{
   const char *ignore = luaL_checkstring( L, index );
   
   switch( ignore[0] )
   {
      case 'a':
         return GIT_SUBMODULE_IGNORE_ALL;
      case 'd':
         return GIT_SUBMODULE_IGNORE_DIRTY;
      case 'n':
         return GIT_SUBMODULE_IGNORE_NONE;
      case 'u':
         return GIT_SUBMODULE_IGNORE_UNTRACKED;
      case 'r':
         return GIT_SUBMODULE_IGNORE_RESET;
      default:
         return GIT_SUBMODULE_IGNORE_DEFAULT;
       
   }         
}

int luagi_submodule_ignore( lua_State *L )
{
   git_submodule **sub = checksubmodule_at( L, 1 );
   git_submodule_ignore_t type = git_submodule_ignore( *sub );
   return luagi_sub_push_ignore( L, type );
}

int luagi_submodule_set_ignore( lua_State *L )
{
   git_submodule **sub = checksubmodule_at( L, 1 );
   git_submodule_ignore_t ign = luagi_sub_check_ignore( L, 2 );

   git_submodule_ignore_t old =  git_submodule_set_ignore( *sub, ign );
   return luagi_sub_push_ignore( L, old );
}

static int luagi_sub_push_update( lua_State *L, git_submodule_update_t update )
{
   switch ( update )
   {
      default:
      case GIT_SUBMODULE_UPDATE_NONE:
              // none is none, use IGNORE define
         lua_pushstring( L, NONE );
         break;
      case GIT_SUBMODULE_UPDATE_CHECKOUT:
         lua_pushstring( L, CHECKOUT );
         break;
      case GIT_SUBMODULE_UPDATE_MERGE:
         lua_pushstring( L, MERGE );
         break;
      case GIT_SUBMODULE_UPDATE_REBASE:
         lua_pushstring( L, REBASE );
         break;
      case GIT_SUBMODULE_UPDATE_DEFAULT:
         lua_pushstring( L, DEFAULT );
         break;
   }
   return 1;
}

static git_submodule_update_t luagi_sub_check_update( lua_State *L, int index )
{
   const char *update = luaL_checkstring( L, index );
   
   switch( update[0] )
   {
      case 'c':
         return GIT_SUBMODULE_UPDATE_CHECKOUT;
      case 'm':
         return GIT_SUBMODULE_UPDATE_MERGE;
      case 'n':
         return GIT_SUBMODULE_UPDATE_NONE;
      case 'r':
         if( update[2] == 'b' )
         {
            return GIT_SUBMODULE_UPDATE_REBASE;
         }
         else
         {
            return GIT_SUBMODULE_UPDATE_RESET;
         }
      default:
         return GIT_SUBMODULE_UPDATE_DEFAULT;
   }         
   return GIT_SUBMODULE_UPDATE_DEFAULT;
}

int luagi_submodule_update( lua_State *L )
{
   git_submodule **sub = checksubmodule_at( L, 1 );
   git_submodule_update_t t = git_submodule_update( *sub );
   return luagi_sub_push_update( L, t );
}
int luagi_submodule_set_update( lua_State *L )
{
   git_submodule **sub = checksubmodule_at( L, 1 );
   git_submodule_update_t value = luagi_sub_check_update( L, 2 );
   git_submodule_update_t t = git_submodule_set_update( *sub, value );
   return luagi_sub_push_update( L, t );
}

static int luagi_sub_push_recurse( lua_State *L, git_submodule_recurse_t recurse )
{
   switch ( recurse )
   {
      default:
      case GIT_SUBMODULE_RECURSE_NO:
         lua_pushstring( L, RECURSE_NO );
         break;
      case GIT_SUBMODULE_RECURSE_YES:
         lua_pushstring( L, RECURSE_YES );
         break;
      case GIT_SUBMODULE_RECURSE_ONDEMAND:
         lua_pushstring( L, RECURSE_ONDEMAND );
         break;
   }
   return 1;
}

static git_submodule_recurse_t luagi_sub_check_recurse( lua_State *L, int index )
{
   const char *recurse = luaL_checkstring( L, index );
   
   switch( recurse[0] )
   {
      case 'y':
         return GIT_SUBMODULE_RECURSE_YES;
      case 'n':
         return GIT_SUBMODULE_RECURSE_NO;
      case 'o':
         return GIT_SUBMODULE_RECURSE_ONDEMAND;
      default:
      case 'r':
         return GIT_SUBMODULE_RECURSE_RESET;
   }         
}



int luagi_submodule_fetch_recurse_submodules( lua_State *L )
{
   git_submodule **sub = checksubmodule_at( L, 1 );
   git_submodule_recurse_t t = git_submodule_fetch_recurse_submodules( *sub );
   return luagi_sub_push_recurse( L, t );
}

int luagi_submodule_set_fetch_recurse_submodules( lua_State *L )
{
   git_submodule **sub = checksubmodule_at( L, 1 );
   git_submodule_recurse_t recurse = luagi_sub_check_recurse( L, 2 );
   git_submodule_recurse_t t = git_submodule_set_fetch_recurse_submodules( *sub, recurse );
   return luagi_sub_push_recurse( L, t );
}

int luagi_submodule_init( lua_State *L )
{
   git_submodule **sub = checksubmodule_at( L, 1 );
   int overwrite = lua_toboolean( L, 2 );

   if( git_submodule_init( *sub, overwrite ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_submodule_sync( lua_State *L )
{
   git_submodule **sub = checksubmodule_at( L, 1 );

   if( git_submodule_sync( *sub ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_submodule_reload( lua_State *L )
{
   git_submodule **sub = checksubmodule_at( L, 1 );
   int force = lua_toboolean( L, 2 );

   if( git_submodule_reload( *sub, force ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

static int luagi_get_status( lua_State *L, int (*func)( unsigned int *status, git_submodule *sub ) )
{
   git_submodule **sub = checksubmodule_at( L, 1 );

   unsigned int status;

   if( func( &status, *sub ) )
   {
      ERROR_PUSH( L );
   }

   //TODO flags 
   lua_pushinteger( L, status );
   return 1;
}

int luagi_submodule_status( lua_State *L )
{
   return luagi_get_status( L, git_submodule_status ); 
}

int luagi_submodule_location( lua_State *L )
{
   return luagi_get_status( L, git_submodule_location ); 
}

int luagi_submodule_add_to_index( lua_State *L )
{
   git_submodule **sub = checksubmodule_at( L, 1 );
   int write_index = lua_toboolean( L, 2 );

   if( git_submodule_add_to_index( *sub, write_index ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_submodule_save( lua_State *L )
{
   git_submodule **sub = checksubmodule_at( L, 1 );

   if( git_submodule_save( *sub ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

