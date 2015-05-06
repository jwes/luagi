#include "cherrypick.h"

#include <git2/cherrypick.h>
#include <lauxlib.h>

#include "commit.h"
#include "index.h"
#include "ltk.h"
#include "luagi.h"
#include "merge.h"

static int luagi_cherry_pick_init_options( lua_State *L __attribute__((unused)), int index __attribute__((unused)), git_cherry_pick_options *opts )
{
   int ret = git_cherry_pick_init_options( opts, GIT_CHERRY_PICK_OPTIONS_VERSION );

   return ret;
}

int luagi_cherry_pick_commit( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_commit **cherry_pick_commit = checkcommit_at( L, 2 );
   git_commit **our_commit = checkcommit_at( L, 3 );
   luaL_checktype( L, 4, LUA_TBOOLEAN );
   int mainline = lua_toboolean( L, 4 );
   git_merge_options opts;
   luagi_merge_init_options( L, 5, &opts );   

   git_index **out = lua_newuserdata( L, sizeof( git_index * ) );

   if( git_cherry_pick_commit( out, *repo, *cherry_pick_commit, *our_commit, mainline, &opts ) )
   {
      return ltk_push_error( L );
   }
   luaL_getmetatable( L, LUAGI_INDEX_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_cherry_pick( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_commit **commit = checkcommit_at( L, 2 );

   git_cherry_pick_options opts;
   luagi_cherry_pick_init_options( L, 3, &opts );

   if( git_cherry_pick( *repo, *commit, &opts ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}
