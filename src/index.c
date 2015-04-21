#include "index.h"
#include <string.h>
#include <git2/errors.h>
#include <git2/repository.h>
#include "luagi.h"
#include "oid.h"

int luagi_index_new( lua_State *L )
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

   luaL_getmetatable(L, LUAGI_INDEX_FUNCS);
   lua_setmetatable(L, -2);
   return 1;
}

int luagi_index_free( lua_State *L )
{ 
   git_index **index = checkindex_at( L, 1 );
   git_index_free( *index );
   return 0;
}

int luagi_index_owner( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   git_repository **repo = lua_newuserdata( L, sizeof( git_repository *) );
   *repo = git_index_owner( *index );
   if( *repo == NULL )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable(L, REPO_NAME );
   lua_setmetatable(L, -2);
   return 1;
}

int luagi_index_caps( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   int caps = git_index_caps( *index );

   lua_newtable(L);
   lua_pushboolean( L, caps & GIT_INDEXCAP_IGNORE_CASE );
   lua_setfield( L, -2, IGNORE_CASE );

   lua_pushboolean( L, caps & GIT_INDEXCAP_NO_FILEMODE );
   lua_setfield( L, -2, NO_FILEMODE );

   lua_pushboolean( L, caps & GIT_INDEXCAP_NO_SYMLINKS );
   lua_setfield( L, -2, NO_SYMLINKS );

   lua_pushboolean( L, caps & GIT_INDEXCAP_FROM_OWNER );
   lua_setfield( L, -2, FROM_OWNER );
   return 1;
}

