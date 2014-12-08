#include <git2/types.h>
#include <lua.h>
#include <lauxlib.h>
#include <string.h>
#include "tree.h"
#include "oid.h"
#include "defines.h"

#define checktreebuilder(L) \
      (git_treebuilder**) luaL_checkudata( L, 1, LUAGI_TREE_BUILDER_FUNCS )
/* treebuilder stuff */
int luagi_tree_builder_create( lua_State *L )
{
   git_tree *tree = NULL;
   if( lua_isuserdata( L, 1) )
   {
      tree = *checktree( L );
   }
   git_treebuilder** builder = (git_treebuilder**) lua_newuserdata( L, sizeof( git_treebuilder* ) );

   int ret = git_treebuilder_create( builder, tree );
   if( ret != 0 )
   {
      lua_pushnil( L );
      lua_pushfstring( L, "could not create builder -- %d", ret );
      return 2;
   }
   
   luaL_getmetatable( L, LUAGI_TREE_BUILDER_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_tree_builder_gc( lua_State *L )
{
   git_treebuilder** builder = checktreebuilder( L );
   git_treebuilder_free( *builder );
   return 0;
}
int luagi_tree_builder_clear( lua_State *L )
{
   git_treebuilder** builder = checktreebuilder( L );
   git_treebuilder_clear( *builder );
   return 0;
}
int luagi_tree_builder_get( lua_State *L )
{
   git_treebuilder** builder = checktreebuilder( L );
   const char* filename = luaL_checkstring( L, 2 );
   const git_tree_entry* entry = git_treebuilder_get( *builder, filename );
   if( entry == NULL )
   {
      lua_pushnil( L );
      lua_pushstring( L, "no result found" );
      return 2;
   }
   git_tree_entry** e = (git_tree_entry**) lua_newuserdata( L, sizeof( git_tree_entry*) );
   int ret = git_tree_entry_dup( e, entry );
   if( ret != 0 )
   {
      lua_pushnil( L );
      lua_pushfstring( L, "git_tree_entry_dup failed %d", ret );
      return 2;
   }
   luaL_getmetatable( L, LUAGI_TREE_ENTRY_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}
int luagi_tree_builder_insert( lua_State *L )
{
   git_treebuilder** builder = checktreebuilder( L );
   const char* filename = luaL_checkstring( L, 2 );
   git_oid oid;
   int ret = luagi_check_oid( &oid, L, 3 );
   if( ret != 0 )
   {
      lua_pushnil( L );
      lua_pushstring( L, "invalid ref" );
      return 2;
   }

   const char* filemode = luaL_checkstring( L, 4 );
   git_filemode_t mode = 0;
   if( strncmp( filemode, NEW, strlen( NEW ) ) == 0 ) 
   {
      mode = GIT_FILEMODE_NEW;
   } 
   else if( strncmp( filemode, TREE, strlen( TREE ) ) == 0 )
   {
      mode = GIT_FILEMODE_TREE;
   }
   else if( strncmp( filemode, BLOB, strlen( BLOB ) ) == 0 )
   {
      mode = GIT_FILEMODE_BLOB;
   }
   else if( strncmp( filemode, BLOB_EXECUTABLE, strlen( BLOB_EXECUTABLE ) ) == 0 )
   {
      mode = GIT_FILEMODE_BLOB_EXECUTABLE;
   }
   else if( strncmp( filemode, LINK, strlen( LINK ) ) == 0 )
   {
      mode = GIT_FILEMODE_LINK;
   }
   else if( strncmp( filemode, COMMIT, strlen( COMMIT ) ) == 0 )
   {
      mode = GIT_FILEMODE_COMMIT;
   }

   if( mode == 0 )
   {
      lua_pushnil( L );
      lua_pushstring( L, "invalid filemode" );
      return 2;
   }
   const git_tree_entry** entry = (const git_tree_entry**) lua_newuserdata( L, sizeof( git_tree_entry* ) );
   ret = git_treebuilder_insert( entry, *builder, filename, &oid, mode );
   if( ret != 0 )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_TREE_ENTRY_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}
int luagi_tree_builder_remove( lua_State *L )
{
   git_treebuilder** builder = checktreebuilder( L );
   const char* filename = luaL_checkstring( L, 2 );
   int ret = git_treebuilder_remove( *builder, filename );
   lua_pushinteger( L, ret );
   return 1;
}
int luagi_tree_builder_entry_count( lua_State *L )
{
   git_treebuilder** builder = checktreebuilder( L );
   lua_pushinteger( L, git_treebuilder_entrycount( *builder ) );
   return 1;
}
int luagi_tree_builder_write( lua_State *L )
{
   git_treebuilder** builder = checktreebuilder( L );
   git_repository** repo = checkrepo( L, 2 );
   git_oid oid;
   
   git_treebuilder_write( &oid, *repo, *builder );
   return luagi_push_oid( L, &oid );
}

struct filter_payload 
{
   lua_State *L;
   int function;
};

static int builder_filter( const git_tree_entry *entry, void *payload )
{
   struct filter_payload *fp = (struct filter_payload *) payload;
   if( entry == NULL )
   {
      luaL_error( fp->L, "no input entry" );
      return -1;
   }
   lua_pushvalue( fp->L, fp->function );
   git_tree_entry** e = (git_tree_entry**) lua_newuserdata( fp->L, sizeof( git_tree_entry* ) );
   
   if( git_tree_entry_dup( e, entry ))
   {
      luaL_error( fp->L, "dup failed" );
      return -1;
   }

   luaL_getmetatable( fp->L, LUAGI_TREE_ENTRY_FUNCS );
   lua_setmetatable( fp->L, -2 );

   if(lua_pcall( fp->L, 1, 1, 0) )
   {
      luaL_error( fp->L, "failure calling function" );
      return -1;
   }
   int ret = lua_toboolean( fp->L, -1 );
   lua_pop( fp->L, 1 );
   return ret;
}

int luagi_tree_builder_filter( lua_State *L )
{
   git_treebuilder** builder = checktreebuilder( L );
   if( lua_type( L, 2 ) != LUA_TFUNCTION )
   {
      luaL_error( L, "function( tree_entry entry ) parameter required" );
   }

   struct filter_payload *payload = (struct filter_payload *) malloc( sizeof( struct filter_payload ) );
   payload->L = L;
   payload->function = 2;

   git_treebuilder_filter( *builder, builder_filter, payload );

   free( payload );
   return 0;
}
