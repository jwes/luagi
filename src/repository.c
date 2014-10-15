#include <git2/repository.h>

#include <lauxlib.h>

#include "repository.h"
#include "luagi.h"
#include "reference.h"
#include "index.h"
#include "types.h"
#include "oid.h"
#include "odb.h"
#include "refdb.h"

#include "config.h"

int luagi_open( lua_State *L )
{
   git_repository **repo;

   const char *path = luaL_checkstring( L, 1 );
   repo = (git_repository **) lua_newuserdata(L, sizeof(git_repository *) );
   
   if( git_repository_open( repo, path ) )
   {
      ERROR_PUSH( L )
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
   git_odb **odb = checkodb_at( L, 1 );

   git_repository **repo = lua_newuserdata( L, sizeof( git_repository * ) );

   if( git_repository_wrap_odb( repo, *odb ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, REPO_NAME );
   lua_setmetatable( L, -2 );
   return 1;
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
static unsigned int luagi_open_flags_from_table( lua_State *L, int index )
{
   luaL_checktype( L, index, LUA_TTABLE );

   unsigned int flags = GIT_REPOSITORY_OPEN_NO_SEARCH;

   add_flag( flags, L, index, CROSS_FS, GIT_REPOSITORY_OPEN_CROSS_FS );
   add_flag( flags, L, index, BARE, GIT_REPOSITORY_OPEN_BARE );

   return flags;
}

int luagi_repository_open_ext( lua_State *L )
{
   const char *path = luaL_checkstring( L, 1 );
   unsigned int flags = luagi_open_flags_from_table( L, 2 );
   const char *ceiling_dirs = luaL_optstring( L, 3, NULL );

   git_repository **repo = lua_newuserdata( L, sizeof( git_repository *) );
   if( git_repository_open_ext( repo, path, flags, ceiling_dirs ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, REPO_NAME );
   lua_setmetatable( L, -2 );
   return 1;
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

static int luagi_repo_init_init_options( lua_State *L, int index, git_repository_init_options *opts )
{
   luaL_checktype( L, index, LUA_TTABLE );
   int ret = git_repository_init_init_options( opts, GIT_REPOSITORY_INIT_OPTIONS_VERSION );

   add_flag( opts->flags, L, index, BARE, GIT_REPOSITORY_INIT_BARE );
   add_flag( opts->flags, L, index, NO_REINIT, GIT_REPOSITORY_INIT_NO_REINIT );
   add_flag( opts->flags, L, index, NO_DOTGIT_DIR, GIT_REPOSITORY_INIT_NO_DOTGIT_DIR );
   add_flag( opts->flags, L, index, MKDIR, GIT_REPOSITORY_INIT_MKDIR );
   add_flag( opts->flags, L, index, MKPATH, GIT_REPOSITORY_INIT_MKPATH );
   add_flag( opts->flags, L, index, EXTERNAL_TEMPLATE, GIT_REPOSITORY_INIT_EXTERNAL_TEMPLATE );

   lua_getfield( L, index, MODE );
   const char *mode = luaL_optstring( L, -1, NULL );
   if( mode && *mode )
   {
      switch( *mode )
      {
         case 'g':
            opts->mode = GIT_REPOSITORY_INIT_SHARED_GROUP;
            break;
         case 'a':
            opts->mode = GIT_REPOSITORY_INIT_SHARED_ALL;
            break;
         default:
         case 'u':
            opts->mode = GIT_REPOSITORY_INIT_SHARED_UMASK;
            break;
      }
   }

   lua_getfield( L, index, WORKDIR_PATH );
   const char *wpath = luaL_optstring( L, -1, NULL );
   if( wpath && *wpath )
   {
      opts->workdir_path = wpath;
   }

   lua_getfield( L, index, DESCRIPTION );
   const char *description = luaL_optstring( L, -1, NULL );
   if( description && *description )
   {
      opts->description = description;
   }

   lua_getfield( L, index, TEMPLATE_PATH );
   const char *template_path = luaL_optstring( L, -1, NULL );
   if( template_path && *template_path )
   {
      opts->template_path = template_path;
   }

   lua_getfield( L, index, INITIAL_HEAD );
   const char *initial_head = luaL_optstring( L, -1, NULL );
   if( initial_head && *initial_head )
   {
      opts->initial_head = initial_head;
   }

   lua_getfield( L, index, ORIGIN_URL );
   const char *origin_url = luaL_optstring( L, -1, NULL );
   if( origin_url && *origin_url )
   {
      opts->origin_url = origin_url;
   }

   return ret;
}

int luagi_repository_init_ext( lua_State *L )
{
   const char *path = luaL_checkstring( L, 1 );
   git_repository_init_options opts;
   luagi_repo_init_init_options( L, 2, &opts );

   git_repository **repo = lua_newuserdata( L, sizeof( git_repository * ) );

   if( git_repository_init_ext( repo, path, &opts ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, REPO_NAME );
   lua_setmetatable( L, -2 );
   return 1;
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

int luagi_get_config( lua_State *L, int (*func)( git_config **out, git_repository *repo ))
{
   git_repository **repo = checkrepo( L, 1 );

   git_config **out = lua_newuserdata( L, sizeof( git_config *) );

   if( func( out, *repo ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_CONFIG_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_repository_config( lua_State *L )
{
   return luagi_get_config( L, git_repository_config );
}

int luagi_repository_config_snapshot( lua_State *L )
{
   return luagi_get_config( L, git_repository_config_snapshot );
}

int luagi_repository_odb( lua_State *L )
{

   git_repository **repo = checkrepo( L, 1 );
   git_odb **odb = lua_newuserdata( L, sizeof( git_odb *) );

   if( git_repository_odb( odb, *repo ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_ODB_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_repository_refdb( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_refdb **refdb = lua_newuserdata( L, sizeof( git_refdb * ) );

   if( git_repository_refdb( refdb, *repo ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_REFDB_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
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
static int fetchhead_cb( const char *ref_name,
                        const char *remote_url,
                        const git_oid *oid,
                        unsigned int is_merge,
                        void *payload )
{
   luagi_foreach_t *p = payload;

   lua_pushvalue( p->L, p->callback_pos );
   lua_pushstring( p->L, ref_name );
   lua_pushstring( p->L, remote_url );
   luagi_push_oid( p->L, oid );
   lua_pushboolean( p->L, is_merge );

   if( lua_pcall( p->L, 5, 1, 0 ) )
   {
      luaL_error( p->L, "error calling fetchhead callback" );
      return 1;
   }

   int ret = luaL_checkinteger( p->L, -1 );
   lua_pop( p->L, 1 );
   return ret;
}

int luagi_repository_fetchhead_foreach( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );
   luaL_checktype( L, 2, LUA_TFUNCTION );
   p->L = L;
   p->callback_pos = 2;

   if( git_repository_fetchhead_foreach( *repo, fetchhead_cb, p ) )
   {
      ERROR_ABORT( p->L )
   }

   free( p );
   return 0;
}

static int mergehead_cb( const git_oid *oid, void *payload )
{
   luagi_foreach_t *p = payload;
   lua_pushvalue( p->L, p->callback_pos );
   luagi_push_oid( p->L, oid );

   if( lua_pcall( p->L, 1, 1, 0 ) )
   {
      luaL_error( p->L, "error calling fetchhead callback" );
      return 1;
   }

   int ret = luaL_checkinteger( p->L, -1 );
   lua_pop( p->L, 1 );
   return ret;
}

int luagi_repository_mergehead_foreach( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   
   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );
   luaL_checktype( L, 2, LUA_TFUNCTION );
   p->L = L;
   p->callback_pos = 2;

   if( git_repository_mergehead_foreach( *repo, mergehead_cb, p ) )
   {
      ERROR_ABORT( p->L )
   }

   free( p );
   return 0;
}

int luagi_repository_hashfile( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *path = luaL_checkstring( L, 2 );
   git_otype type = luagi_otype_from_string( luaL_checkstring( L, 3 ) );
   const char *as_path = luaL_optstring( L, 4, NULL );

   git_oid oid;
   if( git_repository_hashfile( &oid, *repo, path, type, as_path ) )
   {
      ERROR_PUSH( L )
   }

   return luagi_push_oid( L, &oid );
}

int luagi_repository_set_head( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *refname = luaL_checkstring( L, 2 );
   git_signature sig;
   table_to_signature( L, &sig, 3 );
   const char *log_message = luaL_checkstring( L, 4 );

   if( git_repository_set_head( *repo, refname, &sig, log_message ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_repository_set_head_detached( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid oid;
   luagi_check_oid( &oid, L, 2 );
   git_signature sig;
   table_to_signature( L, &sig, 3 );
   const char *log_message = luaL_checkstring( L, 4 );

   if( git_repository_set_head_detached( *repo, &oid, &sig, log_message ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_repository_detach_head( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_signature sig;
   table_to_signature( L, &sig, 3 );
   const char *log_message = luaL_checkstring( L, 4 );

   if( git_repository_detach_head( *repo, &sig, log_message ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

static int push_state( lua_State *L, int state )
{
   lua_pushinteger( L, state );
   return 1;
}

int luagi_repository_state( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   int state = git_repository_state( *repo );

   return push_state( L, state );
}

int luagi_repository_set_namespace( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *namespace = luaL_checkstring( L, 2 );

   if( git_repository_set_namespace( *repo, namespace ) )
   {
      ERROR_ABORT( L );
   }
   return 0;
}

int luagi_repository_get_namespace( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *namespace = git_repository_get_namespace( *repo );

   lua_pushstring( L, namespace );
   return 1;
}

int luagi_repository_is_shallow( lua_State *L )
{
   return luagi_generic_is( L, git_repository_is_shallow );
}



