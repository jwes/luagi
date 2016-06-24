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

#include <git2/oid.h>
#include <git2/errors.h>
#include <stdio.h>
#define __USE_BSD
#include <string.h>
#undef __USE_BSD

#include "ltk.h"
#include "luagi.h"
#include "object.h"
#include "oid.h"
#include "types.h"

/*#define checktree(L) \
      (git_tree**) luaL_checkudata( L, 1, LUAGI_TREE_FUNCS )
*/
int luagi_tree_lookup( lua_State *L )
{
   git_repository** repo = checkrepo( L, 1 );

   git_oid oid;
   int len = 0;
   int ret = luagi_check_oid_prefix( &oid, &len, L, 2 );
   if( ret != 0 )
   {
      lua_pushnil( L );
      lua_pushstring(L, "the given string is no valid git oid");
      return 2;
   }

   git_tree** out = (git_tree**)lua_newuserdata( L, sizeof(git_tree*) );

   if( len == GIT_OID_HEXSZ )
   {
      ret = git_tree_lookup( out, *repo, &oid );
   } 
   else
   {
      ret = git_tree_lookup_prefix( out, *repo, &oid, len );
   }

   if( ret )
   {
      const git_error* err = giterr_last();
      lua_pushnil( L );
      lua_pushfstring(L, "failed to lookup tree: %s", err->message );
      return 2;
   }

   ltk_setmetatable( L, LUAGI_TREE_FUNCS );
   return 1;
}

int luagi_tree_id( lua_State *L )
{
   git_tree** tree = checktree( L );
   const git_oid* oid = git_tree_id( *tree );
   return luagi_push_oid( L, oid );
}

int luagi_tree_entrycount( lua_State *L )
{
   git_tree** tree = checktree( L );
   size_t count = git_tree_entrycount( *tree );
   lua_pushinteger( L, count );
   return 1;
}

int luagi_tree_gc( lua_State *L )
{
   git_tree** tree = checktree( L );
   git_tree_free( *tree );
   return 0;
}

/* tree entry genereation */
static int tree_entry_by_final( lua_State *L, const git_tree_entry* entry )
{
   git_tree_entry** e = (git_tree_entry**) lua_newuserdata(L, sizeof(git_tree_entry*) );

   int ret = git_tree_entry_dup( e, entry );
   if( ret != 0 )
   {
      lua_pushnil( L );
      lua_pushfstring( L, "dup failed, %d", ret );
      return 2;
   }
   ltk_setmetatable( L, LUAGI_TREE_ENTRY_FUNCS );
   return 1;
}

int luagi_tree_entry_byname( lua_State *L )
{
   git_tree** tree = checktree( L );
   const char* filename = luaL_checkstring( L , 2 );

   const git_tree_entry* e = git_tree_entry_byname( *tree, filename );
   if( e == NULL )
   {
      lua_pushnil( L );
      lua_pushfstring( L, "file: %s not found", filename );
      return 2;
   }
   return tree_entry_by_final( L, e );
}

int luagi_tree_entry_byindex( lua_State *L )
{
   git_tree** tree = checktree( L );
   unsigned int index = luaL_checkinteger( L , 2 );
   // indicies in lua are 1 based
   index--;

   const git_tree_entry* e = git_tree_entry_byindex( *tree, index );
   if( e == NULL )
   {
      lua_pushnil( L );
      lua_pushfstring( L, "file: index %d not found", index );
      return 2;
   }
   return tree_entry_by_final(L, e );
}

int luagi_tree_entry_byid( lua_State *L )
{
   git_tree** tree = checktree( L );
   git_oid oid;
   int ret = luagi_check_oid( &oid, L, 2 );
   if( ret != 0 )
   {
      return ltk_push_git_error( L );
   }

   const git_tree_entry* e = git_tree_entry_byid( *tree, &oid );
   if( e == NULL )
   {
      return ltk_push_git_error( L );
   }
   return tree_entry_by_final(L, e );
}

/* user owned tree entries */
int luagi_tree_entry_bypath( lua_State *L )
{
   git_tree** tree = checktree( L );
   const char* path = luaL_checkstring( L , 2 );

   git_tree_entry** entry = (git_tree_entry**) lua_newuserdata(L, sizeof(git_tree_entry*) );

   int ret = git_tree_entry_bypath( entry, *tree, path );
   if( ret != 0 )
   {
      lua_pushnil( L );
      if( ret == GIT_ENOTFOUND )
      {
         lua_pushfstring( L, "file: %s not found", path );
      }
      else 
      {
         lua_pushfstring( L, "unknown error bypath %d", ret );
      }
      return 2;
   }
   ltk_setmetatable( L, LUAGI_TREE_ENTRY_FUNCS );
   return 1;
}

int luagi_tree_entry_gc( lua_State *L )
{
   git_tree_entry** entry = checktreeentry( L, 1 );
   git_tree_entry_free( *entry );
   return 0;
}

