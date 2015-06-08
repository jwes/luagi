#include "annotated_commit.h"
#include "luagi.h"
#include "reference.h"
#include "ltk.h"
#include "oid.h"

void fill_annotated_commit( lua_State *L, int index, const git_annotated_commit **heads, int len )
{
   for( int i = 0; i < len; i++ )
   {
      lua_pushinteger( L, i + 1 );
      lua_gettable( L, index );
      heads[i] = *( check_annotated_commit_at( L, -1 ) );
      lua_pop( L, 1 );
   }
}

int luagi_annotated_commit_from_ref( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_reference **ref = check_ref_at( L, 2 );

   git_annotated_commit **head = lua_newuserdata( L, sizeof( git_annotated_commit *) );

   if( git_annotated_commit_from_ref( head, *repo, *ref ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_ANNOTATED_COMMIT_FUNCS );
   return 1;
}
   
int luagi_annotated_commit_from_fetchhead( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *branch_name = luaL_checkstring( L, 2 );
   const char *remote_url = luaL_checkstring( L, 3 );
   git_oid oid;
   if( luagi_check_oid( &oid, L, 4 ) )
   {
      ltk_error_abort( L );
      return 0;
   }

   git_annotated_commit **head = lua_newuserdata( L, sizeof( git_annotated_commit * ) );

   if( git_annotated_commit_from_fetchhead( head, *repo, branch_name, remote_url, &oid ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_ANNOTATED_COMMIT_FUNCS );
   return 1;
}

int luagi_annotated_commit_lookup( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid oid;
   if( luagi_check_oid( &oid, L, 2 ) )
   {
      ltk_error_abort( L );
      return 0;
   }

   git_annotated_commit **head = lua_newuserdata( L, sizeof( git_annotated_commit * ) );

   if( git_annotated_commit_lookup( head, *repo, &oid ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_ANNOTATED_COMMIT_FUNCS );
   return 1;
}

int luagi_annotated_commit_id( lua_State *L )
{
   git_annotated_commit **head = check_annotated_commit_at( L, 1 );
   const git_oid *out = git_annotated_commit_id( *head );
   return luagi_push_oid( L, out );
}

int luagi_annotated_commit_free( lua_State *L )
{
   git_annotated_commit **head = check_annotated_commit_at( L, 1 );
   git_annotated_commit_free( *head );
   return 0;
}

