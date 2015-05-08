#include "reference.h"

#include <git2/oid.h>
#include <git2/refs.h>
#include <git2/signature.h>
#include <git2/strarray.h>
#include <string.h>

#include "blob.h"
#include "commit.h"
#include "ltk.h"
#include "luagi.h"
#include "object.h"
#include "oid.h"
#include "tag.h"
#include "tree.h"
#include "types.h"

#define bufsize 1024
int luagi_reference_lookup( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *name = luaL_checkstring( L, 2 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference *) );
   if( git_reference_lookup( out, *repo, name ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_REFERENCE_FUNCS );
   return 1;
}

int luagi_reference_name_to_id( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *name = luaL_checkstring( L, 2 );

   git_oid out;
   if(git_reference_name_to_id( &out, *repo, name ) )
   {
      return ltk_push_git_error( L );
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
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_REFERENCE_FUNCS );
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
   git_signature *sig;
   luaL_checktype( L, 5, LUA_TTABLE );
   table_to_signature( L, &sig, 5 );

   const char *log_message = luaL_checkstring( L, 6 );
   int force = lua_toboolean( L, 7 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference *) );
   int ret = git_reference_symbolic_create_matching( out, *repo, name, target, force, current_value, sig, log_message );
   git_signature_free( sig );
   if( ret )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_REFERENCE_FUNCS );
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
   git_signature *sig;
   luaL_checktype( L, 4, LUA_TTABLE );
   table_to_signature( L, &sig, 4 );

   const char *log_message = luaL_checkstring( L, 5 );
   int force = lua_toboolean( L, 6 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference *) );
   int ret = git_reference_symbolic_create( out, *repo, name, target, force, sig, log_message );
   git_signature_free( sig );
   if( ret )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_REFERENCE_FUNCS );
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
      ltk_error_abort( L );
   }

   git_signature *sig;
   luaL_checktype( L, 4, LUA_TTABLE );
   table_to_signature( L, &sig, 4 );

   const char *log_message = luaL_checkstring( L, 5 );
   int force = lua_toboolean( L, 6 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference *) );
   int ret = git_reference_create( out, *repo, name, &oid, force, sig, log_message );
   git_signature_free( sig );
   if( ret )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_REFERENCE_FUNCS );
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
      ltk_error_abort( L );
   }
   git_oid curr_oid;
   if( luagi_check_oid( &curr_oid, L, 4 ) )
   {
      ltk_error_abort( L );
   }

   git_signature *sig;
   luaL_checktype( L, 5, LUA_TTABLE );
   table_to_signature( L, &sig, 5 );

   const char *log_message = luaL_checkstring( L, 6 );
   int force = lua_toboolean( L, 7 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference *) );
   int ret = git_reference_create_matching( out, *repo, name, &oid, force, &curr_oid, sig, log_message );
   git_signature_free( sig );
   if( ret )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_REFERENCE_FUNCS );
   return 1;
}


// reference methods
int luagi_reference_gen_target( lua_State *L, const char *tablename )
{
   git_reference **ref = luaL_checkudata( L, 1, tablename );
   const git_oid *oid = git_reference_target( *ref );
   return luagi_push_oid( L, oid );
}

int luagi_reference_gen_target_peel( lua_State *L, const char *tablename )
{
   git_reference **ref = luaL_checkudata( L, 1, tablename );
   const git_oid *oid = git_reference_target_peel( *ref );
   return luagi_push_oid( L, oid );
}

int luagi_reference_gen_symbolic_target( lua_State *L, const char *tablename )
{
   git_reference **ref = luaL_checkudata( L, 1, tablename );
   lua_pushstring( L, git_reference_symbolic_target( *ref ) );
   return 1;
}

int luagi_reference_gen_type( lua_State *L, const char *tablename )
{
   git_reference **ref = luaL_checkudata( L, 1, tablename );
   git_ref_t type = git_reference_type( *ref );
   if( type == GIT_REF_INVALID )
   {
      lua_pushstring( L, INVALID ); 
   }
   else if ( type == GIT_REF_OID )
   {
      lua_pushstring( L, OID );
   }
   else
   {
      lua_pushstring( L, SYMBOLIC );
   }
   return 1;
}

int luagi_reference_gen_name( lua_State *L, const char *tablename )
{
   git_reference **ref = luaL_checkudata( L, 1, tablename );
   lua_pushstring( L, git_reference_name( *ref ) );
   return 1;
}

