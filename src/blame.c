#include <git2/blame.h>
#include "blame.h"
#include "luagi.h"


static int luagi_blame_init_options( lua_State *L __attribute__((unused)),
                                     int index __attribute__((unused)),
                                     git_blame_options *opts )
{
   int ret = git_blame_init_options( opts, GIT_BLAME_OPTIONS_VERSION );
   // TODO options

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
   lua_setfield( L, -2, BLAME_LINES_IN_HUNK ); 

   // TODO to table
   return 1;
}

int luagi_blame_get_hunk_byindex( lua_State *L )
{
   git_blame **blame = checkblame_at( L, 1 );
   int index = luaL_checkinteger( L, 2 );

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
