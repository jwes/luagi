#include <string.h>

#include "reset.h"
#include "luagi.h"
#include "object.h"

#include <git2/reset.h>
#include <lauxlib.h>

#define SOFT "soft"
#define MIXED "mixed"
#define HARD "hard"

static git_reset_t luagi_check_resettype( lua_State *L, int index )
{
   const char *reset_mode = luaL_checkstring( L, index );
   if( strncmp( reset_mode, HARD, strlen( HARD ) ) )
   {
      return GIT_RESET_HARD;
   }
   else if( strncmp( reset_mode, MIXED, strlen( MIXED ) ) )
   {
      return GIT_RESET_MIXED;
   }
   else
   {
      return GIT_RESET_SOFT;
   }

}

int luagi_reset( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_object **target = checkobject_at( L, 2 );
   git_reset_t reset = luagi_check_resettype( L, 3 );

   git_signature sig;
   table_to_signature( L, &sig, 4 );
   const char *log_message = luaL_optstring( L, 5, NULL );

   if( git_reset( *repo, *target, reset, &sig, log_message ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_reset_default( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_object **target = checkobject_at( L, 2 );

   git_strarray array = luagi_strings_from_lua_list( L, 3 );


   if( git_reset_default( *repo, *target, &array ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}