int luagi_reference_gen_resolve( lua_State *L, const char *tablename )
{
   git_reference **ref = luaL_checkudata( L, 1, tablename );
   git_reference **out = lua_newuserdata( L, sizeof( git_reference * ) );
   if( git_reference_resolve( out, *ref ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_REFERENCE_FUNCS );
   return 1;
}

int luagi_reference_gen_symbolic_set_target( lua_State *L, const char *tablename )
{
   git_reference **ref = luaL_checkudata( L, 1, tablename );
   const char *target = luaL_checkstring( L, 2 );
   git_signature *sig;
   if( table_to_signature( L, &sig, 3 ) )
   {
      ltk_error_abort( L );
   }
   const char *log_message = luaL_checkstring( L, 4 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference * ) );
   int ret =  git_reference_symbolic_set_target( out, *ref, target, sig, log_message );
   git_signature_free( sig );
   if( ret )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_REFERENCE_FUNCS );
   return 1;
}

int luagi_reference_gen_set_target( lua_State *L, const char *tablename )
{
   git_reference **ref = luaL_checkudata( L, 1, tablename );
   git_oid oid;
   if( luagi_check_oid( &oid, L, 2 ) )
   {
      ltk_error_abort( L );
   }
   luaL_checktype( L, 3, LUA_TTABLE );
   git_signature *sig;
   if( table_to_signature( L, &sig, 3 ) )
   {
      ltk_error_abort( L );
   }
   const char *log_message = luaL_checkstring( L, 4 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference * ) );
   int ret = git_reference_set_target( out, *ref, &oid, sig, log_message );
   git_signature_free( sig );
   if( ret )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_REFERENCE_FUNCS );
   return 1;
}

int luagi_reference_gen_rename( lua_State *L, const char *tablename )
{
   git_reference **ref = luaL_checkudata( L, 1, tablename );
   const char *name = luaL_checkstring( L, 2 );
   luaL_checktype( L, 3, LUA_TTABLE );
   git_signature *sig;
   if( table_to_signature( L, &sig, 3 ) )
   {
      ltk_error_abort( L );
   }
   const char *log_message = luaL_checkstring( L, 4 );
   int force = lua_toboolean( L, 5 );

   git_reference **out = lua_newuserdata( L, sizeof( git_reference * ) );
   int ret = git_reference_rename( out, *ref, name, force, sig, log_message );
   git_signature_free( sig );
   if( ret )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_REFERENCE_FUNCS );
   return 1;
}

