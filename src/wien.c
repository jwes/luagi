#include <lua.h>
#include <lauxlib.h>
#include <git2/repository.h>  
#include <git2/signature.h>   

#include <string.h>
#include "wien.h"
#include "common.h"
#include "branch.h"
#include "tree.h"
#include "commit.h"
#include "clone.h"
#include "remote.h"
#include "transport.h"
#include "status.h"
#include "diff.h"

static int lgit_open( lua_State *L )
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

static int lgit_gc( lua_State *L )
{
   git_repository **repo = checkrepo(L, 1);
   git_repository_free( *repo );
   return 0;
}


static const struct luaL_Reg repofuncs [] = {
   { "branch", lgit_create_branch },
   { "branches", lgit_branches }, 
   { "lookup_branch", lgit_branch_lookup }, 
   { "lookup_commit", lgit_commit_lookup },
   { "lookup_tree", lgit_tree_lookup }, 
   { "commit", lgit_commit_create }, 
   { "clone_into", lgit_clone_into },
   { "__gc", lgit_gc },
   //remotes
   { "remotes", lgit_remote_list },
   { "load_remote", lgit_remote_load },
   { "create_anon_remote", lgit_remote_create_anonymous },
   { "create_remote_with_fetch", lgit_remote_create_with_fetchspec },
   { "create_remote", lgit_remote_create },
   //transport
   { "transport_from_url",     lgit_transport_new   },
   { "create_local_transport", lgit_transport_local },
   { "create_dummy_transport", lgit_transport_dummy },
   { "create_smart_transport", lgit_transport_smart },
   //status
   { "status_for_each", lgit_status_foreach },
   { "status_for_each_ext", lgit_status_foreach_ext },
   { "status_file", lgit_status_file },
   { "status_list", lgit_status_list_new },
   { "status_ignored", lgit_status_should_ignore },
   //diff

   { "diff_tree_to_index", lgit_diff_tree_to_index },
   { "diff_index_to_workdir", lgit_diff_index_to_workdir },
   { "diff_tree_to_workdir",lgit_diff_tree_to_workdir },
   { "diff_tree_to_workdir_with_index", lgit_diff_tree_to_workdir_with_index },

   { NULL, NULL },
};

static const struct luaL_Reg mylib [] = {
   { "version", lgit_version },
   { "features", lgit_features },
   { "open", lgit_open },
   { "tree_builder", lgit_tree_builder_create },
   { "clone", lgit_clone },
   // remote
   { "is_valid_remote_name", lgit_remote_is_valid_name   },
   { "is_valid_remote_url", lgit_remote_valid_url },
   { "is_supported_remote_url", lgit_remote_supported_url },
   { NULL, NULL } /*sentinel*/
};

void setup_funcs( lua_State *L, const char *meta_name, const luaL_Reg *funcs )
{
   luaL_newmetatable( L, meta_name );
   lua_pushvalue( L, -1 );
   lua_setfield( L, -2, "__index");
   luaL_setfuncs( L, funcs, 0);
}

int luaopen_wien(lua_State *L)
{
   /* metatable for the branch iterator */
   luaL_newmetatable( L, LGIT_BRANCH_STATICS );
   lua_pushcfunction( L, lgit_branch_iter_gc);
   lua_setfield( L, -2, "__gc" );

   setup_funcs(L, LGIT_TREE_FUNCS, lgit_tree_funcs);
   setup_funcs(L, LGIT_TREE_ENTRY_FUNCS, lgit_tree_entry_funcs);
   setup_funcs(L, LGIT_TREE_BUILDER_FUNCS, lgit_tree_builder_funcs);
   setup_funcs(L, LGIT_BRANCH_FUNCS, lgit_branch_funcs);
   setup_funcs(L, LGIT_COMMIT_FUNCS, lgit_commit_funcs);
   setup_funcs(L, LGIT_REMOTE_FUNCS, lgit_remote_funcs);
   setup_funcs(L, LGIT_STATUS_FUNCS, lgit_status_funcs );
   setup_funcs(L, LGIT_DIFF_FUNCS, lgit_diff_funcs );
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
const char *lgit_otype_to_string( git_otype type )
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

int lgit_oid_fromstr( git_oid *oid, const char *ref )
{
   int len = strlen( ref );
   if ( len < GIT_OID_MINPREFIXLEN || len > GIT_OID_HEXSZ )
   {
      return -21;
   }
   return git_oid_fromstrp( oid, ref);
}

git_strarray lgit_strings_from_lua_list( lua_State *L, int table_idx )
{
   git_strarray array;
   array.count = luaL_len( L, table_idx );
   
   int size = 0;
   for( unsigned int i = 1; i <= array.count; i++ )
   {
      lua_pushinteger( L, i );
      lua_gettable( L, table_idx );
      const char* str = luaL_checkstring( L, -1 );
      size += strlen( str ) + 1;
   }
  
   char **strings = (char **) malloc( size );
   size_t pos = 0;
   for( unsigned int i = 1; i <= array.count; i++ )
   {
      lua_pushinteger( L, i );
      lua_gettable( L, table_idx );
      const char* str = luaL_checkstring( L, -1 );
      size_t len = strlen( str ) + 1;
      strncpy(strings[pos], str, len );
      pos += len;
   }

   array.strings = strings;
   return array;
}
