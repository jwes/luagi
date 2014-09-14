#ifndef IS_LUAGI_OID
#define IS_LUAGI_OID
#include <lua.h>
#include <git2/oid.h>

int luagi_push_oid( lua_State *L, const git_oid *oid );
int luagi_check_oid( git_oid *out, lua_State *L, int pos );
int luagi_check_oid_prefix( git_oid *out, int *oid_len, lua_State *L, int pos );

#endif
