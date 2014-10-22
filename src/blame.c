#include <git2/blame.h>
#include "blame.h"
#include "luagi.h"
#include "oid.h"
static int luagi_blame_init_options( lua_State *L __attribute__((unused)),
                                     int index __attribute__((unused)),
                                     git_blame_options *opts )
{
   int ret = git_blame_init_options( opts, GIT_BLAME_OPTIONS_VERSION );

   if( lua_type( L, index ) != LUA_TTABLE )
      return ret;

   add_flag( opts->flags, L, index, 
            NORMAL, 
            GIT_BLAME_NORMAL );
   add_flag( opts->flags, L, index, 
            COPIES_IN_SAME_FILE, 
            GIT_BLAME_TRACK_COPIES_SAME_FILE );
   add_flag( opts->flags, L, index, 
            COPIES_IN_SAME_COMMIT_MOVES, 
            GIT_BLAME_TRACK_COPIES_SAME_COMMIT_MOVES );
   add_flag( opts->flags, L, index, 
            COPIES_IN_SAME_COMMIT_COPIES, 
            GIT_BLAME_TRACK_COPIES_SAME_COMMIT_COPIES );
   add_flag( opts->flags, L, index, 
            COPIES_ANY_COMMIT_COPIES, 
            GIT_BLAME_TRACK_COPIES_ANY_COMMIT_COPIES );
   add_flag( opts->flags, L, index, 
             FIRST_PARENT,
             GIT_BLAME_FIRST_PARENT );

   lua_getfield( L, index, MIN_MATCH );
   int16_t min_match = luaL_optinteger( L, -1, -1 );
   if( min_match >= 0 )
   {
      opts->min_match_characters = min_match;
   }

   lua_getfield( L, index, NEWEST );
   if( ! lua_isnil( L, -1 ) )
   {
      luagi_check_oid( &opts->newest_commit, L, -1 );
   }
   
   lua_getfield( L, index, OLDEST );
   if( ! lua_isnil( L, -1 ) )
   {
      luagi_check_oid( &opts->oldest_commit, L, -1 );
   }

   lua_getfield( L, index, MIN_LINE );
   int32_t min_line = luaL_optinteger( L, -1, -1 );
   if( min_line >= 0 )
   {
      opts->min_line = min_line;
   }
   
   lua_getfield( L, index, MAX_LINE );
   int32_t max_line = luaL_optinteger( L, -1, -1 );
   if( max_line >= 0 )
   {
      opts->max_line = max_line;
   }

   return ret;
}
       
int luagi_blame_file( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *path = luaL_checkstring( L, 2 );
   git_blame_options opts;
   luagi_blame_init_options( L, 3, &opts );

   git_blame **out = lua_newuserdata( L, sizeof( git_blame *) );

   if( git_blame_file( out, *repo, path, &opts ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_BLAME_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_blame_buffer( lua_State *L )
{
   git_blame **reference = checkblame_at( L, 1 );
   int len = luaL_len( L, 2 );
   const char *buffer = luaL_checkstring( L, 2 );

   git_blame **out = lua_newuserdata( L, sizeof( git_blame *) );

   if( git_blame_buffer( out, *reference, buffer, len ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_BLAME_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}


int luagi_blame_get_hunk_count( lua_State *L )
{
   git_blame **blame = checkblame_at( L, 1 );

   lua_pushinteger( L, git_blame_get_hunk_count( *blame ) );
   return 1;
}

static int luagi_hunk_to_table( lua_State *L, const git_blame_hunk *hunk )
{
   lua_newtable( L );
   lua_pushinteger( L, hunk->lines_in_hunk );
   lua_setfield( L, -2, LINES_IN_HUNK ); 

   //create the table final
   lua_newtable( L );
   luagi_push_oid( L, &hunk->final_commit_id );
   lua_setfield( L, -2, OID );

   lua_pushinteger( L, hunk->final_start_line_number );
   lua_setfield( L, -2, START_LINE_NUMBER );

   signature_to_table( L, hunk->final_signature );
   lua_setfield( L, -2, SIGNATURE );

   //set the table final
   lua_setfield( L, -2, FINAL );

   //create the table orig
   lua_newtable( L );
   luagi_push_oid( L, &hunk->orig_commit_id );
   lua_setfield( L, -2, OID );

   lua_pushstring( L, hunk->orig_path );
   lua_setfield( L, -2, PATH );

   lua_pushinteger( L, hunk->orig_start_line_number );
   lua_setfield( L, -2, START_LINE_NUMBER );

   signature_to_table( L, hunk->orig_signature );
   lua_setfield( L, -2, SIGNATURE );

   //set the table orig
   lua_setfield( L, -2, ORIG );

   lua_pushboolean( L, hunk->boundary );
   lua_setfield( L, -2, IS_BOUNDARY );

   return 1;
}

int luagi_blame_get_hunk_byindex( lua_State *L )
{
   git_blame **blame = checkblame_at( L, 1 );
   int index = luaL_checkinteger( L, 2 );
   
   index--;
   int max = git_blame_get_hunk_count( *blame );
   if( index < 0 || index >= max )
   {
      lua_pushnil( L );
      lua_pushfstring( L, "invalid value for index %d, allowed range (%d, %d)", index + 1, 1, max );
      return 2;
   }

   const git_blame_hunk *hunk = git_blame_get_hunk_byindex( *blame, index );
   if( hunk == NULL )
   {
      ERROR_PUSH( L )
   }
   
   return luagi_hunk_to_table( L, hunk );
}

int luagi_blame_get_hunk_byline( lua_State *L )
{
   git_blame **blame = checkblame_at( L, 1 );
   int line = luaL_checkinteger( L, 2 );

   if( line <= 0 )
   {
      lua_pushnil( L );
      lua_pushstring( L, "invalid value for line" );
      return 2;
   }

   const git_blame_hunk *hunk = git_blame_get_hunk_byline( *blame, line );
   if( hunk == NULL )
   {
      ERROR_PUSH( L )
   }
   
   return luagi_hunk_to_table( L, hunk );
}

int luagi_blame_free( lua_State *L )
{
   git_blame **blame = checkblame_at( L, 1 );
   git_blame_free( *blame );
   return 0;
}
