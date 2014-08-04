#ifndef IS_LGIT_CHECKOUT
#define IS_LGIT_CHECKOUT
#include <git2/checkout.h>
#include <lua.h>
#include <lauxlib.h>

#define LGIT_CHECKOUT_OPTS_VERSION "version"

int lgit_parse_checkout_options( git_checkout_options *options, lua_State *L, const int tableIndex )
{
   lua_getfield( L, tableIndex, LGIT_CHECKOUT_OPTS_VERSION );
   int version = luaL_optinteger( L, -1, GIT_CHECKOUT_OPTIONS_VERSION );
   git_checkout_init_options( options, version );
   return 0;
}
#endif
