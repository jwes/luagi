#include "commit.h"

#include <git2/commit.h>
#include <git2/errors.h>
#include <git2/oid.h>
#include <git2/signature.h>
#include <lauxlib.h>
#include <stdio.h>

#include "ltk.h"
#include "luagi.h"
#include "oid.h"

int luagi_commit_lookup( lua_State *L )
{
   git_repository** repo = checkrepo( L, 1 );

   git_oid oid;
   int ret = luagi_check_oid( &oid, L, 2);
   if( ret != 0 )
   {
      lua_pushnil( L );
      lua_pushstring(L, "the given string is no valid git oid");
      return 2;
   }

   git_commit** out_commit = (git_commit**)lua_newuserdata( L, sizeof( git_commit* ));
   ret = git_commit_lookup( out_commit, *repo, &oid );
   if( ret != 0 )
   {
      lua_pushnil( L );
      lua_pushfstring( L, "lookup failed - %d", ret );
   }
   ltk_setmetatable( L, LUAGI_COMMIT_FUNCS );
   return 1;
}

int luagi_commit_gc( lua_State *L )
{
   git_commit** commit = checkcommit( L );
   git_commit_free( *commit );
   return 0;
}

int luagi_commit_id( lua_State *L )
{
   git_commit** commit = checkcommit( L );
   const git_oid* oid = git_commit_id( *commit );
   return luagi_push_oid( L, oid );
}

int luagi_commit_encoding( lua_State *L )
{
   git_commit** commit = checkcommit( L );
   lua_pushstring( L, git_commit_message_encoding( *commit ));
   return 1;
}

int luagi_commit_message( lua_State *L )
{
   git_commit** commit = checkcommit( L );
   lua_pushstring( L, git_commit_message( *commit ));
   return 1;
}

int luagi_commit_summary( lua_State *L )
{
   git_commit** commit = checkcommit( L );
   lua_pushstring( L, git_commit_summary( *commit ));
   return 1;
}

int luagi_commit_committer( lua_State *L )
{
   git_commit** commit = checkcommit( L );   
   const git_signature* sig = git_commit_committer( *commit );
   int ret = signature_to_table( L, sig );
   if( ret != 0 )
   {
      lua_pushnil( L );
      lua_pushfstring( L, "signature to table failed %d", ret );
      return 2;
   }
   /* signature table is on the stack */
   return 1;
}

int luagi_commit_author( lua_State *L )
{
   git_commit** commit = checkcommit( L );
   const git_signature* sig = git_commit_author( *commit );
   int ret = signature_to_table( L, sig );
   if( ret != 0 )
   {
      lua_pushnil( L );
      lua_pushfstring( L, "signature to table failed %d", ret );
      return 2;
   }
   /* signature table is on the stack */
   return 1;
}

int luagi_commit_tree( lua_State *L )
{
   git_commit** commit = checkcommit( L );
   git_tree** tree = (git_tree**) lua_newuserdata( L, sizeof( git_tree* ) );
   int ret = git_commit_tree( tree, *commit);
   if( ret != 0 )
   {
      lua_pushnil( L );
      lua_pushfstring( L, "can't find tree %d", ret );
      return 2;
   }

   ltk_setmetatable( L, LUAGI_TREE_FUNCS );
   return 1;
}

int luagi_commit_parentcount( lua_State *L )
{
   git_commit** commit = checkcommit( L );
   unsigned int count = git_commit_parentcount( *commit );
   lua_pushinteger( L, count );
   return 1;
}

int luagi_commit_parent( lua_State *L )
{
   git_commit** commit = checkcommit( L );
   unsigned int n = luaL_checkinteger( L, 2 );
   // lua to c
   n--;

   git_commit** parent = (git_commit**) lua_newuserdata( L, sizeof( git_commit* ) );
   int ret = git_commit_parent( parent, *commit, n );
   if( ret != 0 )
   {
      lua_pushnil( L );
      lua_pushfstring( L, "can't find parent %d: %d", n, ret );
      return 2;
   }
   ltk_setmetatable( L, LUAGI_COMMIT_FUNCS );
   return 1;
}

