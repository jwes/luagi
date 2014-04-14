#ifndef IS_LGIT_COMMIT
#define IS_LGIT_COMMIT
#include <lua.h>

int lgit_commit_lookup( lua_State *L );
int lgit_commit_gc( lua_State *L );

#endif
