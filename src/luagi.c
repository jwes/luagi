#include <lua.h>
#include <lauxlib.h>
#include <git2/repository.h>  
#include <git2/signature.h>   

#include <string.h>
#include "luagi.h"
#include "common.h"
#include "branch.h"
#include "tree.h"
#include "commit.h"
#include "clone.h"
#include "remote.h"
#include "transport.h"
#include "status.h"
#include "diff.h"
#include "index.h"
#include "object.h"
#include "checkout.h"
#include "stash.h"
#include "reference.h"
#include "merge.h"
#include "blob.h"
#include "cherrypick.h"
#include "push.h"
#include "reset.h"
#include "revert.h"
#include "tag.h"

static int luagi_open( lua_State *L )
{
   git_repository **repo;

   const char *path = luaL_checkstring( L, 1 );
   repo = (git_repository **) lua_newuserdata(L, sizeof(git_repository *) );
   
   const int ret = git_repository_open( repo, path );

   if( ret < 0 )
   {
      lua_pushnil(L);
      lua_pushnumber(L, ret);
      return 2;
   }

   luaL_getmetatable( L, REPO_NAME );
   lua_setmetatable( L, -2 );
   return 1;
}

static int luagi_gc( lua_State *L )
{
   git_repository **repo = checkrepo(L, 1);
   git_repository_free( *repo );
   return 0;
}

static const struct luaL_Reg repofuncs [] = {
   { "branch", luagi_create_branch },
   { "branches", luagi_branches }, 
   { "lookup_branch", luagi_branch_lookup }, 
   { "lookup_commit", luagi_commit_lookup },
   { "lookup_tree", luagi_tree_lookup }, 
   { "commit", luagi_commit_create }, 
   { "clone_into", luagi_clone_into },
   { "__gc", luagi_gc },
   //remotes
   { "remotes", luagi_remote_list },
   { "load_remote", luagi_remote_load },
   { "create_anon_remote", luagi_remote_create_anonymous },
   { "create_remote_with_fetch", luagi_remote_create_with_fetchspec },
   { "create_remote", luagi_remote_create },
   //transport
   { "transport_from_url",     luagi_transport_new   },
   { "create_local_transport", luagi_transport_local },
   { "create_dummy_transport", luagi_transport_dummy },
   { "create_smart_transport", luagi_transport_smart },
   //status
   { "status_for_each", luagi_status_foreach },
   { "status_for_each_ext", luagi_status_foreach_ext },
   { "status_file", luagi_status_file },
   { "status_list", luagi_status_list_new },
   { "status_ignored", luagi_status_should_ignore },
   //diff

   { "diff_tree_to_index", luagi_diff_tree_to_index },
   { "diff_tree_to_tree", luagi_diff_tree_to_tree },
   { "diff_index_to_workdir", luagi_diff_index_to_workdir },
   { "diff_tree_to_workdir",luagi_diff_tree_to_workdir },
   { "diff_tree_to_workdir_with_index", luagi_diff_tree_to_workdir_with_index },
   { "diff_commit_as_email", luagi_diff_commit_as_email },

   { "index", luagi_repository_index },
   //treeentry
   { "treeentry_to_object", luagi_tree_entry_to_object },
   //checkout
   { "checkout_head", luagi_checkout_head },
   { "checkout_index", luagi_checkout_index },
   { "checkout_tree", luagi_checkout_tree },

   //stash
   { "stash", luagi_stash_save },
   { "stash_for_each", luagi_stash_foreach },
   { "stash_drop", luagi_stash_drop },

   //reference
   { "create_reference", luagi_reference_create },
   { "create_reference_matching", luagi_reference_create_matching },
   { "create_symbolic_reference", luagi_reference_symbolic_create },
   { "create_symbolic_reference_matching", luagi_reference_symbolic_create_matching },
   { "lookup_reference", luagi_reference_lookup },
   { "reference_name_to_id", luagi_reference_name_to_id },
   { "reference_dwim", luagi_reference_dwim },
   { "remove_reference", luagi_reference_remove },
   { "list_references", luagi_reference_list },
   { "foreach_reference", luagi_reference_foreach },
   { "foreach_reference_with_glob", luagi_reference_foreach_glob },
   { "iterate_references", luagi_reference_iterator },
   { "reference_has_log", luagi_reference_has_log },
   { "reference_ensure_log", luagi_reference_ensure_log },

   //merge
   { "merge_base", luagi_merge_base },
   { "merge_base_many", luagi_merge_base_many },
   { "merge_base_octopus", luagi_merge_base_octopus },
   { "merge_head_from_ref", luagi_merge_head_from_ref },
   { "merge_head_from_fetchhead", luagi_merge_head_from_fetchhead },
   { "merge_head_from_id", luagi_merge_head_from_id },
   { "merge_trees", luagi_merge_trees },
   { "merge_commits", luagi_merge_commits },
   { "merge", luagi_merge },
   { "merge_file_from_index", luagi_merge_file_from_index },
   { "merge_analysis", luagi_merge_analysis },
   // blob
   { "lookup_blob", luagi_blob_lookup },
   { "lookup_blob_prefix", luagi_blob_lookup_prefix },
   { "create_blob_from_workdir", luagi_blob_create_fromworkdir },
   { "create_blob_from_disk", luagi_blob_create_fromdisk },
   { "create_blob_from_chunks", luagi_blob_create_fromchunks },
   { "create_blob_from_buffer", luagi_blob_create_frombuffer },
   // cherrypick
   { "cherry_pick", luagi_cherry_pick },
   { "cherry_pick_commit", luagi_cherry_pick_commit },
   //push
   { "push", luagi_push_new },

   //reset
   { "reset", luagi_reset },
   { "reset_default", luagi_reset_default },
   //revert
   { "revert", luagi_revert },
   { "revert_commit", luagi_revert_commit },
   //tag
   { "lookup_tag", luagi_tag_lookup },
   { "lookup_tag_prefix", luagi_tag_lookup_prefix },
   { "create_tag", luagi_tag_create },
   { "create_tag_annotation", luagi_tag_annotation_create },
   { "create_tag_from_buffer", luagi_tag_create_frombuffer },
   { "create_tag_lightweight", luagi_tag_create_lightweight },
   { "delete_tag", luagi_tag_delete },

   { "list_tags", luagi_tag_list },
   { "list_tags_matching", luagi_tag_list_match },
   { "foreach_tag", luagi_tag_foreach },


   { NULL, NULL },
};

