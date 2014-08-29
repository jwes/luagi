#include <git2/merge.h>
#include "merge.h"
#include "luagi.h"
#include "oid.h"
#include "reference.h"
#include "index.h"
#include "commit.h"
#include "checkout.h"

#define NONE "none"
#define NORMAL "normal"
#define UP_TO_DATE "up to date"
#define FASTFORWARD "fast forward"
#define UNBORN "unborn"

#define NO_FASTFORWARD "no fast forward"
#define FASTFORWARD_ONLY "fast forward only"

#define PATH "path"
#define CONTENT "content"

int luagi_merge_analysis( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   int len =  luaL_len( L, 2 );
   const git_merge_head *heads[ len ];

   for( int i = 0; i < len; i++ )
   {
      lua_pushinteger( L, i );
      lua_gettable( L, 2 );
      heads[i] = *( check_mergehead_at( L, -1 ) );
   }

   git_merge_analysis_t anout;
   git_merge_preference_t pout;

   if( git_merge_analysis( &anout, &pout, *repo, heads, len ) )
   {
      ERROR_PUSH( L )
   }
   const char *aoutstr = NONE;
   const char *poutstr = NONE;

   switch( anout )
   {
      case GIT_MERGE_ANALYSIS_NONE:
         break;
      case GIT_MERGE_ANALYSIS_NORMAL:
         aoutstr = NORMAL;
         break;
      case GIT_MERGE_ANALYSIS_UP_TO_DATE:
         aoutstr = UP_TO_DATE;
         break;
      case GIT_MERGE_ANALYSIS_FASTFORWARD:
         aoutstr = FASTFORWARD;
         break;
      case GIT_MERGE_ANALYSIS_UNBORN:
         aoutstr = UNBORN;
         break;
   }
   lua_pushstring( L, aoutstr );
   switch( pout )
   {
      case GIT_MERGE_PREFERENCE_NONE:
         break;
      case GIT_MERGE_PREFERENCE_NO_FASTFORWARD:
         poutstr = NO_FASTFORWARD;
         break;
      case GIT_MERGE_PREFERENCE_FASTFORWARD_ONLY:
         poutstr = FASTFORWARD_ONLY;
         break;
   }
   lua_pushstring( L, poutstr );      
   return 2;
}
 
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

static int luagi_merge_init_file_options( lua_State *L __attribute__((unused)), int index __attribute__((unused)), git_merge_file_options *opts )
{
   return git_merge_file_init_options( opts, GIT_MERGE_FILE_OPTIONS_VERSION );
}

static int luagi_merge_init_options( lua_State *L __attribute__((unused)), int index __attribute__((unused)), git_merge_options *opts )
{
   return git_merge_init_options( opts, GIT_MERGE_OPTIONS_VERSION );
}


static int luagi_check_merge_input( lua_State *L, int index, git_merge_file_input *input )
{
   int ret = git_merge_file_init_input( input, GIT_MERGE_FILE_INPUT_VERSION );
   if( ret )
      return ret; 

   luaL_checktype( L, index, LUA_TTABLE );
   lua_getfield( L, index, PATH );
   input->path = luaL_checkstring( L, -1 );

   lua_getfield( L, index, CONTENT );
   input->size = luaL_len( L, -1 );
   input->ptr = luaL_checkstring( L, -1 );

   return 0;
}

int luagi_merge_file( lua_State *L )
{
   git_merge_file_input ancestor;
   luagi_check_merge_input( L, 1, &ancestor );
   git_merge_file_input ours;
   luagi_check_merge_input( L, 2, &ours );
   git_merge_file_input theirs;
   luagi_check_merge_input( L, 3, &theirs );

   git_merge_file_options opts;
   luagi_merge_init_file_options( L, 4, &opts );

   git_merge_file_result *out = lua_newuserdata( L, sizeof( git_merge_file_result ) );

   if( git_merge_file( out, &ancestor, &ours, &theirs, &opts ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_MERGERESULT_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_merge_file_from_index( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const git_index_entry *ancestor = checkindexentry_at( L, 2 );
   const git_index_entry *ours = checkindexentry_at( L, 3 );
   const git_index_entry *theirs = checkindexentry_at( L, 4 );

   git_merge_file_options opts;
   luagi_merge_init_file_options( L, 5, &opts );
   git_merge_file_result *res = lua_newuserdata( L, sizeof( git_merge_file_result ) );
   if( git_merge_file_from_index( res, *repo, ancestor, ours, theirs, &opts ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_MERGERESULT_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_merge_file_result_free( lua_State *L )
{
   git_merge_file_result *result = check_mergeresult_at( L, 1 );
   git_merge_file_result_free( result );
   return 0;
}

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

int luagi_merge( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   luaL_checktype( L, 2, LUA_TTABLE );

   int len =  luaL_len( L, 2 );
   const git_merge_head *heads[ len ];

   for( int i = 0; i < len; i++ )
   {
      lua_pushinteger( L, i );
      lua_gettable( L, 2 );
      heads[i] = *( check_mergehead_at( L, -1 ) );
   }

   //TODO options
   git_merge_options mopts;
   luagi_merge_init_options( L, 3, &mopts ); 
   git_checkout_options copts;
   luagi_parse_checkout_options( &copts, L, 4 ); 

   if( git_merge( *repo, heads, len, &mopts, &copts ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_mergeresult_automergable( lua_State *L )
{
   git_merge_file_result *res = check_mergeresult_at( L, 1 );
   lua_pushboolean( L, res->automergeable );
   return 1;
}

int luagi_mergeresult_path( lua_State *L )
{
   git_merge_file_result *res = check_mergeresult_at( L, 1 );
   lua_pushstring( L, res->path );
   return 1;
}

int luagi_mergeresult_mode( lua_State *L )
{
   git_merge_file_result *res = check_mergeresult_at( L, 1 );
   lua_pushinteger( L, res->mode );
   return 1;
}

int luagi_mergeresult_content( lua_State *L )
{
   git_merge_file_result *res = check_mergeresult_at( L, 1 );
   lua_pushstring( L, res->ptr );
   return 1;
}
