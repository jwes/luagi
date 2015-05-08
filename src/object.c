#include "object.h"

#include <git2/object.h>
#include <git2/oid.h>

#include "ltk.h"
#include "luagi.h"
#include "oid.h"

int luagi_object_lookup( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   git_oid id;
   if(luagi_check_oid( &id, L, 2 ) )
   {
      return ltk_push_git_error( L );
   }

   const char *typestr = luaL_checkstring( L, 3 );
   git_otype type = git_object_string2type( typestr );
  
   git_object **out = lua_newuserdata( L, sizeof( git_object *) );
   if( git_object_lookup( out, *repo, &id, type ) )
   {
      return ltk_push_git_error( L );
   }

   luaL_getmetatable(L, LUAGI_OBJECT_FUNCS );
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
      return ltk_push_git_error( L );
   }

   luaL_getmetatable(L, LUAGI_OBJECT_FUNCS );
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
      return ltk_push_git_error( L );
   }

   return luagi_push_oid( L, oid );
}

int luagi_object_type( lua_State *L )
{
   git_object **obj = checkobject_at( L, 1 );

   git_otype type = git_object_type( *obj );
   lua_pushstring( L, git_object_type2string( type ) );

   return 1;
}

int luagi_object_free( lua_State *L )
{

   git_object **obj = checkobject_at( L, 1 );
   git_object_free( *obj );
   return 0;   
}
