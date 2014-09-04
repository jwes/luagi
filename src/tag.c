#include <git2/tag.h>

#include "tag.h"
#include "luagi.h"
#include "oid.h"
#include "object.h"

int luagi_tag_lookup( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid id;
   luagi_check_oid( &id, L, 2 );

   git_tag **out = lua_newuserdata( L, sizeof( git_tag * ) ); 
   if( git_tag_lookup( out, *repo, &id ) ) {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_TAG_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_tag_lookup_prefix( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid id;
   luagi_check_oid( &id, L, 2 );
   int len = luaL_checkinteger( L, 3 );

   git_tag **out = lua_newuserdata( L, sizeof( git_tag * ) ); 
   if( git_tag_lookup_prefix( out, *repo, &id, len ) ) {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_TAG_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

   
int luagi_tag_create( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *tag_name = luaL_checkstring( L, 2 );
   git_object **obj = checkobject_at( L, 3 );
   git_signature sig;
   table_to_signature( L, &sig, 4 );
   const char *message = luaL_checkstring( L, 5 );
   int force = lua_toboolean( L, 6 );

   git_oid out;
   if( git_tag_create( &out, *repo, tag_name, *obj, &sig, message, force ) )
   {
      ERROR_PUSH( L )
   }
   return luagi_push_oid( L, &out );
}

int luagi_tag_annotation_create( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *tag_name = luaL_checkstring( L, 2 );
   git_object **obj = checkobject_at( L, 3 );
   git_signature sig;
   table_to_signature( L, &sig, 4 );
   const char *message = luaL_checkstring( L, 5 );

   git_oid out;
   if( git_tag_annotation_create( &out, *repo, tag_name, *obj, &sig, message  ) )
   {
      ERROR_PUSH( L )
   }
   return luagi_push_oid( L, &out );
}

int luagi_tag_create_frombuffer( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *buffer = luaL_checkstring( L, 2 );
   int force = lua_toboolean( L, 3 );

   git_oid out;
   if( git_tag_create_frombuffer( &out, *repo, buffer, force ) )
   {
      ERROR_PUSH( L )
   }

   return luagi_push_oid( L, &out );
}

int luagi_tag_create_lightweight( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *tag_name = luaL_checkstring( L, 2 );
   git_object **target = checkobject_at( L, 3 );
   int force = lua_toboolean( L, 4 );

   git_oid oid;
   if( git_tag_create_lightweight( &oid, *repo, tag_name, *target, force ) )
   {
      ERROR_PUSH( L )
   }
   
   return luagi_push_oid( L, &oid );
}

int luagi_tag_delete( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *tag_name = luaL_checkstring( L, 2 );

   if( git_tag_delete( *repo, tag_name ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_tag_list( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_strarray *data = lua_newuserdata( L, sizeof( git_strarray ) );
   if( git_tag_list( data, *repo ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_STRARRAY );
   lua_setmetatable( L, -2 );
   return 1;
}
   
int luagi_tag_list_match( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *pattern = luaL_checkstring( L, 2 );

   git_strarray *data = lua_newuserdata( L, sizeof( git_strarray ) );
   if( git_tag_list_match( data, pattern, *repo ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_STRARRAY );
   lua_setmetatable( L, -2 );
   return 1;
}

static int foreach_cb( const char *name, git_oid *oid, void *payload )
{
   luagi_foreach_t *p = payload; 
   lua_pushvalue( p->L, p->callback_pos );

   lua_pushstring( p->L, name );
   luagi_push_oid( p->L, oid );

   if( lua_pcall( p->L, 2, 1, 0 ) )
   {
      luaL_error( p->L, "call to tag foreach callback failed" );
      return 0;
   }
   int ret = luaL_checkinteger( p->L, -1 );
   lua_pop( p->L, 1 );
   return ret;
}

int luagi_tag_foreach( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );

   if( git_tag_foreach( *repo, foreach_cb, p ) )
   {
      ERROR_ABORT( L )
   }
   free( p );
   return 0;
}


int luagi_tag_free( lua_State *L )
{
   git_tag **tag = checktag_at( L, 1 );
   git_tag_free( *tag );
   return 0;
}

int luagi_tag_id( lua_State *L )
{
   git_tag **tag = checktag_at( L, 1 );

   const git_oid *oid = git_tag_id( *tag );
   return luagi_push_oid( L, oid );
}

int luagi_tag_owner( lua_State *L )
{
   git_tag **tag = checktag_at( L, 1 );
   git_repository **repo = lua_newuserdata( L, sizeof( git_repository * ) );

   *repo = git_tag_owner( *tag );
   luaL_getmetatable( L, REPO_NAME );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_tag_target( lua_State *L )
{
   git_tag **tag = checktag_at( L, 1 );
   git_object **target = lua_newuserdata( L, sizeof( git_object * ) );

   if( git_tag_target( target, *tag ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_OBJECT_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

   
int luagi_tag_target_id( lua_State *L )
{
   git_tag **tag = checktag_at( L, 1 );

   const git_oid *oid = git_tag_target_id( *tag );
   return luagi_push_oid( L, oid );
}

int luagi_tag_target_type( lua_State *L )
{
   git_tag **tag = checktag_at( L, 1 );

   git_otype type = git_tag_target_type( *tag );
   lua_pushstring( L, luagi_otype_to_string( type ) );
   return 1;
}

int luagi_tag_name( lua_State *L )
{
   git_tag **tag = checktag_at( L, 1 );
   lua_pushstring( L, git_tag_name( *tag ) );
   return 1;
}

int luagi_tag_tagger( lua_State *L )
{
   git_tag **tag = checktag_at( L, 1 );
   const git_signature *sig = git_tag_tagger( *tag );
   signature_to_table( L, sig );
   return 1;
}

int luagi_tag_message( lua_State *L )
{
   git_tag **tag = checktag_at( L, 1 );

   lua_pushstring( L, git_tag_message( *tag ) );
   return 1;
}

int luagi_tag_peel( lua_State *L )
{
   git_tag **tag = checktag_at( L, 1 );
   git_object **object = lua_newuserdata( L, sizeof( git_object * ) );

   if( git_tag_peel( object, *tag ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_OBJECT_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}
