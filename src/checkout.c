#include "checkout.h"
#include <git2/checkout.h>

#include "luagi.h"
#include "index.h"
#include "object.h"

int luagi_parse_checkout_options( git_checkout_options *options, lua_State *L, const int tableIndex )
{
   lua_getfield( L, tableIndex, LUAGI_CHECKOUT_OPTS_VERSION );
   int version = luaL_optinteger( L, -1, GIT_CHECKOUT_OPTIONS_VERSION );
   git_checkout_init_options( options, version );
   return 0;
}

int luagi_checkout_head( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_checkout_options opts;
   luagi_parse_checkout_options( &opts, L, 2 );

   if( git_checkout_head( *repo, &opts ) )
   {
      ERROR_ABORT( L ) 
   }
   return 0;
}

int luagi_checkout_index( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_index **index = checkindex_at( L, 2 );

   git_checkout_options opts;
   luagi_parse_checkout_options( &opts, L, 3 );

   if( git_checkout_index( *repo, *index, &opts ) )
   {
      ERROR_ABORT( L ) 
   }

   return 0;
}

int luagi_checkout_tree( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_object **obj = checkobject_at( L ,2 );

   git_checkout_options opts;
   luagi_parse_checkout_options( &opts, L, 3 );

   if( git_checkout_tree( *repo, *obj, &opts ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

