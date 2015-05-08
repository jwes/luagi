#include "merge.h"

#include "checkout.h"
#include "commit.h"
#include "index.h"
#include "ltk.h"
#include "luagi.h"
#include "oid.h"
#include "reference.h"

static void fill_merge_heads( lua_State *L, int index, const git_merge_head **heads, int len )
{
   for( int i = 0; i < len; i++ )
   {
      lua_pushinteger( L, i + 1 );
      lua_gettable( L, index );
      heads[i] = *( check_mergehead_at( L, -1 ) );
      lua_pop( L, 1 );
   }
}

int luagi_merge_analysis( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   int len =  luaL_len( L, 2 );
   const git_merge_head *heads[ len ];
   fill_merge_heads( L, 2, heads, len );

   git_merge_analysis_t anout;
   git_merge_preference_t pout;

   if( git_merge_analysis( &anout, &pout, *repo, heads, len ) )
   {
      return ltk_push_git_error( L );
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

static int push_merge_file_result( lua_State *L, git_merge_file_result *res )
{
   lua_newtable( L );
   lua_pushboolean( L, res->automergeable );
   lua_setfield( L, -2, AUTOMERGEABLE );

   lua_pushstring( L, res->path );
   lua_setfield( L, -2, PATH );

   lua_pushinteger( L, res->mode );
   lua_setfield( L, -2, MODE );

   lua_pushstring( L, res->ptr );
   lua_setfield( L, -2, CONTENT );
   return 1;
}

int luagi_merge_base( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid one, two, out;
   if( luagi_check_oid( &one, L, 2 ) )
   {
      ltk_error_abort( L );
      return 0;
   }
   if( luagi_check_oid( &two, L, 3 ) )
   {
      ltk_error_abort( L );
      return 0;
   }

   if( git_merge_base( &out, *repo, &one, &two ) )
   {
      return ltk_push_git_error( L );
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

   for( int i = 1; i <= count; i++ )
   {
      lua_pushinteger( L, i );
      lua_gettable( L, tidx );
      luagi_check_oid( &input_array[i-1], L, -1 );
   }
   git_oid out;
   if( func( &out, *repo, count, input_array ) )
   {
      return ltk_push_git_error( L );
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
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_MERGEHEAD_FUNCS );
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
      ltk_error_abort( L );;
      return 0;
   }

   git_merge_head **head = lua_newuserdata( L, sizeof( git_merge_head * ) );

   if( git_merge_head_from_fetchhead( head, *repo, branch_name, remote_url, &oid ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_MERGEHEAD_FUNCS );
   return 1;
}

int luagi_merge_head_from_id( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid oid;
   if( luagi_check_oid( &oid, L, 2 ) )
   {
      ltk_error_abort( L );;
      return 0;
   }

   git_merge_head **head = lua_newuserdata( L, sizeof( git_merge_head * ) );

   if( git_merge_head_from_id( head, *repo, &oid ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_MERGEHEAD_FUNCS );
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
static void luagi_set_file_favor( git_merge_file_favor_t *favor, const char *favorstr )
{
   if( favorstr && *favorstr )
   {
      switch( *favorstr )
      {
         case 'n'://ormal
            *favor = GIT_MERGE_FILE_FAVOR_NORMAL;
            break;
         case 'o'://urs
            *favor = GIT_MERGE_FILE_FAVOR_OURS;
            break;
         case 't'://heirs
            *favor = GIT_MERGE_FILE_FAVOR_THEIRS;
            break;
         case 'u'://nion
            *favor = GIT_MERGE_FILE_FAVOR_UNION;
            break;
      }
   }
}

static int luagi_merge_init_file_options( lua_State *L __attribute__((unused)), int index __attribute__((unused)), git_merge_file_options *opts )
{
   int ret = git_merge_file_init_options( opts, GIT_MERGE_FILE_OPTIONS_VERSION );

   lua_getfield( L, index, ANCESTOR_LABEL );
   const char *ancestor = luaL_optstring( L, -1, NULL );
   if( ancestor )
   {
      opts->ancestor_label = ancestor;
   }

   lua_getfield( L, index, OUR_LABEL );
   const char *our = luaL_optstring( L, -1, NULL );
   if( our )
   {
      opts->our_label = our;
   }

   lua_getfield( L, index, THEIR_LABEL );
   const char *their = luaL_optstring( L, -1, NULL );
   if( their )
   {
      opts->their_label = their;
   }

   add_flag( opts->flags, L, index, DEFAULT, GIT_MERGE_FILE_DEFAULT );
   add_flag( opts->flags, L, index, STYLE_MERGE, GIT_MERGE_FILE_STYLE_MERGE );
   add_flag( opts->flags, L, index, STYLE_DIFF3, GIT_MERGE_FILE_STYLE_DIFF3 );
   add_flag( opts->flags, L, index, SIMPLIFY_ALNUM, GIT_MERGE_FILE_SIMPLIFY_ALNUM );
   
   lua_getfield( L, index, FILE_FAVOR );
   const char * favor = luaL_optstring( L, -1, NULL );
   luagi_set_file_favor( &opts->favor, favor );

   return ret;
}

int luagi_merge_init_options( lua_State *L __attribute__((unused)), int index __attribute__((unused)), git_merge_options *opts )
{
   int ret =  git_merge_init_options( opts, GIT_MERGE_OPTIONS_VERSION );

   add_flag( opts->flags, L, index, FIND_RENAMES, GIT_MERGE_TREE_FIND_RENAMES );

   lua_getfield( L, index, RENAME_THRESHOLD );
   if( lua_type( L, -1 ) == LUA_TNUMBER )
   {
      opts->rename_threshold = luaL_checkinteger( L, -1 );
   }

   lua_getfield( L, index, TARGET_LIMIT );
   if( lua_type( L, -1 ) == LUA_TNUMBER )
   {
      opts->target_limit = luaL_checkinteger( L, -1 );
   }
   //TODO similarity metric

   lua_getfield( L, index, FILE_FAVOR );
   const char * favor = luaL_optstring( L, -1, NULL );
   luagi_set_file_favor( &opts->file_favor, favor );

   return ret;
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

   git_merge_file_result out;

   if( git_merge_file( &out, &ancestor, &ours, &theirs, &opts ) )
   {
      return ltk_push_git_error( L );
   }

   int ret = push_merge_file_result( L, &out );
   git_merge_file_result_free( &out );
   return ret;
}

int luagi_merge_file_from_index( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_index_entry ancestor;
   luagi_check_index_entry( &ancestor, L, 2 );
   git_index_entry ours;
   luagi_check_index_entry( &ours, L, 3 );
   git_index_entry theirs;
   luagi_check_index_entry( &theirs, L, 4 );

   git_merge_file_options opts;
   luagi_merge_init_file_options( L, 5, &opts );
   git_merge_file_result out;
   if( git_merge_file_from_index( &out, *repo, &ancestor, &ours, &theirs, &opts ) )
   {
      return ltk_push_git_error( L );
   }
   int ret = push_merge_file_result( L, &out );
   git_merge_file_result_free( &out );
   return ret;
}

int luagi_merge_trees( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_tree **ancestor_tree = checktree_at( L, 2 );
   git_tree **our_tree = checktree_at( L, 3 );
   git_tree **their_tree = checktree_at( L, 4 );

   git_merge_options opts;
   luagi_merge_init_options( L, 5, &opts );
   
   git_index **index = lua_newuserdata( L, sizeof( git_index *) );
   if( git_merge_trees( index, *repo, *ancestor_tree, *our_tree, *their_tree, &opts ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_INDEX_FUNCS );
   return 1;
}

int luagi_merge_commits( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_commit **our = checkcommit_at( L, 2 );
   git_commit **their = checkcommit_at( L, 3 );
   git_merge_options opts;
   luaL_checktype( L, 4, LUA_TTABLE );
   luagi_merge_init_options( L, 4, &opts );
   
   git_index **index = lua_newuserdata( L, sizeof( git_index *) );
   if( git_merge_commits( index, *repo, *our, *their, &opts ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_INDEX_FUNCS );
   return 1;
}

int luagi_merge( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   luaL_checktype( L, 2, LUA_TTABLE );

   luaL_checktype( L, 3, LUA_TTABLE );
   luaL_checktype( L, 4, LUA_TTABLE );
   int len =  luaL_len( L, 2 );
   const git_merge_head *heads[ len ];
   fill_merge_heads( L, 2, heads, len );

   git_merge_options mopts;
   luagi_merge_init_options( L, 3, &mopts ); 
   git_checkout_options copts;
   luagi_parse_checkout_options( &copts, L, 4 ); 

   if( git_merge( *repo, heads, len, &mopts, &copts ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

