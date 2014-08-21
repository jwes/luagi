#include "object.h"
#include <git2/object.h>
#include <git2/oid.h>
#include "wien.h"
int luagi_object_lookup( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *idstr = luaL_checkstring( L, 2 );
   const char *typestr = luaL_checkstring( L, 3 );

   git_oid id;
   if( git_oid_fromstr( &id, idstr ) )
   {
      ERROR_PUSH( L )
   }

   git_otype type = git_object_string2type( typestr );
  
   git_object **out = lua_newuserdata( L, sizeof( git_object *) );
   if( git_object_lookup( out, *repo, &id, type ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable(L, LGIT_OBJECT_FUNCS );
   lua_setmetatable(L, -2);

   return 1;
}

int luagi_object_lookup_bypath( lua_State *L )
{
   git_object **obj = checkobject_at( L, 1 );
   const char *path = luaL_checkstring( L, 2 );
   const char *typestr = luaL_checkstring( L, 3 );

   git_otype type = git_object_string2type( typestr );
  
   git_object **out = lua_newuserdata( L, sizeof( git_object *) );
   if( git_object_lookup_bypath( out, *obj, path, type ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable(L, LGIT_OBJECT_FUNCS );
   lua_setmetatable(L, -2);

   return 1;
}

//use
int luagi_object_id( lua_State *L )
{
   git_object **obj = checkobject_at( L, 1 );

   const git_oid *oid = git_object_id( *obj );
   if( oid == NULL )
   {
      ERROR_PUSH( L )
   }

   char buf[ GIT_OID_HEXSZ + 1 ];
   lua_pushstring( L, git_oid_tostr( buf, sizeof( buf ), oid ) );
   return 1;
}

int luagi_object_type( lua_State *L )
{
   git_object **obj = checkobject_at( L, 1 );

   git_otype type = git_object_type( *obj );
   lua_pushstring( L, git_object_type2string( type ) );

   return 1;
}

int luagi_object_owner( lua_State *L )
{
   git_object **obj = checkobject_at( L, 1 );
   git_repository **repo = lua_newuserdata( L, sizeof( git_repository *) );
   *repo = git_object_owner( *obj );
   if( *repo == NULL )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable(L, REPO_NAME );
   lua_setmetatable(L, -2);
   return 1;
}

int luagi_object_free( lua_State *L )
{

   git_object **obj = checkobject_at( L, 1 );
   git_object_free( *obj );
   return 0;   
}
