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

int luagi_revwalk_hide( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );
   git_oid oid;
   luagi_check_oid( &oid, L, 2 );

   if( git_revwalk_hide( *rev, &oid ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_revwalk_hide_glob( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );
   const char *glob = luaL_checkstring( L, 2 );

   if( git_revwalk_hide_glob( *rev, glob ) )
   {
        ERROR_ABORT( L )
   }
   return 0;
}

int luagi_revwalk_hide_head( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );

   if( git_revwalk_hide_head( *rev ) )

   {
      ERROR_ABORT( L )
   }
   return 0;
}


int luagi_revwalk_push_ref( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );
   const char *ref = luaL_checkstring( L, 2 );

   if( git_revwalk_push_ref( *rev, ref ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}
   
int luagi_revwalk_hide_ref( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );
   const char *ref = luaL_checkstring( L, 2 );

   if( git_revwalk_hide_ref( *rev, ref ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_revwalk_next( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );
   git_oid out;

   if( git_revwalk_next( &out, *rev ) )
   {
      ERROR_PUSH( L )
   }

   return luagi_push_oid( L, &out );
}
static unsigned int checkmode( lua_State *L, int index )
{
   unsigned int mode = GIT_SORT_NONE;

   const char *m_str = luaL_checkstring( L, index );
   const int len = luaL_len( L, index );
   switch( m_str[0] )
   {
   case 'n':
      mode = GIT_SORT_NONE;
      break;
   case 't':
      if(  len >= 2 && m_str[1] == 'i' )
      {
         mode = GIT_SORT_TIME;
      }
      else
      {
         mode = GIT_SORT_TOPOLOGICAL;
      }
      break;
   case 'r':
      mode = GIT_SORT_REVERSE;
      break;
   }
   return mode;
}
int luagi_revwalk_sorting( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );
   unsigned int sorting_mode = checkmode( L, 2 );

   git_revwalk_sorting( *rev, sorting_mode );

   return 0;
}

int luagi_revwalk_push_range( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );
   const char *range = luaL_checkstring( L, 2 );

   if( git_revwalk_push_range( *rev, range ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_revwalk_simplify_first_parent( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );

   git_revwalk_simplify_first_parent( *rev );

   return 0;
}

int luagi_revwalk_free( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );

   git_revwalk_free( *rev );

   return 0;
}

int luagi_revwalk_repository( lua_State *L )
{
   git_revwalk **rev = checkrevwalk( L );

   git_repository **repo = lua_newuserdata( L, sizeof( git_repository * ) );

   *repo = git_revwalk_repository( *rev );
   if( *repo == NULL )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_REVWALK_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}
   
int luagi_revwalk_add_hide_callback( lua_State *L )
{
   //TODO callbacks
   luaL_error( L, "not yet implemented" );
   return 0;
}

