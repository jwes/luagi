#include "stash.h"
#include <git2/oid.h>
#include <git2/stash.h>
#include <git2/signature.h>
#include "luagi.h"

int luagi_stash_save( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 ); 
   
   luaL_checktype( L, 2, LUA_TTABLE );
   git_signature stasher;
   if( table_to_signature( L, &stasher, 2 ) )
   {
      ERROR_PUSH( L )
   }

   const char *message = luaL_optstring( L, 3, NULL );

   //TODO flags:
   unsigned int flags = GIT_STASH_DEFAULT;

   git_oid out;
   if( git_stash_save( &out, *repo, &stasher, message, flags ) )
   {
      ERROR_PUSH( L )
   }

   char buf[ GIT_OID_HEXSZ + 1 ];

   lua_pushstring( L, git_oid_tostr( buf, sizeof( buf ), &out ) );
   return 1;
}
struct foreach_payload
{
   lua_State *L;
   int callback_pos;
};

static int callback ( size_t index, const char *message, 
                const git_oid *stash_id, void *payload )
{
   struct foreach_payload *p = payload;

   lua_pushvalue( p->L, p->callback_pos );
   lua_pushinteger( p->L, index );
   lua_pushstring( p->L, message );

   char buf[ GIT_OID_HEXSZ + 1];
   lua_pushstring( p->L, git_oid_tostr( buf, sizeof( buf ), stash_id ) );
   if( lua_pcall( p->L, 3, 1, 0 ) != LUA_OK )
   {
      luaL_error( p->L, "can not call stash callback" );
   }

   int ret = luaL_checkinteger( p->L, -1 );
   lua_pop( p->L, 1 );
   return ret;
}

int luagi_stash_foreach( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   struct foreach_payload *p = malloc( sizeof( struct foreach_payload ) );
   luaL_checktype( L, 2, LUA_TFUNCTION );
   p->L = L;
   p->callback_pos = 2;

   if( git_stash_foreach( *repo, callback, p ) )
   {
      ERROR_ABORT( L )
   }
   free( p );
   return 0;
}

int luagi_stash_drop( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   size_t index = luaL_checkinteger( L, 2 );

   if( git_stash_drop( *repo, index ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}