int luagi_index_set_caps( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );

   if( ! lua_istable( L, 2 ) )
   {
      ERROR_ABORT( L )
      return 0;
   }
   int caps = 0;
   lua_getfield( L, 2, IGNORE_CASE );
   if( lua_toboolean( L, -1 ) )
   {
      caps |= GIT_INDEXCAP_IGNORE_CASE;
   }
   lua_pop( L, 1 );

   lua_getfield( L, 2, NO_FILEMODE );
   if( lua_toboolean( L, -1 ) )
   {
      caps |= GIT_INDEXCAP_NO_FILEMODE;
   }
   lua_pop( L, 1 );

   lua_getfield( L, 2, NO_SYMLINKS );
   if( lua_toboolean( L, -1 ) )
   {
      caps |= GIT_INDEXCAP_NO_SYMLINKS;
   }
   lua_pop( L, 1 );

   lua_getfield( L, 2, FROM_OWNER );
   if( lua_toboolean( L, -1 ) )
   {
      caps |= GIT_INDEXCAP_FROM_OWNER;
   }
   lua_pop( L, 1 );

   if( git_index_set_caps( *index, caps ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_index_read( lua_State *L )
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

int luagi_index_write( lua_State *L )
{
   git_index **index = checkindex_at(L, 1 );

   if( git_index_write( *index ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_index_path( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   lua_pushstring( L, git_index_path( *index ) );
   return 1;
}

int luagi_index_read_tree( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   git_tree **tree = checktree_at( L, 2 );

   if( git_index_read_tree( *index, *tree ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_index_write_tree( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   git_oid out;
   if( lua_isuserdata( L, 2 ) )
   {
      git_repository **repo = checkrepo( L, 2 );
      if( git_index_write_tree_to( &out, *index, *repo ) )
      {
         ERROR_PUSH( L )
      }
   }
   else
   {
      if( git_index_write_tree( &out, *index ) )
      {
         ERROR_PUSH( L )
      }
   }
   return luagi_push_oid( L, &out );
}

int luagi_index_entrycount( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );

   lua_pushinteger( L, git_index_entrycount( *index ) );
   return 1;
}

int luagi_index_clear( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );

   if( git_index_clear( *index ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_index_get_byindex( lua_State *L )
{ 
   git_index **index = checkindex_at( L, 1 );
   unsigned int pos = luaL_checkinteger( L, 2 );
   if( pos < 1 || pos > git_index_entrycount( *index ))
   {
        lua_pushnil( L );
        lua_pushstring( L, "index out of bounds");
        return 2;
   }
   // indexes are 1 based
   pos--;

   git_index_entry *entry = lua_newuserdata( L, sizeof( git_index_entry ) );

   const git_index_entry *out = git_index_get_byindex( *index, pos );
   if( out == NULL )
   {
      ERROR_PUSH( L )
   }

   memcpy( entry, out, sizeof( git_index_entry ) );

   luaL_getmetatable(L, LUAGI_INDEX_ENTRY_FUNCS);
   lua_setmetatable(L, -2);
   return 1;
}

int luagi_index_get_bypath( lua_State *L )
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

   luaL_getmetatable(L, LUAGI_INDEX_ENTRY_FUNCS);
   lua_setmetatable(L, -2);
   return 1;
}
   
static int luagi_remove_func( lua_State *L, int (*func)( git_index *index, const char *path, int stage ) )
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

int luagi_index_remove( lua_State *L )
{
  return luagi_remove_func( L, git_index_remove ); 
}

int luagi_index_remove_directory( lua_State *L )
{
  return luagi_remove_func( L, git_index_remove_directory ); 
}

int luagi_index_add( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   const git_index_entry *entry = checkindexentry_at( L, 2 );

   if( git_index_add( *index, entry ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_index_entry_stage( lua_State *L )
{
   const git_index_entry *entry = checkindexentry_at( L, 1 );

   lua_pushinteger( L, git_index_entry_stage( entry ) );
   return 1;
}

static int luagi_index_do_bypath( lua_State *L, int (*func)( git_index *index, const char *path) )
{
   git_index **index = checkindex_at( L, 1 );
   const char *path = luaL_checkstring( L, 2 );

   if( func( *index, path ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_index_add_bypath( lua_State *L )
{
   return luagi_index_do_bypath( L, git_index_add_bypath );
}

int luagi_index_remove_bypath( lua_State *L )
{
   return luagi_index_do_bypath( L, git_index_remove_bypath );
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

   int ret = lua_toboolean( p->L, -1 );
   lua_pop( p->L, 1 );
   return !ret;
}

int luagi_index_add_all( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   git_strarray array = luagi_strings_from_lua_list( L, 2 );

   struct pathspec_payload* p = malloc( sizeof( struct pathspec_payload ) );
   p->L = L;
   p->function_pos = 4;
  
   int flags = GIT_INDEX_ADD_DEFAULT;
   lua_getfield( L, 3, FORCE );
   if( lua_toboolean( L, -1 ) )
   {
      flags |= GIT_INDEX_ADD_FORCE;
   }
   lua_getfield( L, 3, DISABLE_PATHSPEC_MATCH );
   if( lua_toboolean( L, -1 ) )
   {
      flags |= GIT_INDEX_ADD_DISABLE_PATHSPEC_MATCH;
   }
   lua_getfield( L, 3, CHECK_PATHSPEC );
   if( lua_toboolean( L, -1 ) )
   {
      flags |= GIT_INDEX_ADD_CHECK_PATHSPEC;
   }
   lua_pop( L, 3 );

   if( git_index_add_all( *index, &array, flags, index_matched_pathspec, p ) )
   {
      ERROR_ABORT( L );
   }
   git_strarray_free( &array );
   free( p );
   return 0;
}


int luagi_index_remove_all( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   luaL_checktype( L, 2, LUA_TTABLE );
   luaL_checktype( L, 3, LUA_TFUNCTION );
   git_strarray array = luagi_strings_from_lua_list( L, 2 );

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

int luagi_index_update_all( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   git_strarray array = luagi_strings_from_lua_list( L, 2 );

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

int luagi_index_find( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   const char *path = luaL_checkstring( L, 2 );

   lua_pushinteger( L, git_index_find( NULL, *index, path ) );
   return 1;
}

int luagi_index_conflict_add( lua_State *L )
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

int luagi_index_conflict_get( lua_State *L )
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

   luaL_getmetatable(L, LUAGI_INDEX_ENTRY_FUNCS);
   lua_setmetatable(L, -1);
   lua_setmetatable(L, -2);
   lua_setmetatable(L, -3);
   return 3;
}

int luagi_index_conflict_remove( lua_State *L )
{
   return luagi_index_do_bypath( L, git_index_conflict_remove );
}

int luagi_index_conflict_cleanup( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   if( git_index_conflict_cleanup( *index ) )
   {
      const git_error *err = giterr_last();
      luaL_error( L, err->message );
   }
   return 0;
}

int luagi_index_has_conflicts( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );

   lua_pushboolean( L, git_index_has_conflicts( *index ) );
   return 1;
}

int luagi_index_conflict_free( lua_State *L )
{
   git_index_conflict_iterator **iter = checkindexconflict_at( L, 1 );
   git_index_conflict_iterator_free( *iter );
   return 0;
}

static int conflict_iter( lua_State *L )
{
   git_index_conflict_iterator **iter = checkindexconflict_at( L, lua_upvalueindex( 1 ) );   

    const git_index_entry *ancestor_out = lua_newuserdata( L, sizeof( git_index_entry ) );
   const git_index_entry *our_out = lua_newuserdata( L, sizeof( git_index_entry ) );
   const git_index_entry *their_out = lua_newuserdata( L, sizeof( git_index_entry ) );
   int ret = git_index_conflict_next( &ancestor_out, &our_out, &their_out, *iter );
   if( ret < 0 )
   {
      ERROR_ABORT( L )
      return 0;
   }
   else if ( ret == 0 )
   {
      return 0;
   }

   luaL_getmetatable(L, LUAGI_INDEX_ENTRY_FUNCS);
   lua_setmetatable(L, -1);
   lua_setmetatable(L, -2);
   lua_setmetatable(L, -3);
   return 3;

}

int luagi_index_conflict_iterator( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   
   git_index_conflict_iterator **out = lua_newuserdata( L, sizeof( git_index_conflict_iterator *) );
   if( git_index_conflict_iterator_new( out, *index ) )
   {
      ERROR_ABORT( L )
   }

   luaL_getmetatable(L, LUAGI_INDEX_CONFLICT_FUNCS);
   lua_setmetatable(L, -2);

   lua_pushcclosure( L, conflict_iter, -2 );
   return 1; 
}

