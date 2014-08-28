#include <git2/merge.h>
#include "merge.h"
#include "luagi.h"
#include "oid.h"
#include "reference.h"
#include "index.h"
#include "commit.h"

int luagi_merge_analysis( lua_State *L ){ luaL_error( L, "not yet implemented"); return 0; }
int luagi_merge_base( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid one, two, out;
   if( luagi_check_oid( &one, L, 2 ) )
   {
      ERROR_ABORT( L )
      return 0;
   }
   if( luagi_check_oid( &two, L, 3 ) )
   {
      ERROR_ABORT( L )
      return 0;
   }

   if( git_merge_base( &out, *repo, &one, &two ) )
   {
      ERROR_PUSH( L )
   }
   return luagi_push_oid( L , &out );
}

#define tidx 2
static int base_for_multiple( lua_State *L, int (*func)( git_oid *out, git_repository *name, size_t length, const git_oid inarr [] ) )
{
   git_repository **repo = checkrepo( L, 1 );
   luaL_checktype( L, tidx, LUA_TTABLE );
   int count = luaL_len( L, tidx );
   git_oid input_array [ count ];

   for( int i = 0; i < count; i++ )
   {
      lua_pushinteger( L, i );
      lua_gettable( L, tidx );
      luagi_check_oid( &input_array[i], L, -1 );
   }
   git_oid out;
   if( func( &out, *repo, count, input_array ) )
   {
      ERROR_PUSH( L )
   }
   return luagi_push_oid( L, &out );
}

int luagi_merge_base_many( lua_State *L )
{
   return base_for_multiple( L, git_merge_base_many );
}

int luagi_merge_base_octopus( lua_State *L )
{
   return base_for_multiple( L, git_merge_base_octopus );
}

int luagi_merge_head_from_ref( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_reference **ref = check_ref_at( L, 2 );

   git_merge_head **head = lua_newuserdata( L, sizeof( git_merge_head *) );

   if( git_merge_head_from_ref( head, *repo, *ref ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_MERGEHEAD_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}
   
int luagi_merge_head_from_fetchhead( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *branch_name = luaL_checkstring( L, 2 );
   const char *remote_url = luaL_checkstring( L, 3 );
   git_oid oid;
   if( luagi_check_oid( &oid, L, 4 ) )
   {
      ERROR_ABORT( L );
      return 0;
   }

   git_merge_head **head = lua_newuserdata( L, sizeof( git_merge_head * ) );

   if( git_merge_head_from_fetchhead( head, *repo, branch_name, remote_url, &oid ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_MERGEHEAD_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_merge_head_from_id( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid oid;
   if( luagi_check_oid( &oid, L, 2 ) )
   {
      ERROR_ABORT( L );
      return 0;
   }

   git_merge_head **head = lua_newuserdata( L, sizeof( git_merge_head * ) );

   if( git_merge_head_from_id( head, *repo, &oid ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_MERGEHEAD_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

   

int luagi_merge_head_id( lua_State *L )
{
   git_merge_head **head = check_mergehead_at( L, 1 );
   const git_oid *out = git_merge_head_id( *head );
   return luagi_push_oid( L, out );
}

int luagi_merge_head_free( lua_State *L )
{
   git_merge_head **head = check_mergehead_at( L, 1 );
   git_merge_head_free( *head );
   return 0;
}

static int luagi_merge_init_options( lua_State *L __attribute__((unused)), int index __attribute__((unused)), git_merge_options *opts )
{
   return git_merge_init_options( opts, GIT_MERGE_OPTIONS_VERSION );
}

int luagi_merge_file( lua_State *L ){ luaL_error( L, "not yet implemented"); return 0; }
int luagi_merge_file_from_index( lua_State *L ){ luaL_error( L, "not yet implemented"); return 0; }
int luagi_merge_file_result_free( lua_State *L ){ luaL_error( L, "not yet implemented"); return 0; }

int luagi_merge_trees( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_tree **ancestor_tree = checktree_at( L, 2 );
   git_tree **our_tree = checktree_at( L, 3 );
   git_tree **their_tree = checktree_at( L, 4 );
   //TODO options
   git_merge_options opts;
   luagi_merge_init_options( L, 5, &opts );
   
   git_index **index = lua_newuserdata( L, sizeof( git_index *) );
   if( git_merge_trees( index, *repo, *ancestor_tree, *our_tree, *their_tree, &opts ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_INDEX_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}
int luagi_merge_commits( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_commit **our = checkcommit_at( L, 2 );
   git_commit **their = checkcommit_at( L, 3 );
   //TODO options
   git_merge_options opts;
   luagi_merge_init_options( L, 4, &opts );
   
   git_index **index = lua_newuserdata( L, sizeof( git_index *) );
   if( git_merge_commits( index, *repo, *our, *their, &opts ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_INDEX_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}
int luagi_merge( lua_State *L ){ luaL_error( L, "not yet implemented"); return 0; }

