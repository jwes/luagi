#include <lauxlib.h>
#include <git2/errors.h>
#include <git2/remote.h>
#include <git2/signature.h>
#include <string.h>
#include "remote.h"
#include "luagi.h"
#include "transport.h"
#include "oid.h"

int luagi_remote_list( lua_State *L )
{ 
   git_repository **repo = checkrepo( L, 1 );

   git_strarray array;
   if( git_remote_list( &array, *repo ) )
   {
      const git_error *err = giterr_last();
      lua_pushnil( L );
      lua_pushstring( L, err->message );
      return 2;
   }
   luagi_lua_list_from_string( L, &array );
   return 1;
}

int luagi_remote_load( lua_State *L )
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
   luaL_getmetatable( L, LUAGI_REMOTE_FUNCS  );
   lua_setmetatable( L, -2 );
   return 1; 
}

int luagi_remote_create_anonymous( lua_State *L )
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
   luaL_getmetatable( L, LUAGI_REMOTE_FUNCS  );
   lua_setmetatable( L, -2 );
   return 1; 
}

int luagi_remote_create_with_fetchspec( lua_State *L )
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
   luaL_getmetatable( L, LUAGI_REMOTE_FUNCS  );
   lua_setmetatable( L, -2 );
   return 1; 
}

int luagi_remote_create( lua_State *L )
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
   
   luaL_getmetatable( L, LUAGI_REMOTE_FUNCS  );
   lua_setmetatable( L, -2 );
   return 1; 
}

int luagi_remote_save( lua_State *L )
{ 
   git_remote** rem = checkremote( L );
   if( git_remote_save( *rem ) )
   {
      const git_error* err = giterr_last();
      luaL_error( L, err->message );
   }
   return 0; 
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
   const char *param = luaL_checkstring( L, 2 );

   if( func( *rem, param ) )
   {
      const git_error* err = giterr_last();
      luaL_error( L, err->message );
   }
}

int luagi_remote_name( lua_State *L )
{ 
   return get_param( L, git_remote_name );
}
int luagi_remote_url( lua_State *L )
{ 
   return get_param( L, git_remote_url );
}
int luagi_remote_pushurl( lua_State *L )
{
   return get_param( L, git_remote_pushurl );
}
int luagi_remote_set_url( lua_State *L )
{ 
   set_param( L, git_remote_set_url );
   return 0; 
}
int luagi_remote_set_pushurl( lua_State *L )
{ 
   set_param( L, git_remote_set_pushurl );
   return 0; 
}
int luagi_remote_add_fetch( lua_State *L )
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
int luagi_remote_get_fetch_refspecs( lua_State *L )
{
   return get_refspecs( L, git_remote_get_fetch_refspecs );
}

static int set_refspecs( lua_State *L, int (*func)(git_remote *remote, git_strarray *array ) )
{ 
   git_remote **rem = checkremote( L );
   // get table an build git_strarray  
   luaL_checktype( L, 2, LUA_TTABLE ); 

   git_strarray array = luagi_strings_from_lua_list( L, 2 );

   if( func( *rem, &array))
   {
      const git_error *err = giterr_last();
      luaL_error( L, err->message );
   }

   return 0; 
}

int luagi_remote_set_fetch_refspecs( lua_State *L )
{
   return set_refspecs( L, git_remote_set_fetch_refspecs );
}

int luagi_remote_add_push( lua_State *L )
{ 
   set_param( L, git_remote_add_push );
   return 0; 
}

int luagi_remote_get_push_refspecs( lua_State *L )
{ 
   return get_refspecs( L, git_remote_get_push_refspecs );
}

int luagi_remote_set_push_refspecs( lua_State *L )
{ 
   return set_refspecs( L, git_remote_set_push_refspecs );
}

int luagi_remote_clear_refspecs( lua_State *L )
{ 
   git_remote** rem = checkremote( L );
   git_remote_clear_refspecs( *rem );
   return 0;
}
int luagi_remote_refspec_count( lua_State *L )
{ 
   git_remote** rem = checkremote( L );
   size_t count = git_remote_refspec_count( *rem );
   lua_pushinteger( L, count );
   return 1; 
}

/** we return a table with src, dest, string-representation
  force flag, and direction.
 if someone needs the transform methods, we have to create the userdata**/