static const struct luaL_Reg mylib [] = {
   { "version", luagi_version },
   { "features", luagi_features },
   { "open", luagi_open },
   { "tree_builder", luagi_tree_builder_create },
   { "clone", luagi_clone },
   // remote
   { "is_valid_remote_name", luagi_remote_is_valid_name   },
   { "is_valid_remote_url", luagi_remote_valid_url },
   { "is_supported_remote_url", luagi_remote_supported_url },
   //index if path is given, it does open
   { "create_index", luagi_index_new },
   { "reference_normalize_name", luagi_reference_normalize_name },
   { "reference_is_valid_name", luagi_reference_is_valid_name },
   { "merge_files", luagi_merge_file },
   { NULL, NULL } /*sentinel*/
};

void setup_funcs( lua_State *L, const char *meta_name, const luaL_Reg *funcs )
{
   luaL_newmetatable( L, meta_name );
   lua_pushvalue( L, -1 );
   lua_setfield( L, -2, "__index");
   luaL_setfuncs( L, funcs, 0);
}
static int luagi_strarray_at( lua_State *L )
{
   git_strarray *array = luaL_checkudata( L, 1, LUAGI_STRARRAY );
   unsigned int index = luaL_checkinteger( L, 2 );
   //
   // lua indices start with 1
   if( index > array->count || index <= 0 )
   {
      lua_pushnil( L );
      lua_pushstring( L, "index out of bounds" );
      return 2;
   }
   
   lua_pushstring( L, array->strings[index] );
   return 1;
}

static int luagi_strarray_free( lua_State *L )
{
   git_strarray *array = luaL_checkudata( L, 1, LUAGI_STRARRAY );
   git_strarray_free( array );
   return 0;
}
int luaopen_luagi(lua_State *L)
{
   /* metatable for the branch iterator */
   luaL_newmetatable( L, LUAGI_BRANCH_STATICS );
   lua_pushcfunction( L, luagi_branch_iter_gc);
   lua_setfield( L, -2, "__gc" );

   luaL_newmetatable( L, LUAGI_STRARRAY );
   lua_pushcfunction( L, luagi_strarray_at );
   lua_setfield( L, -2, "__index" );
   lua_pushcfunction( L, luagi_strarray_free );
   lua_setfield( L, -2, "__gc" );

   setup_funcs(L, LUAGI_TREE_FUNCS, luagi_tree_funcs);
   setup_funcs(L, LUAGI_TREE_ENTRY_FUNCS, luagi_tree_entry_funcs);
   setup_funcs(L, LUAGI_TREE_BUILDER_FUNCS, luagi_tree_builder_funcs);
   setup_funcs(L, LUAGI_BRANCH_FUNCS, luagi_branch_funcs);
   setup_funcs(L, LUAGI_COMMIT_FUNCS, luagi_commit_funcs);
   setup_funcs(L, LUAGI_REMOTE_FUNCS, luagi_remote_funcs);
   setup_funcs(L, LUAGI_STATUS_FUNCS, luagi_status_funcs );
   setup_funcs(L, LUAGI_DIFF_FUNCS, luagi_diff_funcs );
   setup_funcs(L, LUAGI_DIFF_STATS_FUNCS, luagi_diff_stats_funcs );
   setup_funcs(L, LUAGI_INDEX_FUNCS, luagi_index_funcs );
   setup_funcs(L, LUAGI_INDEX_ENTRY_FUNCS, luagi_index_entry_funcs );
   setup_funcs(L, LUAGI_INDEX_CONFLICT_FUNCS, luagi_index_conflict_funcs );
   setup_funcs(L, LUAGI_OBJECT_FUNCS, luagi_object_funcs );
   setup_funcs(L, LUAGI_REFERENCE_FUNCS, luagi_reference_funcs );
   setup_funcs(L, LUAGI_REFERENCE_ITER_FUNCS, luagi_reference_iter_funcs );
   setup_funcs(L, LUAGI_MERGEHEAD_FUNCS, luagi_mergehead_funcs );
   setup_funcs(L, LUAGI_MERGERESULT_FUNCS, luagi_mergeresult_funcs );
   setup_funcs(L, LUAGI_BLOB_FUNCS, luagi_blob_funcs );
   setup_funcs(L, LUAGI_PUSH_FUNCS, luagi_push_funcs );
   setup_funcs(L, LUAGI_TAG_FUNCS, luagi_tag_funcs );
   setup_funcs(L, REPO_NAME, repofuncs);

   luaL_newlib( L, mylib );
   return 1;
}