int luagi_reference_gen_delete( lua_State *L, const char *tablename )
{
   git_reference **ref = luaL_checkudata( L, 1, tablename );
   if( git_reference_delete( *ref ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_reference_remove( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *name = luaL_checkstring( L, 2 );

   if( git_reference_remove( *repo, name ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_reference_list( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_strarray array;
   if( git_reference_list( &array, *repo ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_push_strarray( L, array );
   git_strarray_free( &array );
   return 1;
}

static int ref_callback( git_reference *ref, void *payload )
{
   luagi_foreach_t *p = payload;

   lua_pushvalue( p->L, p->callback_pos );
   git_reference **out = lua_newuserdata( p->L, sizeof( git_reference *));
   *out = ref; 
   luaL_getmetatable( p->L, LUAGI_REFERENCE_FUNCS );
   lua_setmetatable( p->L, -2 );
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
   lua_pushvalue( p->L, p->callback_pos );
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
   luaL_checktype( L, 2, LUA_TFUNCTION );

   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );
   p->L = L;
   p->callback_pos = 2;

   int just_names = lua_toboolean( L, 3 );
   
   if( just_names )
   {
      if( git_reference_foreach_name( *repo, name_callback, p ))
      {
         ltk_error_abort( L );
      }
   }
   else
   {
      if( git_reference_foreach( *repo, ref_callback, p ))
      {
         ltk_error_abort( L );
      }
   }
   free( p );
   return 0;   
}

static int generic_is_func( lua_State *L, const char *tablename, int (*func)(const git_reference *ref ) )
{
   git_reference **ref = luaL_checkudata( L, 1, tablename );
   lua_pushboolean( L, func( *ref ) );
   return 1;
}

int luagi_reference_gen_is_branch( lua_State *L, const char *tablename )
{
   return generic_is_func( L, tablename, git_reference_is_branch );
}

int luagi_reference_gen_is_remote( lua_State *L, const char *tablename )
{
   return generic_is_func( L, tablename, git_reference_is_remote );
}

int luagi_reference_gen_is_tag( lua_State *L, const char *tablename )
{
   return generic_is_func( L, tablename, git_reference_is_tag );
}

int luagi_reference_gen_is_note( lua_State *L, const char *tablename )
{
   return generic_is_func( L, tablename, git_reference_is_note );
}

int luagi_reference_gen_peel( lua_State *L, const char *tablename )
{
   git_reference **ref = luaL_checkudata( L, 1, tablename );
   git_otype type = luagi_otype_from_string( luaL_checkstring( L, 2 ) );

   git_object **out = lua_newuserdata( L, sizeof( git_object *) );

   if( git_reference_peel( out, *ref, type ) )
   {
      return ltk_push_git_error( L );
   }
   switch( type)
   {
   case GIT_OBJ_COMMIT:
      luaL_getmetatable( L, LUAGI_COMMIT_FUNCS );
      break;
   case GIT_OBJ_TREE:
      luaL_getmetatable( L, LUAGI_TREE_FUNCS );
      break;
   case GIT_OBJ_BLOB:
      luaL_getmetatable( L, LUAGI_BLOB_FUNCS );
      break;
   case GIT_OBJ_TAG:
      luaL_getmetatable( L, LUAGI_TAG_FUNCS );
      break;
   default:
      luaL_getmetatable( L, LUAGI_OBJECT_FUNCS );
      break;
   }
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_reference_gen_shorthand( lua_State *L, const char *tablename )
{
   git_reference **ref = luaL_checkudata( L, 1, tablename );
   lua_pushstring( L, git_reference_shorthand( *ref ) );
   return 1;
}

int luagi_reference_gen_free( lua_State *L, const char *tablename )
{
   git_reference **ref = luaL_checkudata( L, 1, tablename );
   git_reference_free( *ref );
   return 0;
}

static int reference_iter( lua_State *L )
{
   git_reference_iterator **iter = checkreferenceiter_at( L, lua_upvalueindex( 1 ) );   

   git_reference **out = lua_newuserdata( L, sizeof( git_reference * ) );
   int ret = git_reference_next( out, *iter );
   if( ret == GIT_ITEROVER )
   {
      return 0;
   }
   else if( ret != 0 )
   {
      ltk_error_abort( L );
      return 0;
   }

   ltk_setmetatable( L, LUAGI_REFERENCE_FUNCS );
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
      ltk_error_abort( L );
      return 0;
   }

   luaL_getmetatable(L, LUAGI_REFERENCE_ITER_FUNCS);
   lua_setmetatable(L, -2);

   lua_pushcclosure( L, reference_iter, 1 );
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
   luaL_checktype( L, 3, LUA_TFUNCTION );
   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );
   p->L = L;
   p->callback_pos = 3;

   int ret = git_reference_foreach_glob( *repo, glob, name_callback, p );
   free( p );
   if( ret )
   {
      ltk_error_abort( L );
   }
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
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_reference_normalize_name( lua_State *L )
{
   const char *name = luaL_checkstring( L, 1 );

   int flags = GIT_REF_FORMAT_NORMAL;
   if( lua_type( L, 2 ) == LUA_TTABLE)
   {

      add_flag( flags, L, 2, ALLOW_ONELEVEL, GIT_REF_FORMAT_ALLOW_ONELEVEL );
      add_flag( flags, L, 2, REFSPEC_PATTERN, GIT_REF_FORMAT_REFSPEC_PATTERN );
      add_flag( flags, L, 2, REFSPEC_SHORTHAND, GIT_REF_FORMAT_REFSPEC_SHORTHAND );
   }

   char buffer_out[ bufsize ];

   memset( buffer_out, 0, bufsize );
   if( git_reference_normalize_name( buffer_out, bufsize, name, flags ) != 0 )
   {
      return ltk_push_git_error( L );
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


int luagi_reference_target( lua_State *L )
{
   return luagi_reference_gen_target( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_target_peel( lua_State *L )
{
   return luagi_reference_gen_target_peel( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_symbolic_target( lua_State *L )
{
   return luagi_reference_gen_symbolic_target( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_type( lua_State *L )
{
   return luagi_reference_gen_type( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_name( lua_State *L )
{
   return luagi_reference_gen_name( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_resolve( lua_State *L )
{
   return luagi_reference_gen_resolve( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_symbolic_set_target( lua_State *L )
{
   return luagi_reference_gen_symbolic_set_target( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_set_target( lua_State *L )
{
   return luagi_reference_gen_set_target( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_rename( lua_State *L )
{
   return luagi_reference_gen_rename( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_delete( lua_State *L )
{
   return luagi_reference_gen_delete( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_is_branch( lua_State *L )
{
   return luagi_reference_gen_is_branch( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_is_remote( lua_State *L )
{
   return luagi_reference_gen_is_remote( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_is_tag( lua_State *L )
{
   return luagi_reference_gen_is_tag( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_is_note( lua_State *L )
{
   return luagi_reference_gen_is_note( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_peel( lua_State *L )
{
   return luagi_reference_gen_peel( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_shorthand( lua_State *L )
{
   return luagi_reference_gen_shorthand( L, LUAGI_REFERENCE_FUNCS );
}

int luagi_reference_free( lua_State *L )
{
   return luagi_reference_gen_free( L, LUAGI_REFERENCE_FUNCS );
}

