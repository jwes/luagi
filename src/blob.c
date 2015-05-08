#include "blob.h"

#include <git2/blob.h>
#include <string.h>

#include "ltk.h"
#include "luagi.h"
#include "oid.h"

int luagi_blob_lookup( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid oid;
   int len = 0;
   if( luagi_check_oid_prefix( &oid, &len, L, 2 ) )
   {
      ltk_error_abort( L );
      return 0;
   }
   git_blob **out = lua_newuserdata( L, sizeof( git_blob * ) ); 

   int ret = 0;
   if( len == GIT_OID_HEXSZ )
   {
      ret = git_blob_lookup( out, *repo, &oid );
   }
   else
   {
      ret = git_blob_lookup_prefix( out, *repo, &oid, len );
   }

   if( ret )
   {
      return ltk_push_git_error( L );
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
      return ltk_push_git_error( L );
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
      return ltk_push_git_error( L );
   }

   return luagi_push_oid( L, &oid );
}

static int chunk_cb( char *content, size_t max_length, void *payload )
{
   luagi_foreach_t *p = payload;
   lua_pushvalue( p->L, p->callback_pos );
   lua_pushinteger( p->L, max_length );
   
   if( lua_pcall( p->L, 1, 1, 0 ) )
   {
      return 0;
   }
   int len = 0;
   if( lua_type(p->L, -1) != LUA_TNIL )
   {
      len = luaL_len( p->L, -1 );
      const char *str = luaL_checkstring( p->L, -1 );
      strncpy( content, str, len );
   }
   lua_pop( p->L, 1 );
   return len; 
}
int luagi_blob_create_fromchunks( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   luaL_checktype( L, 2, LUA_TFUNCTION );
   const char *hintpath = luaL_optstring( L, 3, NULL );

   luagi_foreach_t *p = malloc( sizeof(luagi_foreach_t ));
   p->L = L,
   p->callback_pos = 2;

   git_oid oid;
   if( git_blob_create_fromchunks( &oid, *repo, hintpath, chunk_cb, p ) )
   {
      free( p );
      return ltk_push_git_error( L );
   }
   
   free( p );
   return luagi_push_oid( L, &oid );
}

int luagi_blob_create_frombuffer( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   int len = luaL_len( L, 2 );
   const char *buffer = luaL_checkstring( L, 2 );

   git_oid oid;
   if( git_blob_create_frombuffer( &oid, *repo, buffer, len ) )
   {
      return ltk_push_git_error( L );
   }

   return luagi_push_oid( L, &oid );
}

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

int luagi_blob_rawsize( lua_State *L )
{
   git_blob **blob = check_blob_at( L, 1 );
   lua_pushinteger( L, git_blob_rawsize( *blob ) );
   return 1;
}

int luagi_blob_rawcontent( lua_State *L )
{
   git_blob **blob = check_blob_at( L, 1 );
   const void *raw = git_blob_rawcontent( *blob );
   git_off_t size = git_blob_rawsize( *blob );
   lua_pushlstring( L, raw, size );
   return 1;
}

int luagi_blob_filtered_content( lua_State *L )
{
   git_blob **blob = check_blob_at( L, 1 );
   const char *as_path = luaL_checkstring( L, 2 );
   int check_for_binary = lua_toboolean( L, 3 );

   git_buf out = GIT_BUF_INIT_CONST(NULL, 0);
   if( git_blob_filtered_content( &out, *blob, as_path, check_for_binary ) )
   {
      return ltk_push_git_error( L );
   }

   lua_pushlstring( L, out.ptr, out.size );
   return 1;
}

int luagi_blob_is_binary( lua_State *L )
{
   git_blob **blob = check_blob_at( L, 1 );
   int b = git_blob_is_binary( *blob );
   lua_pushboolean( L, b );
   return 1;
}

