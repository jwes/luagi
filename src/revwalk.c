#include <git2/revwalk.h>
#include "revwalk.h"
#include "luagi.h"
#include "oid.h"

int luagi_revwalk_new( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   git_revwalk **out = lua_newuserdata( L, sizeof( git_revwalk * ) );

   if( git_revwalk_new( out, *repo ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_REVWALK_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_revwalk_reset( lua_State *L )
{
   git_revwalk **revwalk = checkrevwalk( L );

   git_revwalk_reset( *revwalk );
   return 0;
}

int luagi_revwalk_push( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );

   git_oid oid;
   luagi_check_oid( &oid, L, 2 );

   if( git_revwalk_push( *rev, &oid ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_revwalk_push_glob( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );

   const char *glob = luaL_checkstring( L, 2 );
   if( git_revwalk_push_glob( *rev, glob ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_revwalk_push_head( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );

   if( git_revwalk_push_head( *rev ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_revwalk_hide( lua_State *L );
int luagi_revwalk_hide_glob( lua_State *L );
int luagi_revwalk_hide_head( lua_State *L );
int luagi_revwalk_push_ref( lua_State *L );
int luagi_revwalk_hide_ref( lua_State *L );
int luagi_revwalk_next( lua_State *L );
int luagi_revwalk_sorting( lua_State *L );
int luagi_revwalk_push_range( lua_State *L );
int luagi_revwalk_simplify_first_parent( lua_State *L );
int luagi_revwalk_free( lua_State *L );
int luagi_revwalk_repository( lua_State *L );
int luagi_revwalk_add_hide_callback( lua_State *L );
