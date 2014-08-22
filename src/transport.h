#ifndef IS_LUAGI_TRANSPORT
#define IS_LUAGI_TRANSPORT
#include <lua.h>
#include <lauxlib.h>

#define LUAGI_TRANSPORT_FUNCS "is.westh.luagi.transport"
#define checktransport_at(L, N) \
      (git_transport**) luaL_checkudata( L, N, LUAGI_TRANSPORT_FUNCS )
#define checktransport(L) checktransport_at( L, 1 )

int luagi_transport_new( lua_State *L );
int luagi_transport_local( lua_State *L );
int luagi_transport_dummy( lua_State *L );
int luagi_transport_smart( lua_State *L );

#endif// IS_LUAGI_TRANSPORT
