#ifndef IS_LGIT_TRANSPORT
#define IS_LGIT_TRANSPORT
#include <lua.h>
#include <lauxlib.h>

#define LGIT_TRANSPORT_FUNCS "is.westh.lgit.transport"
#define checktransport_at(L, N) \
      (git_transport**) luaL_checkudata( L, N, LGIT_TRANSPORT_FUNCS )
#define checktransport(L) checktransport_at( L, 1 )

int lgit_transport_new( lua_State *L );
int lgit_transport_local( lua_State *L );
int lgit_transport_dummy( lua_State *L );
int lgit_transport_smart( lua_State *L );

static const struct luaL_Reg lgit_transport_funcs [] = {
   { "from_url",     lgit_transport_new   },
   { "create_local", lgit_transport_local },
   { "create_dummy", lgit_transport_dummy },
   { "create_smart", lgit_transport_smart },
   { NULL, NULL }
};
#endif// IS_LGIT_TRANSPORT
