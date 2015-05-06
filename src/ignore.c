#include "ignore.h"

#include <git2/ignore.h>
#include <lauxlib.h>

#include "ltk.h"
#include "luagi.h"

int luagi_ignore_add_rules( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *rules = luaL_checkstring( L, 2 );

   if( git_ignore_add_rule( *repo, rules ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_ignore_clear_internal_rules( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   if( git_ignore_clear_internal_rules( *repo ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_ignore_path_is_ignored( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *path = luaL_checkstring( L, 2 );

   int ignored;
   if( git_ignore_path_is_ignored( &ignored, *repo, path ) )
   {
      return ltk_push_error( L );
   }
   lua_pushboolean( L, ignored );
   return 1;
}  
