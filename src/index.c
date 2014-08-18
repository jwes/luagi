#include "index.h"
#include <string.h>
#include <git2/errors.h>
#include "wien.h"

int lgit_index_new( lua_State *L )
{
   const char *path = luaL_optstring( L, 1, NULL );
   git_index **index = lua_newuserdata( L, sizeof( git_index *) ); 
   if( path == NULL )
   {
      if( git_index_new( index ) )
      {
         ERROR_PUSH( L )
      }
   }
   else 
   {
      if( git_index_open( index, path ) )
      {
         ERROR_PUSH( L )
      }
   }

   luaL_getmetatable(L, LGIT_INDEX_FUNCS);
   lua_setmetatable(L, -2);
   return 1;
}

int lgit_index_free( lua_State *L )
{ 
   git_index **index = checkindex_at( L, 1 );
   git_index_free( *index );
   return 0;
}

int lgit_index_owner( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_caps( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_set_caps( lua_State *L ){ lua_pushnil( L ); return 1; }

int lgit_index_read( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );

   int force = 0;
   if( lua_isboolean( L, 2 ) )
   {
      force = lua_toboolean( L, 2 );
   }

   if( git_index_read( *index, force ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int lgit_index_write( lua_State *L )
{
   git_index **index = checkindex_at(L, 1 );

   if( git_index_write( *index ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int lgit_index_path( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   lua_pushstring( L, git_index_path( *index ) );
   return 1;
}

int lgit_index_read_tree( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   git_tree **tree = checktree_at( L, 2 );

   if( git_index_read_tree( *index, *tree ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int lgit_index_write_tree( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   git_oid out;
   if( lua_isuserdata( L, 2 ) )
   {
      git_repository **repo = checkrepo( L, 2 );
      if( git_index_write_tree_to( &out, *index, *repo ) )
      {
         const git_error *err = giterr_last();
         lua_pushnil( L );
         lua_pushstring( L, err->message );
         return 2;
      }
   }
   else
   {
      if( git_index_write_tree( &out, *index ) )
      {
         ERROR_PUSH( L )
      }
   }
   char buf [ GIT_OID_HEXSZ + 1 ];
   lua_pushstring( L, git_oid_tostr( buf, sizeof(buf), &out ) );
   return 1;
}

int lgit_index_entrycount( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );

   lua_pushinteger( L, git_index_entrycount( *index ) );
   return 1;
}

int lgit_index_clear( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );

   if( git_index_clear( *index ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int lgit_index_get_byindex( lua_State *L )
{ 
   git_index **index = checkindex_at( L, 1 );
   int pos = luaL_checkinteger( L, 2 );

   git_index_entry *entry = lua_newuserdata( L, sizeof( git_index_entry ) );

   const git_index_entry *out = git_index_get_byindex( *index, pos );
   if( out == NULL )
   {
      ERROR_PUSH( L )
   }

   memcpy( entry, out, sizeof( git_index_entry ) );

   luaL_getmetatable(L, LGIT_INDEX_ENTRY_FUNCS);
   lua_setmetatable(L, -2);
   return 1;
}

int lgit_index_get_bypath( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   const char *path = luaL_checkstring( L, 2 );
   int stage = luaL_checkinteger( L, 3 );

   git_index_entry *entry = lua_newuserdata( L, sizeof( git_index_entry ) );

   const git_index_entry *out = git_index_get_bypath( *index, path, stage );
   if( out == NULL )
   {
      ERROR_PUSH( L )
   }

   memcpy( entry, out, sizeof( git_index_entry ) );

   luaL_getmetatable(L, LGIT_INDEX_ENTRY_FUNCS);
   lua_setmetatable(L, -2);
   return 1;
}
   
static int lgit_remove_func( lua_State *L, int (*func)( git_index *index, const char *path, int stage ) )
{
   git_index **index = checkindex_at( L, 1 );
   const char *path = luaL_checkstring( L, 2 );
   int stage = luaL_checkinteger( L, 3 );

   if( func( *index, path, stage ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int lgit_index_remove( lua_State *L )
{
  return lgit_remove_func( L, git_index_remove ); 
}

int lgit_index_remove_directory( lua_State *L )
{
  return lgit_remove_func( L, git_index_remove_directory ); 
}

int lgit_index_add( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   const git_index_entry *entry = checkindexentry_at( L, 2 );

   if( git_index_add( *index, entry ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int lgit_index_entry_stage( lua_State *L )
{
   const git_index_entry *entry = checkindexentry_at( L, 1 );

   lua_pushinteger( L, git_index_entry_stage( entry ) );
   return 1;
}

static int lgit_index_do_bypath( lua_State *L, int (*func)( git_index *index, const char *path) )
{
   git_index **index = checkindex_at( L, 1 );
   const char *path = luaL_checkstring( L, 2 );

   if( func( *index, path ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int lgit_index_add_bypath( lua_State *L )
{
   return lgit_index_do_bypath( L, git_index_add_bypath );
}

int lgit_index_remove_bypath( lua_State *L )
{
   return lgit_index_do_bypath( L, git_index_remove_bypath );
}

struct pathspec_payload
{
   lua_State *L;
   int function_pos;
};

static int index_matched_pathspec( const char *path, const char *matched_pathspec, void *payload )
{
   struct pathspec_payload *p = payload;
   lua_pushvalue( p->L, p->function_pos );
   lua_pushstring( p->L, path );
   lua_pushstring( p->L, matched_pathspec );

   if( lua_pcall( p->L, 2, 1, 0 ) != LUA_OK )
   {
      dumpStack( p->L );
      luaL_error( p->L, "can not call path callback" );
   }

   int ret = luaL_checkinteger( p->L, -1 );
   lua_pop( p->L, 1 );
   return ret;
}

int lgit_index_add_all( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   git_strarray array = lgit_strings_from_lua_list( L, 2 );

   struct pathspec_payload* p = malloc( sizeof( struct pathspec_payload ) );
   p->L = L;
   p->function_pos = 3;
   
   //TODO flags
   int flags = 0;

   if( git_index_add_all( *index, &array, flags, index_matched_pathspec, p ) )
   {
      ERROR_ABORT( L );
   }
   git_strarray_free( &array );
   free( p );
   return 0;
}


int lgit_index_remove_all( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   git_strarray array = lgit_strings_from_lua_list( L, 2 );

   struct pathspec_payload* p = malloc( sizeof( struct pathspec_payload ) );
   p->L = L;
   p->function_pos = 3;
   
   if( git_index_remove_all( *index, &array, index_matched_pathspec, p ) )
   {
      ERROR_ABORT( L );
   }
   git_strarray_free( &array );
   free( p );
   return 0;
} 

int lgit_index_update_all( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   git_strarray array = lgit_strings_from_lua_list( L, 2 );

   struct pathspec_payload* p = malloc( sizeof( struct pathspec_payload ) );
   p->L = L;
   p->function_pos = 3;
   
   if( git_index_update_all( *index, &array, index_matched_pathspec, p ) )
   {
      ERROR_ABORT( L );
   }
   git_strarray_free( &array );
   free( p );
   return 0;
}

int lgit_index_find( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   const char *path = luaL_checkstring( L, 2 );

   lua_pushinteger( L, git_index_find( NULL, *index, path ) );
   return 1;
}

int lgit_index_conflict_add( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   const git_index_entry *ancestor_entry = checkindexentry_at( L, 2 );
   const git_index_entry *our_entry = checkindexentry_at( L, 2 );
   const git_index_entry *their_entry = checkindexentry_at( L, 2 );

   if( git_index_conflict_add( *index, ancestor_entry, our_entry, their_entry ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int lgit_index_conflict_get( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   const char *path = luaL_checkstring( L, 2 );

   const git_index_entry *ancestor_out = lua_newuserdata( L, sizeof( git_index_entry ) );
   const git_index_entry *our_out = lua_newuserdata( L, sizeof( git_index_entry ) );
   const git_index_entry *their_out = lua_newuserdata( L, sizeof( git_index_entry ) );

   if( git_index_conflict_get( &ancestor_out, &our_out, &their_out, *index, path ) )
   {
      const git_error *err = giterr_last();
      luaL_error( L, err->message );
      return 0;
   }

   luaL_getmetatable(L, LGIT_INDEX_ENTRY_FUNCS);
   lua_setmetatable(L, -2);
   lua_setmetatable(L, -3);
   lua_setmetatable(L, -4);
   return 3;
}

int lgit_index_conflict_remove( lua_State *L )
{
   return lgit_index_do_bypath( L, git_index_conflict_remove );
}

int lgit_index_conflict_cleanup( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   if( git_index_conflict_cleanup( *index ) )
   {
      const git_error *err = giterr_last();
      luaL_error( L, err->message );
   }
   return 0;
}

int lgit_index_has_conflicts( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );

   lua_pushboolean( L, git_index_has_conflicts( *index ) );
   return 1;
}
