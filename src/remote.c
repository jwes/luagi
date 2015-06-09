#include "remote.h"

#include <git2/errors.h>
#include <git2/remote.h>
#include <git2/signature.h>
#include <string.h>

#include "ltk.h"
#include "luagi.h"
#include "oid.h"

typedef struct
{
   lua_State *L;
   int table_pos;
} remote_callback_t;

int luagi_remote_list( lua_State *L )
{ 
   git_repository **repo = checkrepo( L, 1 );

   git_strarray array;
   if( git_remote_list( &array, *repo ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_push_strarray( L, array );
   git_strarray_free( &array );
   return 1;
}

int luagi_remote_lookup( lua_State *L )
{ 
   git_repository **repo = checkrepo( L, 1 );
   const char *name = luaL_checkstring( L, 2 );

   git_remote **out = (git_remote **) lua_newuserdata( L, sizeof( git_remote *) );

   if( git_remote_lookup( out, *repo, name ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_REMOTE_FUNCS );
   return 1; 
}

int luagi_remote_create( lua_State *L )
{ 
   git_repository **repo = checkrepo( L, 1 );
   luaL_checktype( L, 2, LUA_TTABLE );

   lua_getfield( L, 2, URL );
   const char *url = luaL_checkstring( L, -1 );

   lua_getfield( L, 2, NAME );
   const char *name = luaL_optstring( L, -1, NULL );

   lua_getfield( L, 2, FETCH );
   const char *fetch = luaL_optstring( L, -1, NULL );

   git_remote **out = (git_remote **) lua_newuserdata( L, sizeof( git_remote *) );

   int ret = 0;
   if( name && !fetch )
      ret = git_remote_create_anonymous( out, *repo, url, fetch );
   else if( name && fetch )
      ret = git_remote_create_with_fetchspec( out, *repo, name, url, fetch );
   else
      ret = git_remote_create( out, *repo, name, url );


   if( ret )
   {
      return ltk_push_git_error( L );
   }
   
   ltk_setmetatable( L, LUAGI_REMOTE_FUNCS );
   return 1; 
}

int luagi_remote_save( lua_State *L )
{ 
   git_remote** rem = checkremote( L );
   if( git_remote_save( *rem ) )
   {
      ltk_error_abort( L );
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
   ltk_push_strarray( L, array );
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

   git_strarray array = ltk_check_strarray( L, 2 );

   int ret = func( *rem, &array);
   git_strarray_free( &array );
   if( ret ) 
   {
      ltk_error_abort( L );
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
   size_t size = git_remote_refspec_count( *rem );
   if( n <= 0 || n > size )
   {
      return ltk_push_error_msg( L, "index out of bounds" );
   }
   n--;

   const git_refspec *spec = git_remote_get_refspec( *rem, n );
   if( spec == NULL )
   {
      return ltk_push_git_error( L );
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
      ltk_error_abort( L );
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
      return ltk_push_git_error( L );
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
   luaL_checktype( L, 2, LUA_TTABLE ); 
   git_strarray array = ltk_check_strarray( L, 2 );

   int ret = git_remote_download( *rem, &array );
   git_strarray_free( &array );
   if( ret )
   {
      ltk_error_abort( L );
   }
   return 0;
}

static int luagi_push_init_options( lua_State *L __attribute__(( unused )), int index __attribute__(( unused )), git_push_options *opts )
{
   int ret = git_push_init_options( opts, GIT_PUSH_OPTIONS_VERSION );

   lua_getfield( L, index, THREADS );
   if( lua_type( L, -1 ) == LUA_TNUMBER )
   {
      opts->pb_parallelism = luaL_checkinteger( L, -1 );
   }
   return ret;
}

int luagi_remote_upload( lua_State *L )
{ 
   git_remote **rem = checkremote( L );
   luaL_checktype( L, 2, LUA_TTABLE ); 
   git_strarray array = ltk_check_strarray( L, 2 );

   luaL_checktype( L, 3, LUA_TTABLE );
   git_push_options opts;
   luagi_push_init_options( L, 3, &opts );

   int ret = git_remote_upload( *rem, &array, &opts );
   git_strarray_free( &array );
   if( ret )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_remote_prune( lua_State *L )
{
   git_remote **rem = checkremote( L );
   if( git_remote_prune( *rem ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_remote_prune_refs( lua_State *L )
{
   git_remote **rem = checkremote( L );
   lua_pushinteger( L, git_remote_prune_refs( *rem ));
   return 1;
}

int luagi_remote_delete( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *name = luaL_checkstring( L, 2 );

   if( git_remote_delete( *repo, name ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_remote_push( lua_State *L )
{
   git_remote **rem = checkremote( L );
   luaL_checktype( L, 2, LUA_TTABLE ); 
   git_strarray array = ltk_check_strarray( L, 2 );

   luaL_checktype( L, 3, LUA_TTABLE );
   git_push_options opts;
   luagi_push_init_options( L, 3, &opts );

   luaL_checktype( L, 4, LUA_TTABLE );
   git_signature *sig;
   table_to_signature( L, &sig, 4 );

   const char *msg = luaL_optstring( L, 5, NULL );

   int ret = git_remote_push( *rem, &array, &opts, sig, msg );
   git_strarray_free( &array );
   git_signature_free( sig );
   if( ret )
   {
      ltk_error_abort( L );
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
      ltk_error_abort( L );
   }
   git_signature_free( sig );
   return 0;
}

int luagi_remote_fetch( lua_State *L )
{
   git_remote **rem = checkremote( L );
   luaL_checktype( L, 2, LUA_TTABLE ); 
   git_strarray refspecs = ltk_check_strarray( L, 2 );

   luaL_checktype( L, 3, LUA_TTABLE );
   git_signature *sig;

   table_to_signature( L, &sig, 3 );

   const char* reflog_message = luaL_optstring( L, 4, NULL );

   int ret =  git_remote_fetch( *rem, &refspecs, sig, reflog_message );

   git_strarray_free( &refspecs );
   git_signature_free( sig );
   if( ret )
   {
      ltk_error_abort( L );
   }
   return 0;
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

int luagi_remote_free( lua_State *L )
{ 
   git_remote **rem = checkremote( L );

   const git_remote_callbacks *callbacks = git_remote_get_callbacks( *rem );
   if( callbacks && callbacks->payload )
   {
      remote_callback_t *c = callbacks->payload;
      free( c );
   }

   git_remote_free( *rem );
   return 0;
}
/* TODO callbacks
int luagi_remote_check_cert( lua_State *L )
{ 
   git_remote **rem = checkremote( L );
   luaL_checktype( L, 2, LUA_TBOOLEAN );
   int check = lua_toboolean( L, 2 );
   git_remote_check_cert( *rem, check );
   return 0; 
}
*/
// callback functions
static int completion(git_remote_completion_type type, void *data)
{
   remote_callback_t *f = data;
 
   lua_getfield( f->L, f->table_pos, COMPLETION );
   if( lua_type( f->L, f->table_pos + 1) != LUA_TFUNCTION )
   {
      return 1;
   }
   lua_pushvalue( f->L, f->table_pos + 1 );
   lua_pushnumber( f->L, type );

   ltk_debug_stack( f->L );

   if( lua_pcall( f->L, 1, 0, 0 ) != LUA_OK )
   {
      ltk_dump_stack( f->L );
      return 1;
   }

   return 0;

}
static int cred_acquire_cb(
	git_cred **cred __attribute__((unused)),
	const char *url __attribute__((unused)),
	const char *username_from_url __attribute__((unused)),
	unsigned int allowed_types __attribute__((unused)),
	void *payload __attribute__((unused)))
{
 // TODO implement
   return 0;
}

static int transport_message_cb(const char *str, int len, void *data)
{
   remote_callback_t *f = data;

   lua_getfield( f->L, f->table_pos, SIDEBAND_PROGRESS );
   if( lua_type( f->L, f->table_pos + 1 ) != LUA_TFUNCTION )
   {
      return 1;
   }
   lua_pushvalue( f->L, f->table_pos + 1 );
   lua_pushlstring( f->L, str, len );
 
   ltk_debug_stack( f->L );

   if( lua_pcall( f->L, 1, 0, 0 ) != LUA_OK )
   {
      ltk_dump_stack( f->L );
      return 1;
   }
   return 0;
}
static int progress_cb( const git_transfer_progress *progress, void *payload )
{
   remote_callback_t *f = payload;

   lua_getfield( f->L, f->table_pos, TRANSFER_PROGRESS );
   if( lua_type( f->L, f->table_pos + 1 ) != LUA_TFUNCTION )
   {
      return 1;
   }
   lua_pushvalue( f->L, f->table_pos + 1 );
   lua_pushnumber( f->L, progress->total_objects );
   lua_pushnumber( f->L, progress->indexed_objects );
   lua_pushnumber( f->L, progress->received_objects );
   lua_pushnumber( f->L, progress->local_objects );
   lua_pushnumber( f->L, progress->total_deltas );
   lua_pushnumber( f->L, progress->indexed_deltas );
   lua_pushnumber( f->L, progress->received_bytes );
 
   if( lua_pcall( f->L, 7, 0, 0 ) != LUA_OK )
   {
      ltk_dump_stack( f->L );
      return 1;
   }
   return 0;
}
static int update_tips(const char *refname, const git_oid *a, const git_oid *b, void *data)
{
   remote_callback_t *f = data;

   lua_getfield( f->L, f->table_pos, UPDATE_TIPS );
   if( lua_type( f->L, f->table_pos + 1 ) != LUA_TFUNCTION )
   {
      return 1;
   }
   lua_pushvalue( f->L, f->table_pos + 1 );
   lua_pushstring( f->L, refname );
   luagi_push_oid( f->L, a );
   luagi_push_oid( f->L, b );

   if( lua_pcall( f->L, 3, 0, 0 ) != LUA_OK )
   {
      ltk_dump_stack( f->L );
      return 1;
   }
   return 0;
}

int luagi_remote_set_callbacks( lua_State *L )
{ 
   git_remote **rem = checkremote( L );
   luaL_checktype( L, 2, LUA_TTABLE );
   
   remote_callback_t *c = NULL;
   const git_remote_callbacks *cbs = git_remote_get_callbacks( *rem );
   if( cbs && cbs->payload )
   {
      c = cbs->payload;  
   }
   else
   {
      c = malloc( sizeof( remote_callback_t ) );
   }
   c->L = L;
   c->table_pos = 2;

   git_remote_callbacks callbacks;
   git_remote_init_callbacks( &callbacks, GIT_REMOTE_CALLBACKS_VERSION );
   callbacks.sideband_progress = transport_message_cb;
   callbacks.completion = completion;
   callbacks.credentials = cred_acquire_cb;
   callbacks.transfer_progress = progress_cb;
   callbacks.update_tips = update_tips;
   callbacks.payload = c;

   int ret = git_remote_set_callbacks( *rem, &callbacks);
   if( ret )
   {
      free( c);
      ltk_error_abort( L );
   }
   return 0; 
}

int luagi_remote_stats( lua_State *L )
{ 
   git_remote **rem = checkremote( L );

   const git_transfer_progress *progress = git_remote_stats( *rem );
   if( ! progress )
   {
      return ltk_push_git_error( L );
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
   git_repository **repo = checkrepo( L, 1 );
   const char *name = luaL_checkstring( L, 2 );
   const char *new_name = luaL_checkstring( L, 3 );
   git_strarray problems;
   if( git_remote_rename( &problems, *repo, name, new_name ) )
   {
      ltk_error_abort( L );
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
