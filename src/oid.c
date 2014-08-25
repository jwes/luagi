#include "oid.h"
#include <lauxlib.h>

int luagi_check_oid( git_oid *out, lua_State *L, int pos )
{
   const char *str = luaL_checkstring( L, pos );
   return git_oid_fromstr( out, str );
}

int luagi_push_oid( lua_State *L, const git_oid *oid )
{
   char buf[GIT_OID_HEXSZ + 1 ];
   lua_pushstring( L, git_oid_tostr( buf, sizeof(buf), oid ) );
   return 1;
}