int luagi_remote_get_refspec( lua_State *L )
{ 
   git_remote** rem = checkremote( L );
   size_t n = luaL_checkinteger( L , 2 );

   const git_refspec *spec = git_remote_get_refspec( *rem, n );
   if( spec == NULL )
   {
      lua_pushnil( L );
      lua_pushfstring( L, "no element at pos %d ", n );
      return 2;
   }
   
   lua_newtable( L );
   lua_pushstring( L, git_refspec_src( spec ) );
   lua_setfield( L, -2, SRC );

   lua_pushstring( L, git_refspec_dst( spec ) );
   lua_setfield( L, -2, DEST );

   lua_pushstring( L, git_refspec_string( spec ) );
   lua_setfield( L, -2, STRING );

   lua_pushboolean( L, git_refspec_force( spec ) );
   lua_setfield( L, -2, FORCE );

   git_direction dir = git_refspec_direction( spec );
   switch( dir )
   {
      case GIT_DIRECTION_FETCH:
         lua_pushstring( L, FETCH );
         break;
      case GIT_DIRECTION_PUSH:
         lua_pushstring( L, PUSH );
         break;
      default:
         lua_pushstring( L, UNKNOWN );
         break;
   }
   lua_setfield( L, -2, DIRECTION );

   return 1; 
}
int luagi_remote_connect( lua_State *L )
{ 
   git_remote** rem = checkremote( L );
   const char* direction = luaL_optstring( L, 2, NULL );

   git_direction dir = GIT_DIRECTION_FETCH;
   if( direction != NULL && strncmp( PUSH, direction, strlen( PUSH ) ) == 0 )
   {
      dir = GIT_DIRECTION_PUSH;
   }

   if( git_remote_connect( *rem, dir ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_remote_ls( lua_State *L )
{ 
   git_remote **remote = checkremote( L );

   if( ! git_remote_connected( *remote ) )
   {
      lua_pushnil( L );
      lua_pushstring( L, "remote has to be connected" );
      return 2;
   }

   const git_remote_head **heads;
   size_t size;
   if( git_remote_ls( &heads, &size, *remote ) )
   {
      ERROR_PUSH( L )
   }
   //return a table
   lua_newtable( L );
   for( size_t i = 0; i < size; ++i )
   {
      lua_pushinteger( L, i + 1 );
      lua_newtable( L );
      const git_remote_head *head = heads[i];

      lua_pushboolean( L, head->local );
      lua_setfield( L, -2, LOCAL );

      luagi_push_oid( L, &head->oid );
      lua_setfield( L, -2, OID );

      luagi_push_oid( L, &head->loid );
      lua_setfield( L, -2, LOID );

      lua_pushstring( L, head->name );
      lua_setfield( L, -2, NAME );

      if( head->symref_target )
      {
         lua_pushstring( L, head->symref_target );
         lua_setfield( L, -2, TARGET );
      }
      lua_settable( L, -3 );
   }
   return 1; 
}
int luagi_remote_download( lua_State *L )
{ 
   git_remote **rem = checkremote( L );
   if( git_remote_download( *rem ) )
   {
      const git_error *err = giterr_last();
      luaL_error( L, err->message );
   }
   return 0;
}
static int call_with_sig( lua_State *L, int (*func)( git_remote *remote, const git_signature *signature, const char *reflog_message ))
{ 
   git_remote **rem = checkremote( L );
   luaL_checktype( L, 2, LUA_TTABLE );
   git_signature *sig;

   table_to_signature( L, &sig, 2 );

   const char* reflog_message = luaL_optstring( L, 3, NULL );

   if( func( *rem, sig, reflog_message ))
   {
      git_signature_free( sig );
      const git_error *err = giterr_last();
      luaL_error( L, err->message );
   }
   git_signature_free( sig );
   return 0;
}

int luagi_remote_fetch( lua_State *L )
{
   return call_with_sig( L, git_remote_fetch );
}

int luagi_remote_connected( lua_State *L )
{ 
   git_remote **rem = checkremote( L );

   lua_pushboolean( L, git_remote_connected( *rem ) );
   return 1; 
}

int luagi_remote_stop( lua_State *L )
{ 
   git_remote **rem = checkremote( L );

   git_remote_stop( *rem );
   return 0; 
}

int luagi_remote_disconnect( lua_State *L )
{ 
   git_remote **rem = checkremote( L );

   git_remote_disconnect( *rem );
   return 0; 
}

int ligt_remote_free( lua_State *L )
{ 
   git_remote **rem = checkremote( L );

   git_remote_free( *rem );
   return 0;
}

int luagi_remote_check_cert( lua_State *L )
{ 
   git_remote **rem = checkremote( L );
   luaL_checktype( L, 2, LUA_TBOOLEAN );
   int check = lua_toboolean( L, 2 );
   git_remote_check_cert( *rem, check );
   return 0; 
}
int luagi_remote_set_transport( lua_State *L )
{ 
   git_remote **rem = checkremote( L );
   git_transport **transport = checktransport_at( L , 2 );
   if( git_remote_set_transport( *rem, *transport ) )
   {
      const git_error *err = giterr_last();
      luaL_error( L, err->message );
   }
   return 0; 
}

int luagi_remote_set_callbacks( lua_State *L )
{ 
   //TODO callbacks
   lua_pushnil( L ); 
   return 1; 
}

int luagi_remote_stats( lua_State *L )
{ 
   git_remote **rem = checkremote( L );

   const git_transfer_progress *progress = git_remote_stats( *rem );
   if( ! progress )
   {
      ERROR_PUSH( L )
   }

   luagi_push_transfer_stats( L, progress );
   return 1; 
}

int luagi_remote_autotag( lua_State *L )
{ 
   git_remote **rem = checkremote( L );
   git_remote_autotag_option_t option = git_remote_autotag( *rem );
   switch( option )
   {
      default:
      case GIT_REMOTE_DOWNLOAD_TAGS_AUTO:
         lua_pushstring( L, AUTO );
         break;
      case GIT_REMOTE_DOWNLOAD_TAGS_NONE:
         lua_pushstring( L, NONE );
         break;
      case GIT_REMOTE_DOWNLOAD_TAGS_ALL:
         lua_pushstring( L, ALL );
         break;
      
   }
   return 1; 
}

int luagi_remote_set_autotag( lua_State *L )
{ 
   git_remote **rem = checkremote( L );
   const char *option = luaL_optstring( L, 2, AUTO );
   git_remote_autotag_option_t opt;
   if( strncmp( option, NONE, strlen(NONE) ) == 0 )
   {
      opt = GIT_REMOTE_DOWNLOAD_TAGS_NONE;
   }
   else if( strncmp( option, ALL, strlen( ALL ) ) == 0 )
   {
      opt = GIT_REMOTE_DOWNLOAD_TAGS_ALL;
   }
   else 
   {
      opt = GIT_REMOTE_DOWNLOAD_TAGS_AUTO;
   }
   git_remote_set_autotag( *rem, opt );
   return 0; 
}

int luagi_remote_rename( lua_State *L )
{ 
   git_remote **rem = checkremote( L );
   const char *new_name = luaL_checkstring( L, 2 );
   git_strarray problems;
   if( git_remote_rename( &problems, *rem, new_name ) )
   {
      const git_error *err = giterr_last();
      luaL_error( L, err->message );
   }

   for( unsigned int i = 0; i < problems.count; i++ )
   {
      lua_pushstring( L, problems.strings[i] );
   }
   git_strarray_free( &problems );
   unsigned int count = problems.count;
   return count; 
}

int luagi_remote_update_fetch_head( lua_State *L )
{ 
   git_remote **rem = checkremote( L );
   lua_pushboolean( L, git_remote_update_fetchhead( *rem ) );
   return 1; 
}
int luagi_remote_set_update_fetch_head( lua_State *L )
{ 
   git_remote **rem = checkremote( L );
   luaL_checktype( L, 2, LUA_TBOOLEAN );
   int v = lua_toboolean( L, 2 );
   git_remote_set_update_fetchhead( *rem, v );
   return 0; 
}
int luagi_remote_update_tips( lua_State *L )
{ 
   return call_with_sig( L, git_remote_update_tips );
}
int luagi_remote_is_valid_name( lua_State *L )
{ 
   const char *name = luaL_checkstring( L, 1 );
   lua_pushboolean( L, git_remote_is_valid_name( name ) );
   return 1; 
}
int luagi_remote_valid_url( lua_State *L )
{ 
   const char *url = luaL_checkstring( L, 1 );
   lua_pushboolean( L, git_remote_valid_url( url ) );
   return 1; 
}
int luagi_remote_supported_url( lua_State *L )
{ 
   const char *url = luaL_checkstring( L, 1 );
   lua_pushboolean( L, git_remote_supported_url( url ) );
   return 1; 
}
