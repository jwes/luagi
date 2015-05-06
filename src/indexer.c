#include "indexer.h"

#include <git2/indexer.h>

#include "ltk.h"
#include "luagi.h"
#include "odb.h"
#include "oid.h"

static int luagi_transfer_progress_cb( const git_transfer_progress *stats, void *payload )
{
   luagi_foreach_t *p = payload;

   lua_pushvalue( p->L, p->callback_pos );
   luagi_push_transfer_stats( p->L, stats );

   if( lua_pcall( p->L, 1, 1, 0 ) )
   {
      luaL_error( p->L, "error calling progress callback" );
      free( p );
      return -1;
   }

   int ret = luaL_checkinteger( p->L, -1 );
   lua_pop( p->L, 1 );
   return ret;
}

int luagi_indexer_new( lua_State *L )
{
   git_odb **odb = checkodb_at( L, 1 );
   const char *path = luaL_checkstring( L, 2 );
   unsigned int mode = 0; //TODO mode
 
   luaL_checktype( L, 3, LUA_TFUNCTION );
   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );
   p->L = L;
   p->callback_pos = 3;
   git_indexer **out = lua_newuserdata( L, sizeof( git_indexer *) );

   if( git_indexer_new(  out, path, mode, *odb, luagi_transfer_progress_cb, p ) )
   {
      return ltk_push_error( L );
   }
   luaL_getmetatable( L, LUAGI_INDEXER_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_indexer_append( lua_State *L )
{
   git_indexer **indexer = checkindexer( L );
   int size = luaL_len( L, 2 );
   const char *data = luaL_checkstring( L , 2 );

   git_transfer_progress stats;

   if( git_indexer_append( *indexer, data, size, &stats ) )
   {
      return ltk_push_error( L );
   }
   return luagi_push_transfer_stats( L, &stats );
}

int luagi_indexer_commit( lua_State *L )
{
   git_indexer **indexer = checkindexer( L );
   git_transfer_progress stats; 

   if( git_indexer_commit( *indexer, &stats ) )
   {
      return ltk_push_error( L );
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
