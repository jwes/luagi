#ifndef IS_LUAGI_CHECKOUT
#define IS_LUAGI_CHECKOUT
#include <git2/checkout.h>
#include <lua.h>
#include <lauxlib.h>

#define LUAGI_CHECKOUT_OPTS_VERSION "version"

int luagi_parse_checkout_options( git_checkout_options *options, lua_State *L, const int tableIndex );


int luagi_checkout_head( lua_State *L );
int luagi_checkout_index( lua_State *L );
int luagi_checkout_tree( lua_State *L );

#endif
