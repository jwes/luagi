#include <git2/indexer.h>
#include "indexer.h"

#include "luagi.h"
#include "odb.h"
#include "oid.h"

int luagi_indexer_new( lua_State *L );
int luagi_indexer_append( lua_State *L );
int luagi_indexer_commit( lua_State *L )
{
   git_indexer **indexer = checkindexer( L );
   git_transfer_progress stats; 

   if( git_indexer_commit( *indexer, &stats ) )
   {
      ERROR_PUSH( L )
   }

   return luagi_push_transfer_stats( L, &stats );
}

int luagi_indexer_hash( lua_State *L )
{
   git_indexer **indexer = checkindexer( L );
   const git_oid *oid = git_indexer_hash( *indexer );

   return luagi_push_oid( L, oid );
}

int luagi_indexer_free( lua_State *L )
{
   git_indexer **indexer = checkindexer( L );
   git_indexer_free( *indexer );
   return 0;
}
