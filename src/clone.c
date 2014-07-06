#include <lauxlib.h>
#include <git2/clone.h>
#include <git2/errors.h>
#include "wien.h"
#include "clone.h"
#include "checkout.h"

static void parse_options( git_clone_options* options, lua_State *L, const int tableIndex )
{
   if( ! lua_istable( L, tableIndex ) )
      git_clone_init_options( options, GIT_CLONE_OPTIONS_VERSION );
      return;

   lua_getfield( L, tableIndex, LGIT_CLONE_OPTS_VERSION );
   int version = luaL_optinteger( L, -1, GIT_CLONE_OPTIONS_VERSION );

   git_clone_init_options( options, version );

   lua_getfield( L, tableIndex, LGIT_CLONE_OPTS_CHECKOUT_OPTS );
   if( lua_istable( L, -1 ) )
   {
      git_checkout_options opts;
      lgit_parse_checkout_options( &opts, L, -1 );
      options->checkout_opts = opts;
   }
   else
   {
      lua_pop( L, -1 );
   }

   lua_getfield( L, tableIndex, LGIT_CLONE_OPTS_BARE );
   options->bare = luaL_optinteger( L, -1, 0 );
   lua_getfield( L, tableIndex, LGIT_CLONE_OPTS_CHECKOUT_BRANCH );
   options->checkout_branch = luaL_optstring(L, -1, NULL );
}

int lgit_clone( lua_State *L )
{
   const char *url = luaL_checkstring( L, 1 );
   const char *local_path = luaL_checkstring( L, 2 );
   git_clone_options options;

   parse_options( &options, L, 3 );

   git_repository **repo = (git_repository **) lua_newuserdata( L, sizeof(git_repository *) );
   
   if( git_clone( repo, url, local_path, &options) ) 
   {
      const git_error* err = giterr_last();
      lua_pushnil( L );
      lua_pushfstring( L, "unable to clone repository from %s to %s --> %s", url, local_path, err->message );
      return 2;
   }
   luaL_getmetatable( L, REPO_NAME );
   lua_setmetatable( L, -2 );
   return 1;
}

int lgit_clone_into( lua_State *L )
{
   luaL_error(L, "not yet implemented");
   return 0;
}
