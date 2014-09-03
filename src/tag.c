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

int luagi_tag_create_lightweight( lua_State *L );
int luagi_tag_delete( lua_State *L );

int luagi_tag_list( lua_State *L );
int luagi_tag_list_match( lua_State *L );
int luagi_tag_foreach( lua_State *L );


int luagi_tag_free( lua_State *L );
int luagi_tag_id( lua_State *L );
int luagi_tag_owner( lua_State *L );
int luagi_tag_target( lua_State *L );
int luagi_tag_target_id( lua_State *L );
int luagi_tag_target_type( lua_State *L );
int luagi_tag_name( lua_State *L );
int luagi_tag_tagger( lua_State *L );
int luagi_tag_message( lua_State *L );
int luagi_tag_peel( lua_State *L );
