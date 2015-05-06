#include "graph.h"

#include <git2/graph.h>
#include <lauxlib.h>

#include "ltk.h"
#include "luagi.h"
#include "oid.h"

int luagi_graph_ahead_behind( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid local, upstream;
   luagi_check_oid( &local, L, 2 );
   luagi_check_oid( &upstream, L, 3 );

   size_t ahead, behind;

   if( git_graph_ahead_behind( &ahead, &behind, *repo, &local, &upstream ) )
   {
      return ltk_push_error( L );
   }

   lua_pushinteger( L, ahead );
   lua_pushinteger( L, behind );
   return 2;
}

int luagi_graph_descendant_of( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid commit, ancestor;
   luagi_check_oid( &commit, L, 2 );
   luagi_check_oid( &ancestor, L, 3 );

   lua_pushboolean( L, git_graph_descendant_of( *repo, &commit, &ancestor ) );
   return 1;
}

