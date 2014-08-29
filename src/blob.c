#include <git2/blob.h>
#include "blob.h"
#include "luagi.h"
#include "oid.h"

int luagi_blob_lookup( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid oid;
   if( luagi_check_oid( &oid, L, 2 ) )
   {
      ERROR_ABORT( L )
      return 0;
   }
   git_blob **out = lua_newuserdata( L, sizeof( git_blob * ) ); 
   if( git_blob_lookup( out, *repo, &oid ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_BLOB_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_blob_lookup_prefix( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid oid;
   if( luagi_check_oid( &oid, L, 2 ) )
   {
      ERROR_ABORT( L )
      return 0;
   }
   int len = luaL_checkinteger( L, 3 );

   git_blob **out = lua_newuserdata( L, sizeof( git_blob * ) ); 
   if( git_blob_lookup_prefix( out, *repo, &oid, len ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_BLOB_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_blob_create_fromworkdir( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *path = luaL_checkstring( L, 2 );

   git_oid oid;
   if( git_blob_create_fromworkdir( &oid, *repo, path ) )
   {
      ERROR_PUSH( L )
   }

   return luagi_push_oid( L, &oid );
}

int luagi_blob_create_fromdisk( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *path = luaL_checkstring( L, 2 );

   git_oid oid;
   if( git_blob_create_fromdisk( &oid, *repo, path ) )
   {
      ERROR_PUSH( L )
   }

   return luagi_push_oid( L, &oid );
}
int luagi_blob_create_fromchunks( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_blob_create_frombuffer( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

// blob
int luagi_blob_free( lua_State *L )
{
   git_blob **blob = check_blob_at( L, 1 );
   git_blob_free( *blob );
   return 0;
}

int luagi_blob_id( lua_State *L )
{
   git_blob **blob = check_blob_at( L, 1 );
   const git_oid *oid = git_blob_id( *blob );
   return luagi_push_oid( L, oid );
}

int luagi_blob_owner( lua_State *L )
{
   git_blob **blob = check_blob_at( L, 1 );
   git_repository **repo = lua_newuserdata( L, sizeof( git_repository *) );
   *repo = git_blob_owner( *blob );
   luaL_getmetatable( L, REPO_NAME );
   lua_setmetatable( L, -2 );
   return  1;
}

//TODO raw functions to lua?

int luagi_blob_filtered_content( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_blob_is_binary( lua_State *L )
{
   git_blob **blob = check_blob_at( L, 1 );
   int b = git_blob_is_binary( *blob );
   lua_pushboolean( L, b );
   return 1;
}

