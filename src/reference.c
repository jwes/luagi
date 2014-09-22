#include "reference.h"
#include <string.h>
#include <git2/refs.h>
#include <git2/oid.h>
#include <git2/signature.h>
#include <git2/strarray.h>

#include "luagi.h"
#include "oid.h"
#include "object.h"
#include "types.h"

#define bufsize 1024
int luagi_reference_lookup( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *name = luaL_checkstring( L, 2 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference *) );
   if( git_reference_lookup( out, *repo, name ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_REFERENCE_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_reference_name_to_id( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *name = luaL_checkstring( L, 2 );

   git_oid out;
   if(git_reference_name_to_id( &out, *repo, name ) )
   {
      ERROR_PUSH( L )
   }
   return luagi_push_oid( L, &out );
}
   
int luagi_reference_dwim( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *shorthand = luaL_checkstring( L, 2 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference * ) );

   if( git_reference_dwim( out, *repo, shorthand ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_REFERENCE_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_reference_symbolic_create_matching( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *name = luaL_checkstring( L, 2 );
   if( git_reference_is_valid_name( name ) == 0 )
   {
      luaL_error( L, "invalid name" );
      return 0;
   }
   const char *target = luaL_checkstring( L, 3 );
   const char *current_value = luaL_checkstring( L, 4 );
   git_signature sig;
   luaL_checktype( L, 5, LUA_TTABLE );
   table_to_signature( L, &sig, 5 );

   const char *log_message = luaL_checkstring( L, 6 );
   int force = lua_toboolean( L, 7 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference *) );
   if( git_reference_symbolic_create_matching( out, *repo, name, target, force, current_value, &sig, log_message ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_REFERENCE_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_reference_symbolic_create( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *name = luaL_checkstring( L, 2 );
   if( git_reference_is_valid_name( name ) == 0 )
   {
      luaL_error( L, "invalid name" );
      return 0;
   }
   const char *target = luaL_checkstring( L, 3 );
   git_signature sig;
   luaL_checktype( L, 4, LUA_TTABLE );
   table_to_signature( L, &sig, 4 );

   const char *log_message = luaL_checkstring( L, 5 );
   int force = lua_toboolean( L, 6 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference *) );
   if( git_reference_symbolic_create( out, *repo, name, target, force, &sig, log_message ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_REFERENCE_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_reference_create( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *name = luaL_checkstring( L, 2 );
   if( git_reference_is_valid_name( name ) == 0 )
   {
      luaL_error( L, "invalid name" );
      return 0;
   }
   git_oid oid;
   if( luagi_check_oid( &oid, L, 3 ) )
   {
      ERROR_ABORT( L )
   }

   git_signature sig;
   luaL_checktype( L, 4, LUA_TTABLE );
   table_to_signature( L, &sig, 4 );

   const char *log_message = luaL_checkstring( L, 5 );
   int force = lua_toboolean( L, 6 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference *) );
   if( git_reference_create( out, *repo, name, &oid, force, &sig, log_message ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_REFERENCE_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_reference_create_matching( lua_State *L )
{
    git_repository **repo = checkrepo( L, 1 );
   const char *name = luaL_checkstring( L, 2 );
   if( git_reference_is_valid_name( name ) == 0 )
   {
      luaL_error( L, "invalid name" );
      return 0;
   }
   git_oid oid;
   if( luagi_check_oid( &oid, L, 3 ) )
   {
      ERROR_ABORT( L )
   }
   git_oid curr_oid;
   if( luagi_check_oid( &curr_oid, L, 4 ) )
   {
      ERROR_ABORT( L )
   }

   git_signature sig;
   luaL_checktype( L, 5, LUA_TTABLE );
   table_to_signature( L, &sig, 5 );

   const char *log_message = luaL_checkstring( L, 6 );
   int force = lua_toboolean( L, 7 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference *) );
   if( git_reference_create_matching( out, *repo, name, &oid, force, &curr_oid, &sig, log_message ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_REFERENCE_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}


// reference methods
int luagi_reference_target( lua_State *L )
{
   git_reference **ref = check_ref_at( L, 1 );
   const git_oid *oid = git_reference_target( *ref );
   return luagi_push_oid( L, oid );
}

int luagi_reference_target_peel( lua_State *L )
{
   git_reference **ref = check_ref_at( L, 1 );
   const git_oid *oid = git_reference_target_peel( *ref );
   return luagi_push_oid( L, oid );
}

int luagi_reference_symbolic_target( lua_State *L )
{
   git_reference **ref = check_ref_at( L, 1 );
   lua_pushstring( L, git_reference_symbolic_target( *ref ) );
   return 1;
}

int luagi_reference_type( lua_State *L )
{
   git_reference **ref = check_ref_at( L, 1 );
   git_ref_t type = git_reference_type( *ref );
   if( type == GIT_REF_INVALID )
   {
      lua_pushstring( L, REF_INVALID ); 
   }
   else if ( type == GIT_REF_OID )
   {
      lua_pushstring( L, REF_OID );
   }
   else
   {
      lua_pushstring( L, REF_SYMBOLIC );
   }
   return 1;
}

int luagi_reference_name( lua_State *L )
{
   git_reference **ref = check_ref_at( L, 1 );
   lua_pushstring( L, git_reference_name( *ref ) );
   return 1;
}

int luagi_reference_resolve( lua_State *L )
{
   git_reference **ref = check_ref_at( L, 1 );
   git_reference **out = lua_newuserdata( L, sizeof( git_reference * ) );
   if( git_reference_resolve( out, *ref ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_REFERENCE_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_reference_owner( lua_State *L )
{
   git_reference **ref = check_ref_at( L, 1 );
   git_repository **owner = lua_newuserdata( L, sizeof( git_repository * ) );
   *owner = git_reference_owner( *ref );
   if( *owner == NULL )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, REPO_NAME );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_reference_symbolic_set_target( lua_State *L )
{
   git_reference **ref = check_ref_at( L, 1 );
   const char *target = luaL_checkstring( L, 2 );
   git_signature sig;
   if( table_to_signature( L, &sig, 3 ) )
   {
      ERROR_ABORT( L )
   }
   const char *log_message = luaL_checkstring( L, 4 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference * ) );
   if( git_reference_symbolic_set_target( out, *ref, target, &sig, log_message ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_REFERENCE_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_reference_set_target( lua_State *L )
{
   git_reference **ref = check_ref_at( L, 1 );
   git_oid oid;
   if( luagi_check_oid( &oid, L, 2 ) )
   {
      ERROR_ABORT( L )
   }
   git_signature sig;
   if( table_to_signature( L, &sig, 3 ) )
   {
      ERROR_ABORT( L )
   }
   const char *log_message = luaL_checkstring( L, 4 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference * ) );
   if( git_reference_set_target( out, *ref, &oid, &sig, log_message ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_REFERENCE_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_reference_rename( lua_State *L )
{
   git_reference **ref = check_ref_at( L, 1 );
   const char *name = luaL_checkstring( L, 2 );
   git_signature sig;
   if( table_to_signature( L, &sig, 3 ) )
   {
      ERROR_ABORT( L )
   }
   const char *log_message = luaL_checkstring( L, 4 );
   int force = lua_toboolean( L, 5 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference * ) );
   if( git_reference_rename( out, *ref, name, force, &sig, log_message ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_REFERENCE_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_reference_delete( lua_State *L )
{
   git_reference **ref = check_ref_at( L, 1 );
   if( git_reference_delete( *ref ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_reference_remove( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *name = luaL_checkstring( L, 2 );

   if( git_reference_remove( *repo, name ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_reference_list( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_strarray array;
   if( git_reference_list( &array, *repo ) )
   {
      ERROR_PUSH( L )
   }
   luagi_lua_list_from_string( L, &array );
   git_strarray_free( &array );
   return 1;
}

static int ref_callback( git_reference *ref, void *payload )
{
   luagi_foreach_t *p = payload;

   git_reference **out = lua_newuserdata( p->L, sizeof( git_reference *));
   *out = ref; 
   if( lua_pcall( p->L, 1, 1, 0 ) != LUA_OK )
   {
      luaL_error( p->L, "can not call ref callback function" );
   }
   int ret = luaL_checkinteger( p->L, -1 );
   lua_pop( p->L, 1 );
   return ret;
}

static int name_callback( const char *name, void *payload )
{
   luagi_foreach_t *p = payload;
   lua_pushstring( p->L, name );
   if( lua_pcall( p->L, 1, 1, 0 ) != LUA_OK )
   {
      luaL_error( p->L, "can not call ref callback function" );
   }
   int ret = luaL_checkinteger( p->L, -1 );
   lua_pop( p->L, 1 );
   return ret;
}

int luagi_reference_foreach( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );
   p->L = L;
   p->callback_pos = 2;

   int just_names = lua_toboolean( L, 3 );
   
   if( just_names )
   {
      if( git_reference_foreach_name( *repo, name_callback, p ))
      {
         ERROR_ABORT( L )
      }
   }
   else
   {
      if( git_reference_foreach( *repo, ref_callback, p ))
      {
         ERROR_ABORT( L )
      }
   }
   free( p );
   return 0;   
}

static int generic_is_func( lua_State *L, int (*func)(const git_reference *ref ) )
{
   git_reference **ref = check_ref_at( L, 1 );
   lua_pushboolean( L, func( *ref ) );
   return 1;
}

int luagi_reference_is_branch( lua_State *L )
{
   return generic_is_func( L, git_reference_is_branch );
}

int luagi_reference_is_remote( lua_State *L )
{
   return generic_is_func( L, git_reference_is_remote );
}

int luagi_reference_is_tag( lua_State *L )
{
   return generic_is_func( L, git_reference_is_tag );
}

int luagi_reference_is_note( lua_State *L )
{
   return generic_is_func( L, git_reference_is_note );
}

int luagi_reference_peel( lua_State *L )
{
   git_reference **ref = check_ref_at( L, 1 );
   git_otype type = luagi_otype_from_string( luaL_checkstring( L, 2 ) );

   git_object **out = lua_newuserdata( L, sizeof( git_object *) );

   if( git_reference_peel( out, *ref, type ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_OBJECT_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_reference_shorthand( lua_State *L )
{
   git_reference **ref = check_ref_at( L, 1 );
   lua_pushstring( L, git_reference_shorthand( *ref ) );
   return 1;
}

int luagi_reference_free( lua_State *L )
{
   git_reference **ref = check_ref_at( L, 1 );
   git_reference_free( *ref );
   return 0;
}

static int reference_iter( lua_State *L )
{
   git_reference_iterator **iter = checkreferenceiter_at( L, lua_upvalueindex( 1 ) );   

   git_reference **out = lua_newuserdata( L, sizeof( git_reference * ) );
   int ret =  git_reference_next( out, *iter );
   if( ret == GIT_ITEROVER )
   {
      return 0;
   }
   else if( iter != 0 )
   {
      ERROR_ABORT( L )
      return 0;
   }

   luaL_getmetatable( L, LUAGI_REFERENCE_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

// normal from glob and just names
int luagi_reference_iterator( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *glob = luaL_optstring( L, 2, NULL );

   git_reference_iterator **iter = lua_newuserdata( L, sizeof( git_reference_iterator * ) );
   int ret = 0;
   if( glob == NULL )
   {
      ret = git_reference_iterator_new( iter, *repo ); 
   } 
   else
   {
      ret = git_reference_iterator_glob_new ( iter, *repo, glob );
   }

   if( ret )
   {
      ERROR_ABORT( L )
      return 0;
   }
     

   luaL_getmetatable(L, LUAGI_REFERENCE_ITER_FUNCS);
   lua_setmetatable(L, -2);

   lua_pushcclosure( L, reference_iter, -2 );
   return 1; 
}

int luagi_reference_iterator_free( lua_State *L )
{
   git_reference_iterator **iter = checkreferenceiter_at( L, 1 );
   git_reference_iterator_free( *iter );
   return 0;
}

int luagi_reference_foreach_glob( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *glob = luaL_checkstring( L, 2 );

   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );
   p->L = L;
   p->callback_pos = 3;

   if( git_reference_foreach_glob( *repo, glob, name_callback, p ))
   {
      ERROR_ABORT( L )
   }
   free( p );
   return 0;   
}

   

int luagi_reference_has_log( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *refname = luaL_checkstring( L, 2 );

   lua_pushboolean( L, git_reference_has_log( *repo, refname ) );
   return 1;
}

int luagi_reference_ensure_log( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *refname = luaL_checkstring( L, 2 );

   if( git_reference_ensure_log( *repo, refname ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_reference_normalize_name( lua_State *L )
{
   const char *name = luaL_checkstring( L, 1 );
   //TODO flags
   int flags = GIT_REF_FORMAT_NORMAL;
   char buffer_out[ bufsize ];

   memset( buffer_out, 0, bufsize );
   if( git_reference_normalize_name( buffer_out, bufsize, name, flags ) != 0 )
   {
      ERROR_PUSH( L )
   }
   lua_pushstring( L, buffer_out );
   return 1;
}

int luagi_reference_is_valid_name( lua_State *L )
{
   const char *name = luaL_checkstring( L, 1 );
   lua_pushboolean( L, git_reference_is_valid_name( name ) );
   return 1;
}


