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

int luagi_stash_foreach( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
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
