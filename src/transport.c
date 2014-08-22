#include "transport.h"
#include <lauxlib.h>
#include <git2/transport.h>
#include <git2/errors.h>
#include "luagi.h"
#include <string.h>

#define SSH "ssh"
#define HTTP "http"

/* TODO: git_transport_register and git_transport_unregister */

int luagi_transport_new( lua_State *L )
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

   luaL_getmetatable( L, LUAGI_TRANSPORT_FUNCS );
   lua_setmetatable( L, -2 );

   return 1;
}

int luagi_transport_local( lua_State *L )
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

   luaL_getmetatable( L, LUAGI_TRANSPORT_FUNCS );
   lua_setmetatable( L, -2 );

   return 1;
}

int luagi_transport_dummy( lua_State *L )
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

   luaL_getmetatable( L, LUAGI_TRANSPORT_FUNCS );
   lua_setmetatable( L, -2 );

   return 1;
}

static int callback ( git_smart_subtransport **out, 
                      git_transport *owner )
{
   return git_smart_subtransport_git( out, owner );
}

static int ssh_callback ( git_smart_subtransport **out, 
                      git_transport *owner )
{
   return git_smart_subtransport_ssh( out, owner );
}
static int http_callback ( git_smart_subtransport **out, 
                      git_transport *owner )
{
   return git_smart_subtransport_http( out, owner );
}

int luagi_transport_smart( lua_State *L )
{
   git_remote **remote = checkremote( L );     
   const char *tstring = luaL_checkstring( L, 2 );
   git_smart_subtransport_definition def;
   if( strncmp( tstring, HTTP, strlen( HTTP ) ) )
   {
      def.callback = http_callback;
      def.rpc = 1;
   }
   else if ( strncmp( tstring, SSH, strlen( SSH ) ) )
   {
      def.callback = ssh_callback;
      def.rpc = 0;
   }
   else 
   {
      def.callback = callback;
      def.rpc = 0;
   }
      
   git_transport **out = (git_transport **)lua_newuserdata( L, sizeof( git_transport * ) );
   if( git_transport_smart( out, *remote, &def ) )
   {
      const git_error *err = giterr_last();
      lua_pushnil( L );
      lua_pushstring( L, err->message );
      return 2;
   }

   luaL_getmetatable( L, LUAGI_TRANSPORT_FUNCS );
   lua_setmetatable( L, -2 );

   return 1;
}
