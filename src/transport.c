#include "transport.h"
#include <lauxlib.h>
#include <git2/transport.h>
#include <git2/errors.h>
#include "wien.h"

/* TODO: git_transport_register and git_transport_unregister */

int lgit_transport_new( lua_State *L )
{
   git_remote **remote = checkremote( L );
   const char* url = luaL_checkstring( L, 2 );
   git_transport **out = (git_transport **)lua_newuserdata( L, sizeof( git_transport* ) );
   if( git_transport_new( out, *remote, url ) )
   {
      const git_error* err = giterr_last();
      lua_pushnil( L );
      lua_pushstring( L, err->message );
      return 2;
   }

   luaL_getmetatable( L, LGIT_TRANSPORT_FUNCS );
   lua_setmetatable( L, -2 );

   return 1;
}

int lgit_transport_local( lua_State *L )
{
   git_remote **remote = checkremote( L );     

   git_transport **out = (git_transport **)lua_newuserdata( L, sizeof( git_transport * ) );
   if( git_transport_local( out, *remote, NULL ) )
   {
      const git_error *err = giterr_last();
      lua_pushnil( L );
      lua_pushstring( L, err->message );
      return 2;
   }

   luaL_getmetatable( L, LGIT_TRANSPORT_FUNCS );
   lua_setmetatable( L, -2 );

   return 1;
}

int lgit_transport_dummy( lua_State *L )
{
   git_remote **remote = checkremote( L );     

   git_transport **out = (git_transport **)lua_newuserdata( L, sizeof( git_transport * ) );
   if( git_transport_dummy( out, *remote, NULL ) )
   {
      const git_error *err = giterr_last();
      lua_pushnil( L );
      lua_pushstring( L, err->message );
      return 2;
   }

   luaL_getmetatable( L, LGIT_TRANSPORT_FUNCS );
   lua_setmetatable( L, -2 );

   return 1;
}
