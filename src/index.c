#include "index.h"
#include <string.h>
#include <git2/errors.h>
#include <git2/repository.h>
#include <lua.h>
#include "luagi.h"
#include "oid.h"

static int check_stage( lua_State *L, int index )
{
   const char *stage_str = luaL_checkstring( L, index );

   switch( stage_str[0] )
   {
      case 'o':
         return 2; // stage 2 HEAD "ours"
      case 't':
         return 3; // stage 3 MERGE_HEAD "theirs"
      case 'a':
         if( strlen( stage_str ) < 3 )
         {
            luaL_error( L, "ambiguous stage name");
            return -2;
         }
         if( stage_str[2] == 'y' )
         {
            return -1; // any
         }
         else if( stage_str[2] == 'c' )
         {
            return 1; // stage 1 common ancestor "ancestor"
         }
      case 'c':
         return 0; // stage 0 clean "clean"
   }
   luaL_error( L, "invalid stage name");
   return -2;
}

static int push_stage( lua_State *L, int stage )
{
   switch( stage )
   {
      case 0:
         lua_pushstring( L, CLEAN );
         break;
      case 1:
         lua_pushstring( L, ANCESTOR );
         break;
      case 2:
         lua_pushstring( L, OURS );
         break;
      case 3:
         lua_pushstring( L, THEIRS );
         break;
   }
   return 1;
}

static int table_to_index_time( git_index_time *time, lua_State * L, const int index )
{
   luaL_checktype( L, index, LUA_TTABLE );
   lua_getfield( L, index, SECONDS );
   time->seconds = lua_tonumber( L, -1 );
   lua_pop( L, 1 );
   lua_getfield( L, index, NANOSECONDS );
   time->nanoseconds = lua_tonumber( L, -1 );
   lua_pop( L, 1 );
   return 0;
}

static int index_time_to_table( lua_State *L, git_index_time time )
{
   lua_newtable( L );
	lua_pushinteger( L, time.seconds );
   lua_setfield( L, -2, SECONDS );
	lua_pushinteger( L, time.nanoseconds );
   lua_setfield( L, -2, NANOSECONDS );
   return 1;
}

int luagi_check_index_entry( git_index_entry *entry, lua_State *L, const int index)
{
   luaL_checktype( L, index, LUA_TTABLE );

   lua_getfield( L, index, CTIME );
   if( lua_istable( L, -1 ) )
   {
      table_to_index_time( &entry->ctime, L, -1 );
   }
   lua_getfield( L, index, MTIME );
   if( lua_istable( L, -1 ) )
   {
      table_to_index_time( &entry->mtime, L, -1 );
   }

   lua_getfield( L, index, DEV );
   entry->dev = lua_tonumber( L, -1 );
   lua_pop( L, 1 );

   lua_getfield( L, index, INO );
   entry->ino = lua_tonumber( L, -1 );
   lua_pop( L, 1 );

   lua_getfield( L, index, MODE );
   entry->mode = lua_tonumber( L, -1 );
   lua_pop( L, 1 );

   lua_getfield( L, index, UID );
   entry->uid = lua_tonumber( L, -1 );
   lua_pop( L, 1 );

   lua_getfield( L, index, GID );
   entry->gid = lua_tonumber( L, -1 );
   lua_pop( L, 1 );

   lua_getfield( L, index, FILE_SIZE );
   entry->file_size = lua_tonumber( L, -1 );
   lua_pop( L, 1 );

   lua_getfield( L, index, OID );
   luagi_check_oid( &entry->id, L, -1 );
   lua_pop( L, 1 );

   lua_getfield( L, index, PATH );
   entry->path = luaL_optstring(L, -1, NULL );

   return 0;
}

