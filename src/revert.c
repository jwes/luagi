#include <git2/revert.h>

#include <lauxlib.h>

#include "revert.h"
#include "luagi.h"
#include "commit.h"
#include "merge.h"
#include "index.h"


int luagi_revert_commit( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_commit **revert_commit = checkcommit_at( L, 2 );
   git_commit **commit = checkcommit_at( L, 3 );

   luaL_checktype( L, 4, LUA_TBOOLEAN );
   int mainline = lua_toboolean( L, 4 );
   
   git_merge_options opts;
   luagi_merge_init_options( L, 5, &opts );

   git_index **out = lua_newuserdata( L, sizeof(git_index *) );
   if( git_revert_commit( out, *repo, *revert_commit, *commit, mainline, &opts ) )
   {
      ERROR_PUSH( L )
   }
   
   luaL_getmetatable( L, LUAGI_INDEX_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}
static int luagi_revert_init_options( lua_State *L __attribute__((unused)),
                                       int index __attribute__((unused)),
                                       git_revert_options *opts )
{
   int ret = git_revert_init_options( opts, GIT_REVERT_OPTIONS_VERSION );

   return ret;
}

int luagi_revert( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_commit **commit = checkcommit_at( L, 2 );

   git_revert_options opts;
   luagi_revert_init_options( L, 3, &opts );

   if( git_revert( *repo, *commit, &opts ) )
   {
      ERROR_ABORT( L )
   }

   return 0;
}
