#include "oid.h"
#include <lauxlib.h>

int luagi_check_oid( git_oid *out, lua_State *L, int pos )
{
   const char *str = luaL_checkstring( L, pos );
   return git_oid_fromstr( out, str );
}

int luagi_check_oid_prefix( git_oid *out, int *oid_len, lua_State *L, int pos )
{
   *oid_len = luaL_len( L, pos );
   const char *str = luaL_checkstring( L, pos );
   return git_oid_fromstrn( out, str, *oid_len );
}

int luagi_push_oid( lua_State *L, const git_oid *oid )
{
   char buf[GIT_OID_HEXSZ + 1 ];

   if( oid != NULL )
   {
      lua_pushstring( L, git_oid_tostr( buf, sizeof(buf), oid ) );
   }
   else
   {
      lua_pushnil( L );
   }
   return 1;
}
