/*
 * MIT License
 * 
 * Copyright (c) 2016 Johannes Westhuis, Alexander Schulz
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "tree.h"

#include <git2/types.h>
#include <lauxlib.h>
#include <lua.h>
#include <string.h>

#include "defines.h"
#include "ltk.h"
#include "oid.h"

#define checktreebuilder(L) \
      (git_treebuilder**) luaL_checkudata( L, 1, LUAGI_TREE_BUILDER_FUNCS )
/* treebuilder stuff */
int luagi_tree_builder_new( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_tree *tree = NULL;
   if( lua_isuserdata( L, 2) )
   {
      tree = *checktree_at( L, 2 );
   }
   git_treebuilder** builder = lua_newuserdata( L, sizeof( git_treebuilder* ) );

   int ret = git_treebuilder_new( builder, *repo, tree );
   if( ret )
   {
      return ltk_push_git_error( L );
   }
   
   ltk_setmetatable( L, LUAGI_TREE_BUILDER_FUNCS );
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
   ltk_setmetatable( L, LUAGI_TREE_ENTRY_FUNCS );
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
   if( strncmp( filemode, TREE, strlen( TREE ) ) == 0 )
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
   git_tree_entry** entry = (git_tree_entry**) lua_newuserdata( L, sizeof( git_tree_entry* ) );
   const git_tree_entry *e;
   ret = git_treebuilder_insert( &e, *builder, filename, &oid, mode );
   if( ret != 0 )
   {
      return ltk_push_git_error( L );
   }
   ret = git_tree_entry_dup( entry, e );
   if( ret != 0 )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_TREE_ENTRY_FUNCS );
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
   git_oid oid;
   
   git_treebuilder_write( &oid, *builder );
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

   ltk_setmetatable( fp->L, LUAGI_TREE_ENTRY_FUNCS );

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