int luagi_commit_parent_id( lua_State *L )
{
   git_commit** commit = checkcommit( L );
   unsigned int n = luaL_checkinteger( L, 2 );
  
   //luas index starts with 1
   n--;

   const git_oid* oid = git_commit_parent_id( *commit, n );
   return luagi_push_oid( L, oid );
}

int luagi_commit_nth_gen_ancestor( lua_State *L )
{
   git_commit** commit = checkcommit( L );
   unsigned int n = luaL_checkinteger( L, 2 );
   //lua to c 
   n--;
   
   git_commit** anc = (git_commit**) lua_newuserdata( L, sizeof( git_commit* ) );

   int ret = git_commit_nth_gen_ancestor( anc, *commit, n );
   if( ret != 0 )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_COMMIT_FUNCS );
   return 1;
}

int luagi_commit_create( lua_State *L )
{
   git_repository** repo = checkrepo( L, 1 );
   git_signature *author;
   int ret = table_to_signature( L, &author, 2 );
   git_signature *committer; 
   ret = table_to_signature( L, &committer, 3 );
   const char* message = luaL_checkstring( L, 4 );
   git_tree** tree = (git_tree**) luaL_checkudata( L, 5, LUAGI_TREE_FUNCS );
   
   // table of parents
   luaL_checktype( L, 6, LUA_TTABLE );
   unsigned int n = luaL_len( L, 6 );
   const git_commit* parents[n];

   for( unsigned int i = 0; i < n; i++ )
   {
      lua_rawgeti( L, 6, i + 1 );
      parents[i] = *( (git_commit**) luaL_checkudata( L, -1, LUAGI_COMMIT_FUNCS ) );
   }
   lua_pop( L, n );

   const char* update_ref = luaL_optstring( L, 7, NULL );
   const char* encoding = luaL_optstring( L, 8, NULL );
   git_oid oid;
   ret = git_commit_create( &oid, *repo, update_ref, author,
                      committer, encoding, message, *tree,
                      n, parents );
   git_signature_free( author );
   git_signature_free( committer );
   if( ret != 0 )
   {
      return ltk_push_git_error( L );
   }
   return luagi_push_oid( L, &oid );
}
/*
 * Needs a table
 * supported field names:
 * update_ref
 * author
 * committer
 * message_encoding
 * message
 * tree
 */
int luagi_commit_amend( lua_State *L )
{
   git_commit** commit = checkcommit( L );

   luaL_checktype( L, 2, LUA_TTABLE);
   // since all elements can be null, we use a table...

   lua_getfield( L, 2, "update_ref");
   const char* update_ref = luaL_optstring( L, -1, NULL );
   git_signature* author = NULL;
   int ret = 0;
   lua_getfield( L, 2, "author" );
   if( lua_istable( L, -1 ) )
   {
      ret = table_to_signature( L, &author, -1 );
   }
   git_signature* committer = NULL;
   lua_getfield( L, 2, "committer" );
   if( lua_istable( L, -1 ) )
   {
      ret = table_to_signature( L, &committer, -1 );
   }

   lua_getfield( L, 2, "message_encoding" );
   const char* message_encoding = luaL_optstring( L, -1, NULL );

   lua_getfield( L, 2, "message" );
   const char* message = luaL_optstring( L, -1, NULL );
   
   lua_getfield( L, 2, "tree" );
   git_tree* tree = NULL;
   if( lua_isuserdata( L, -1 ) )
   {
      tree = *( ( git_tree** ) luaL_checkudata( L, -1, LUAGI_TREE_FUNCS ) );
   }

   // use the table and modify the commit
   git_oid outid;
   ret = git_commit_amend( &outid, *commit, update_ref, author, committer, message_encoding, message, tree );
   git_signature_free( author );
   git_signature_free( committer );
   if( ret != 0 )
   {
      lua_pushnil( L );
      lua_pushfstring( L, "commit amend failed %d", ret );
      return 2;
   }
   return luagi_push_oid( L, &outid );
}