int luagi_push_index_entry( lua_State *L, const git_index_entry *entry )
{
   if( ! entry )
   {
      lua_pushnil( L );
      return 1;
   }
   lua_newtable( L );
   index_time_to_table( L, entry->ctime );
   lua_setfield( L, -2, CTIME );
   index_time_to_table( L, entry->mtime );
   lua_setfield( L, -2, MTIME );

   lua_pushinteger( L, entry->dev );
   lua_setfield( L, -2, DEV );
   lua_pushinteger( L, entry->ino );
   lua_setfield( L, -2, INO );
   lua_pushinteger( L, entry->mode );
   lua_setfield( L, -2, MODE );
   lua_pushinteger( L, entry->uid );
   lua_setfield( L, -2, UID );
   lua_pushinteger( L, entry->gid );
   lua_setfield( L, -2, GID );

   lua_pushinteger( L, entry->file_size );
   lua_setfield( L, -2, FILE_SIZE );

	luagi_push_oid( L, &entry->id );
   lua_setfield( L, -2, OID );

	//unsigned short flags;
	//unsigned short flags_extended;

   lua_pushstring( L, entry->path );
   lua_setfield( L, -2, PATH );

   push_stage( L, git_index_entry_stage( entry ) );
   lua_setfield( L, -2, STAGE );
   return 1;
}

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

   lua_pushboolean( L, (caps & GIT_INDEXCAP_FROM_OWNER) == GIT_INDEXCAP_FROM_OWNER );
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

   const git_index_entry *out = git_index_get_byindex( *index, pos );
   if( out == NULL )
   {
      ERROR_PUSH( L )
   }

   return luagi_push_index_entry( L, out );
}

int luagi_index_get_bypath( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   const char *path = luaL_checkstring( L, 2 );
   int stage = check_stage( L, 3 );

   const git_index_entry *out = git_index_get_bypath( *index, path, stage );
   if( out == NULL )
   {
      ERROR_PUSH( L )
   }

   return luagi_push_index_entry( L, out );
}
   
static int luagi_remove_func( lua_State *L, int (*func)( git_index *index, const char *path, int stage ) )
{
   git_index **index = checkindex_at( L, 1 );
   const char *path = luaL_checkstring( L, 2 );
   int stage = check_stage( L, 3 );

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
   git_index_entry entry;
   luagi_check_index_entry( &entry, L, 2 );

   if( git_index_add( *index, &entry ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
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
   luaL_checktype( L, 2, LUA_TTABLE );
   luaL_checktype( L, 3, LUA_TTABLE );
   luaL_checktype( L, 4, LUA_TFUNCTION );
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
   luaL_checktype( L, 2, LUA_TTABLE );
   luaL_checktype( L, 3, LUA_TFUNCTION );
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
   int ret = git_index_find( NULL, *index, path );
   if( ret < 0 )
   {
      lua_pushnil( L );
      return 1;
   }
   else
   {
      lua_pushinteger( L, ret + 1); //one based index
      return 1;
   }
}

int luagi_index_conflict_add( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   git_index_entry ancestor_entry;
   luagi_check_index_entry( &ancestor_entry, L, 2 );
   git_index_entry our_entry;
   luagi_check_index_entry( &our_entry, L, 3 );
   git_index_entry their_entry;
   luagi_check_index_entry( &their_entry, L, 4 );

   if( git_index_conflict_add( *index, &ancestor_entry, &our_entry, &their_entry ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_index_conflict_get( lua_State *L )
{
   git_index **index = checkindex_at( L, 1 );
   const char *path = luaL_checkstring( L, 2 );

   const git_index_entry *ancestor_out;
   const git_index_entry *our_out;
   const git_index_entry *their_out;
   int ret = git_index_conflict_get( &ancestor_out, &our_out, &their_out, *index, path );
   if( ret == GIT_ENOTFOUND )
   {
      return 0;
   }
   else if( ret )
   {
      ERROR_ABORT( L )
      return 0;
   }

   ret += luagi_push_index_entry( L, ancestor_out );
   ret += luagi_push_index_entry( L, our_out );
   ret += luagi_push_index_entry( L, their_out );
   return ret;
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

   const git_index_entry *ancestor_out;
   const git_index_entry *our_out;
   const git_index_entry *their_out;
   int ret = git_index_conflict_next( &ancestor_out, &our_out, &their_out, *iter );
   if( ret == GIT_ITEROVER )
   {
      return 0;
   }
   else if ( ret < 0 )
   {
      ERROR_ABORT( L )
      return 0;
   }

   ret += luagi_push_index_entry( L, ancestor_out );
   ret += luagi_push_index_entry( L, our_out );
   ret += luagi_push_index_entry( L, their_out );
   return ret;
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

   lua_pushcclosure( L, conflict_iter, 1 );
   return 1; 
}