/* entry stuff */
int luagi_tree_entry_name( lua_State *L )
{
   git_tree_entry** entry = checktreeentry( L, 1 );
   lua_pushstring( L, git_tree_entry_name( *entry ));
   return 1;
}
int luagi_tree_entry_id( lua_State *L )
{
   git_tree_entry** entry = checktreeentry( L, 1 );   
   const git_oid* oid = git_tree_entry_id( *entry );

   return luagi_push_oid( L, oid );
   
}
int luagi_tree_entry_type( lua_State *L )
{
   git_tree_entry** entry = checktreeentry( L, 1 );
   git_otype type = git_tree_entry_type( *entry );
   lua_pushstring( L, luagi_string_from_otype( type ) );
   return 1;
}
int luagi_tree_entry_filemode( lua_State *L )
{
   git_tree_entry** entry = checktreeentry( L, 1 );
   git_filemode_t filemode =  git_tree_entry_filemode( *entry );
   char array[10];
   int ret = snprintf( array, 10, "%o", filemode );
   if( ret < 0)
   {
      lua_pushnil( L );
   }
   else
   {
      lua_pushstring( L, array );
   }
   return 1;
}

int luagi_tree_entry_cmp( lua_State *L )
{
   git_tree_entry** e1 = checktreeentry( L, 1 );   
   git_tree_entry** e2 = checktreeentry( L, 2 );   
   lua_pushinteger( L, git_tree_entry_cmp( *e1, *e2 ));
   return 1;
}

int luagi_tree_entry_to_object( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_tree_entry **entry = checktreeentry( L, 2 );

   git_object **out = lua_newuserdata( L, sizeof( git_object *) );
   if( git_tree_entry_to_object( out, *repo, *entry ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_OBJECT_FUNCS );
   return 1;
}

struct walk_info_element {
   char *root;
   git_tree_entry *entry;
   struct walk_info_element *next;
};

struct walk_info {
   struct walk_info_element *next;
   struct walk_info_element *last;
   git_tree **tree;
   git_treewalk_mode mode;
};

static void free_elem( struct walk_info_element *elem )
{
   free( elem->root );
   git_tree_entry_free( elem->entry );
   free( elem );
}

static int treewalk_callback( const char *root, const git_tree_entry *entry,
            void *payload)
{
   struct walk_info* info = (struct walk_info*) payload;

   struct walk_info_element *elem = (struct walk_info_element*) malloc( sizeof( struct walk_info_element ));
   elem->root = strdup( root );
   elem->next = NULL;
   int ret = git_tree_entry_dup(&elem->entry, entry );
   if( ret != 0 )
   {
      return -1;
   }
   if( info->last == NULL )
   {
      info->next = elem;
      info->last = elem;
   }
   else
   {
      info->last->next = elem;
      info->last = elem;
   }
   return 1;
}

static int luagi_walker( lua_State *L );

int luagi_tree_walk( lua_State *L )
{
   git_tree** tree = checktree( L );
   const char* direction = luaL_optstring( L, 2, LUAGI_TREE_PRE );
   git_treewalk_mode mode = GIT_TREEWALK_PRE;
   if( strncmp( direction, LUAGI_TREE_PRE, 3 ) == 0 )
   {
      mode = GIT_TREEWALK_PRE;
   }
   else if ( strncmp( direction, LUAGI_TREE_POST, 4 ) == 0 )
   {
      mode = GIT_TREEWALK_POST;
   } else {
      luaL_error( L, "wrong order, user 'pre' or 'post'" );
   }
   struct walk_info* info = (struct walk_info*) lua_newuserdata(L, sizeof(struct walk_info ) );
   ltk_setmetatable( L, LUAGI_TREE_WALK_STATICS );
   
   info->next = NULL;
   info->last = NULL;
   info->mode = mode;
   info->tree = tree;

   git_tree_walk( *info->tree, info->mode, &treewalk_callback, info );

   // TODO
   // start thread, that does the actual walking
   // create semaphore, which allows waiting for walkresults.
   // read in walker from semaphore and than from info
   // write to info in the thread
   lua_pushcclosure( L, luagi_walker, 1 );
   return 1;
}

static int luagi_walker( lua_State *L )
{
   struct walk_info* info = (struct walk_info*) lua_touserdata( L, lua_upvalueindex( 1 ) );
   struct walk_info_element *elem;
   if ( info->next != NULL )
   {
      elem = info->next;
      info->next = elem->next;
      lua_pushstring( L, elem->root );
      git_tree_entry** e = (git_tree_entry**) lua_newuserdata( L, sizeof( git_tree_entry* ) );
      int ret = git_tree_entry_dup( e, elem->entry );
      if( ret != 0 )
      {
         lua_pushnil( L );
         lua_pushstring( L, "out of memory - dup failed" );
      }  
      
      ltk_setmetatable( L, LUAGI_TREE_ENTRY_FUNCS );
      free_elem( elem );
      return 2;
   }//iteration finished
   return 0;
}
