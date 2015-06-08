#include "cherrypick.h"

#include <git2/cherrypick.h>
#include <lauxlib.h>

#include "commit.h"
#include "index.h"
#include "ltk.h"
#include "luagi.h"
#include "merge.h"

static int luagi_cherrypick_init_options( lua_State *L __attribute__((unused)), int index __attribute__((unused)), git_cherrypick_options *opts )
{
   int ret = git_cherrypick_init_options( opts, GIT_CHERRYPICK_OPTIONS_VERSION );

   return ret;
}

int luagi_cherrypick_commit( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_commit **cherrypick_commit = checkcommit_at( L, 2 );
   git_commit **our_commit = checkcommit_at( L, 3 );
   luaL_checktype( L, 4, LUA_TBOOLEAN );
   int mainline = lua_toboolean( L, 4 );
   git_merge_options opts;
   luagi_merge_init_options( L, 5, &opts );   

   git_index **out = lua_newuserdata( L, sizeof( git_index * ) );

   if( git_cherrypick_commit( out, *repo, *cherrypick_commit, *our_commit, mainline, &opts ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_INDEX_FUNCS );
   return 1;
}

int luagi_cherrypick( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_commit **commit = checkcommit_at( L, 2 );

   git_cherrypick_options opts;
   luagi_cherrypick_init_options( L, 3, &opts );

   if( git_cherrypick( *repo, *commit, &opts ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}
