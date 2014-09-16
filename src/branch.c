#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <git2/errors.h>
#include <git2/refs.h>
#include <git2/signature.h>
#include <git2/commit.h>
#include <git2/buffer.h>
#include <git2/branch.h>
#include "branch.h"
#include "luagi.h"
#include "commit.h"

#include <stdio.h>

#define checkbranch(L) \
      (git_reference**) luaL_checkudata( L, 1, LUAGI_BRANCH_FUNCS )

int luagi_create_branch( lua_State *L )
{
   git_repository** repo = checkrepo(L, 1);
   const char* branch_name = luaL_checkstring(L, 2);
   git_commit** target = ( git_commit** ) luaL_checkudata( L, 3, LUAGI_COMMIT_FUNCS);
   int force = lua_toboolean(L, 4);
   git_signature sig;
   int ret = table_to_signature( L, &sig, 5 );
   if( ret != 0 )
   {
      luaL_error( L, "failed to create a signature %d", ret);
      return 0;
   }
   const char* log_message = luaL_optstring(L, 6, NULL);

   git_reference** out = (git_reference**) lua_newuserdata(L, sizeof(git_reference*) );

   ret = git_branch_create( out, *repo, branch_name, *target,
               force, &sig, log_message);
   if( ret != 0 )
   {
      lua_pushnil(L);
      lua_pushstring(L, "Could not create the branch " );
      return 2;
   }

   luaL_getmetatable( L, LUAGI_BRANCH_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;      
}

int luagi_delete_branch( lua_State *L )
{
   git_reference** out = checkbranch(L);
   int ret = git_branch_delete( *out );
   if( ret != 0 )
   {
      /* severe error */
      luaL_error( L, "couldn't delete branch %d", ret );
   }
   return 0;      
}

int luagi_move_branch( lua_State *L )
{
   git_reference** ref = checkbranch(L);
   const char* to_name = luaL_checkstring(L, 2);
   if(! lua_istable(L, 3) )
   {
      luaL_error( L, "signature is no table" );
      return 0;
   }
   /* signature type needed */
   const char* log_message = luaL_optstring(L, 4, NULL);
   int force = lua_toboolean(L, 5 );

   /* get username from table */
   lua_pushstring( L, "name");
   lua_gettable( L, 3 );
   const char* name = luaL_checkstring( L, -1);
   /* get email from table */
   lua_pushstring( L, "email");
   lua_gettable( L, 3 );
   const char* email = luaL_checkstring( L, -1 );

   git_signature* sig;
   int ret = git_signature_now( &sig, name, email );
   if( ret != 0 )
   {
      luaL_error( L, "failed to create a signature %d", ret);
      return 0;
   }

   git_reference** out = (git_reference**) lua_newuserdata(L, sizeof(git_reference*) );
   ret = git_branch_move( out, *ref, to_name, force, sig, log_message);
   git_signature_free( sig );
   if( ret != 0 )
   {
      if( ret == GIT_EEXISTS ){
         luaL_error( L, "moving failed, branch with name %s already exists, use force if you are sure what you are doing", to_name  );
      }
      else
      {
         luaL_error( L, "moving failed %d", ret );
      }
   }
   luaL_getmetatable( L, LUAGI_BRANCH_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;

}

static int branch_iter( lua_State *L);

int luagi_branches( lua_State *L )
{
   git_repository** repo = checkrepo(L, 1);
   const char* modus = luaL_checkstring(L, 2);
   if( modus == NULL )
   { 
      modus = "";
   }

   git_branch_t flags = GIT_BRANCH_LOCAL;
   if( *modus == 'a' )
   {
      flags = flags | GIT_BRANCH_REMOTE;
   }
   else if ( *modus == 'r' )
   {
      flags = GIT_BRANCH_REMOTE;
   }

   /* create iterator userdata and open iterator */
   git_branch_iterator** iter =(git_branch_iterator**) lua_newuserdata(L, sizeof(git_branch_iterator*));
   luaL_getmetatable(L, LUAGI_BRANCH_STATICS);
   lua_setmetatable(L, -2);

   int ret = git_branch_iterator_new( iter, *repo, flags);
   if( ret != 0 )
   { 
      luaL_error(L, "can't open iterator, error %d", ret);
   }

   lua_pushcclosure( L, branch_iter, 1 );
   return 1;      
}

static int branch_iter( lua_State *L )
{
   git_branch_iterator* iter = *(git_branch_iterator**) lua_touserdata( L, lua_upvalueindex( 1 ) );

   git_reference** out = (git_reference**) lua_newuserdata(L, sizeof(git_reference*) );
   git_branch_t type;
   int ret = git_branch_next( out, &type, iter );
   if( ret == 0 )
   {
      luaL_getmetatable( L, LUAGI_BRANCH_FUNCS );
      lua_setmetatable( L, -2 );

      lua_pushboolean(L, type == GIT_BRANCH_REMOTE );

      return 2;
   } 
   else if( ret == GIT_ITEROVER )
   {
      return 0;
   }
   else 
   {
      luaL_error(L, "iteration failed, error %d", ret);
      return 0;
   }
}

int luagi_branch_iter_gc( lua_State *L )
{
   git_branch_iterator** iter = (git_branch_iterator**) lua_touserdata( L, lua_upvalueindex( 1 ) );
   if( iter != NULL )
   {
      git_branch_iterator_free( *iter );
   }
   return 0;
}

int luagi_branch_upstream_get( lua_State *L )
{
   git_reference** ref = checkbranch(L);
   git_reference** out = (git_reference**) lua_newuserdata(L, sizeof(git_reference*) );
   int ret = git_branch_upstream( out, *ref );
   if( ret != 0 )
   {
      if( ret == GIT_ENOTFOUND )
      {
         lua_pushnil( L );
         lua_pushstring( L, "upstream not found");
         return 2;
      } 
      else 
      {
         luaL_error( L, "error getting upstream %d", ret);
         return 0;
      }
   }
   
   luaL_getmetatable( L, LUAGI_BRANCH_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_branch_upstream_set( lua_State *L )
{
   lua_pushnil( L );
   return 1;
}

int luagi_branch_lookup( lua_State *L )
{
   git_repository** repo = checkrepo( L, 1);
   const char* branch_name = luaL_checkstring( L, 2 );
   const char* btype = luaL_optstring( L, 3, "l" );
   git_branch_t type = GIT_BRANCH_LOCAL;
   if( *btype == 'r' )
   {
      type = GIT_BRANCH_REMOTE;
   }

   git_reference** out = (git_reference**) lua_newuserdata(L, sizeof(git_reference*) );
   int ret = git_branch_lookup( out, *repo, branch_name, type );
   if( ret != 0 )
   {
      if( ret == GIT_ENOTFOUND )
      {
         lua_pushnil( L );
         lua_pushfstring( L, "branch %s not found", branch_name );
         return 2;
      }
      else
      {
         luaL_error( L, "branch lookup failed %d", ret);
         return 0;
      }
   }
   luaL_getmetatable( L, LUAGI_BRANCH_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_branch_gc( lua_State *L )
{
   git_reference** ref = checkbranch(L);
   git_reference_free( *ref );
   return 0;
}

int luagi_branch_is_head( lua_State *L )
{
   git_reference** ref = checkbranch(L);
   int ret = git_branch_is_head( *ref );
   lua_pushboolean( L, ret );
   return 1;
}

int luagi_branch_name( lua_State *L )
{
   git_reference** ref = checkbranch(L);
   const char* out;
   int ret = git_branch_name( &out, *ref );
   if( ret != 0 )
   {
      luaL_error( L, "name failed %d", ret );
      return 0;
   }
   lua_pushstring( L, out );
   return 1;
}