int signature_to_table( lua_State *L, const git_signature *sig )
{
   lua_newtable( L );
   lua_pushstring( L, sig->name );
   lua_setfield( L, -2, SIG_NAME );
   lua_pushstring( L, sig->email );
   lua_setfield( L, -2, SIG_EMAIL );
   git_time time = sig->when;

   lua_pushnumber( L, time.time );
   lua_setfield( L, -2, SIG_TIME );

   lua_pushnumber( L, time.offset );
   lua_setfield( L, -2, SIG_TIME_OFF );
   return 0;
}
int table_to_signature( lua_State *L, git_signature *sig, int tablepos )
{
   lua_getfield( L, tablepos, SIG_NAME );
   const char *name = luaL_checkstring( L, -1 );
   lua_getfield( L, tablepos, SIG_EMAIL );
   const char *email = luaL_checkstring( L, -1 );
   lua_getfield( L, tablepos, SIG_TIME );
   git_time_t time = (git_time_t ) lua_tointeger( L, -1 );
   lua_getfield( L, tablepos, SIG_TIME_OFF );
   int offset = lua_tointeger( L, -1 );

   if( time == 0 )
   {
      return git_signature_now( &sig, name, email );
   }
   else 
   {
      return git_signature_new( &sig, name, email, time, offset );
   }
}
const char *luagi_otype_to_string( git_otype type )
{
   switch( type )
   {
      case GIT_OBJ_ANY:
      default:
         return "any";
      case GIT_OBJ_BAD:
         return "invalid";
      case GIT_OBJ__EXT1:
      case GIT_OBJ__EXT2:
         return "reserved for future use";
      case GIT_OBJ_COMMIT:
         return "commit";
      case GIT_OBJ_TREE:
         return "tree";
      case GIT_OBJ_BLOB:
         return "blob";
      case GIT_OBJ_TAG:
         return "tag";
      case GIT_OBJ_OFS_DELTA:
         return "ofs delta";
      case GIT_OBJ_REF_DELTA:
         return "ref delta";
   }
}

git_strarray luagi_strings_from_lua_list( lua_State *L, int table_idx )
{
   git_strarray array;
   array.count = luaL_len( L, table_idx );
   
   array.strings = calloc( array.count, sizeof( char * ) );
   for( size_t i = 1; i <= array.count; i++ )
   {
      lua_pushinteger( L, i );
      lua_gettable( L, table_idx );
      const char* str = luaL_checkstring( L, -1 );
      size_t len = strlen( str ) + 1;
      char* tmpStr = malloc( len );
      strncpy( tmpStr, str, len );
      array.strings[ i - 1 ] = tmpStr;
   }

   return array;
}

void luagi_lua_list_from_string( lua_State *L, git_strarray *array )
{
   lua_newtable( L );
   for( size_t i = 1; i <= array->count; i++ )
   {
      lua_pushstring( L, array->strings[i-1]);
      lua_pushinteger( L, i );
      lua_settable( L, -3 );
   }
}

void dumpStack( lua_State *L )
{
   int i;
   int top = lua_gettop(L); /* depth of the stack */
   for (i = 1; i <= top; i++) { /* repeat for each level */
      int t = lua_type(L, i);
      switch (t) {
         case LUA_TSTRING: { /* strings */
            printf("'%s'", lua_tostring(L, i));
            break;
         }
         case LUA_TBOOLEAN: { /* booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;
         }
         case LUA_TNUMBER: { /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;
         }
         default: { /* other values */
            printf("%s", lua_typename(L, t));
            break;
         }
      }
      printf(" "); /* put a separator */
   }
   printf("\n"); /* end the listing */
}
