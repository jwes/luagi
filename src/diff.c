#include "diff.h" 
#include "luagi.h" 
#include "index.h"
#include <git2/diff.h>
#include <git2/errors.h>
#include <stdlib.h>
#include <string.h>

#define DIFF_FILE_ID "id"
#define DIFF_FILE_PATH "path"
#define DIFF_FILE_SIZE "size"
#define DIFF_FILE_FLAGS "flags"
#define DIFF_FILE_MODE "mode"

#define DIFF_DELTA_STATUS "status"
#define DIFF_DELTA_FLAGS "flags"
#define DIFF_DELTA_SIMI "similarity"
#define DIFF_DELTA_NFILES "number_of_files"
#define DIFF_DELTA_OLD_FILE "old_file"
#define DIFF_DELTA_NEW_FILE "new_file"

#define BINARY "binary"
#define NOT_BINARY "not_binary"
#define VALID_ID "valid_id"

#define PATCH "patch"
#define PATCH_HEADER "patch_header"
#define RAW "raw"
#define NAME_ONLY "name_only"
#define NAME_STATUS "name_status"

#define D_ADDED "added"
#define D_DELETED "deleted"
#define D_MODIFIED "modified"
#define D_RENAMED "renamed"
#define D_COPIED "copied"
#define D_IGNORED "ignored"
#define D_UNTRACKED "untracked"
#define D_TYPECHANGE "typechange"

void options_from_lua( lua_State *L, int idx, git_diff_options *opts )
{
   lua_isboolean( L, idx );
   git_diff_init_options( opts, GIT_DIFF_OPTIONS_VERSION );
}

int luagi_diff_tree_to_tree( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_tree **old_tree = checktree_at( L, 2 );
   git_tree **new_tree = checktree_at( L, 3 );

   git_diff_options opts;
   options_from_lua( L, 4, &opts );
   
   git_diff **diff = (git_diff **) lua_newuserdata( L, sizeof( git_diff *) );

   if( git_diff_tree_to_tree( diff, *repo, *old_tree, *new_tree, &opts ) )
   {
      const git_error *err = giterr_last();
      lua_pushnil( L );
      lua_pushstring( L, err->message );
      return 2;
   }

   luaL_getmetatable(L, LUAGI_DIFF_FUNCS);
   lua_setmetatable(L, -2);

   return 1;
}

int luagi_diff_tree_to_index( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_tree **tree = checktree_at( L, 2 );
   git_index **index = checkindex_at( L, 3 );

   git_diff_options opts;
   options_from_lua( L, 4, &opts );

   git_diff **diff = (git_diff **) lua_newuserdata( L, sizeof( git_diff *) );

   if( git_diff_tree_to_index( diff, *repo, *tree, *index, &opts ) )
   {
      const git_error *err = giterr_last();
      lua_pushnil( L );
      lua_pushstring( L, err->message );
      return 2;
   }

   luaL_getmetatable(L, LUAGI_DIFF_FUNCS);
   lua_setmetatable(L, -2);
   return 1; 
}

int luagi_diff_index_to_workdir( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_index **index = checkindex_at( L, 2 );

   git_diff_options opts;
   options_from_lua( L, 3, &opts );
   
   git_diff **diff = (git_diff **) lua_newuserdata( L, sizeof( git_diff *) );

   if( git_diff_index_to_workdir( diff, *repo, *index, &opts ) )
   {
      const git_error *err = giterr_last();
      lua_pushnil( L );
      lua_pushstring( L, err->message );
      return 2;
   }

   luaL_getmetatable(L, LUAGI_DIFF_FUNCS);
   lua_setmetatable(L, -2);

   return 1; 
}
static int tree_to_workdir( lua_State *L, int (*func)(git_diff **diff, git_repository *repo, git_tree *old_tree, const git_diff_options *opts ) )
{
   git_repository **repo = checkrepo( L, 1 );
   git_tree **tree = checktree_at( L, 2 );

   git_diff_options opts;
   options_from_lua( L, 3, &opts );
   
   git_diff **diff = (git_diff **) lua_newuserdata( L, sizeof( git_diff *) );

   if( func( diff, *repo, *tree, &opts ) )
   {
      const git_error *err = giterr_last();
      lua_pushnil( L );
      lua_pushstring( L, err->message );
      return 2;
   }

   luaL_getmetatable(L, LUAGI_DIFF_FUNCS);
   lua_setmetatable(L, -2);

   return 1;

   lua_pushnil( L );
   return 1; 
}
int luagi_diff_tree_to_workdir( lua_State *L )
{
   return tree_to_workdir( L, git_diff_tree_to_workdir );
}
int luagi_diff_tree_to_workdir_with_index( lua_State *L )
{
   return tree_to_workdir( L, git_diff_tree_to_workdir_with_index );
}

// diff functions
int luagi_diff_merge( lua_State *L )
{
   git_diff **diff = checkdiff_at( L, 1 );
   git_diff **from = checkdiff_at( L, 2 );
   if( git_diff_merge( *diff, *from ) )
   {
      const git_error *err = giterr_last();
      luaL_error(L, err->message );
   }
   return 0; 
}

int luagi_diff_find_similar( lua_State *L )
{
   //TODO find options
   git_diff **diff = checkdiff_at( L, 1 );

   git_diff_find_options options;
   if( git_diff_find_init_options( &options, GIT_DIFF_FIND_OPTIONS_VERSION ) ) 
   {
      const git_error *err = giterr_last();
      luaL_error( L, err->message );
   }

   if( git_diff_find_similar( *diff, &options ) )
   {
      const git_error *err = giterr_last();
      luaL_error( L, err->message );
   }
   return 0;
}
static git_delta_t delta_t_from_string( const char *type );

int luagi_diff_num_deltas( lua_State *L )
{ 
   git_diff **diff = checkdiff_at( L, 1 );

   if( ! lua_isstring( L, 2 ) )
   {
      lua_pushinteger( L, git_diff_num_deltas( *diff ) );
      return 1; 
   }
   // there is a type parameter, use it
   const char *type = lua_tostring( L, 2 );
   git_delta_t t =  delta_t_from_string( type );
   lua_pushinteger( L, git_diff_num_deltas_of_type( *diff, t ) );
   return 1;
} // includes num deltas of type

int luagi_diff_get_delta( lua_State *L )
{
   git_diff **diff = checkdiff_at( L, 1 );
   int idx = luaL_checkinteger( L, 2 );
   const git_diff_delta *delta = git_diff_get_delta( *diff, idx );

   diff_delta_to_table( L, delta );
   return 1; 
}
int luagi_diff_is_sorted_icase( lua_State *L )
{
   git_diff **diff = checkdiff_at( L, 1 );
   lua_pushboolean( L, git_diff_is_sorted_icase( *diff ) );
   return 1; 
}

struct foreach_f
{
   lua_State *L;
   int use_hunks;
   int use_lines;
};

static int diff_file_callback( const git_diff_delta *delta,
                 float progress,
                 void *payload)
{
   struct foreach_f *f = payload;


   
   lua_pushvalue( f->L, 2 );
   diff_delta_to_table( f->L, delta );
   lua_pushnumber( f->L, progress );

   debugStack( f->L );

   if( lua_pcall( f->L, 2, 1, 0 ) != LUA_OK )
   {
      dumpStack( f->L );
      luaL_error( f->L, "can not call file callback" );
   }

   int ret = luaL_checkinteger( f->L, -1 );
   lua_pop( f->L, 1 );
   return ret;
}

static int diff_hunk_callback( const git_diff_delta *delta,
                const git_diff_hunk *hunk,
                void *payload)
{
   struct foreach_f *f = payload;
   if( ! f->use_hunks )
      return 1;

   lua_pushvalue( f->L, f->use_hunks );
   diff_delta_to_table( f->L, delta );

   lua_pushinteger( f->L, hunk->old_start );
   lua_pushinteger( f->L, hunk->old_lines );
   lua_pushinteger( f->L, hunk->new_start );
   lua_pushinteger( f->L, hunk->new_lines );
   lua_pushstring( f->L, hunk->header );
   
   if( lua_pcall( f->L, 6, 1, 0 ) )
   {
      luaL_error( f->L, "can not call hunk callback" );
   }
   int ret = luaL_checkinteger( f->L, -1 );
   lua_pop( f->L, 1 );
   return ret;
}

static int diff_line_callback( const git_diff_delta *delta,
                const git_diff_hunk *hunk,
                const git_diff_line *line,
                void *payload)
{
   struct foreach_f *f = payload;

   if( ! f->use_lines )
   {
      return 1;
   }

   lua_pushvalue( f->L, f->use_lines );
   diff_delta_to_table( f->L, delta );

   if( hunk && hunk->header && *hunk->header )
   {
      lua_pushstring( f->L, hunk->header );
   }
   else
   {
      lua_pushnil( f->L );
   }

   lua_pushfstring( f->L, "%c", line->origin ); 
   lua_pushinteger( f->L, line->old_lineno );
   lua_pushinteger( f->L, line->new_lineno );
   lua_pushinteger( f->L, line->num_lines );
   
   if( line->content && *line->content )
   {
      char content [ line->content_len + 1 ];
      memcpy( content, line->content, line->content_len );
      content[ line->content_len ] = 0;
      lua_pushstring( f->L, content ); 
   }
   else
   {
      lua_pushnil( f->L );
   }

   if( lua_pcall( f->L, 7, 1, 0 ) )
   {
      luaL_error( f->L, "can not call line callback" );
   }
   int ret = luaL_checkinteger( f->L, -1 );
   lua_pop( f->L, 1 );
   return ret;
}

int luagi_diff_foreach( lua_State *L )
{
   git_diff **diff = checkdiff_at( L, 1 );

   luaL_checktype( L, 2, LUA_TFUNCTION );
   
   struct foreach_f *f = malloc( sizeof( struct foreach_f) );
   f->L = L;

   if( lua_type( L, 3) == LUA_TFUNCTION )
   {
      f->use_hunks = 3;
   } else {
      f->use_hunks = 0;
   }
   if( lua_type( L, 4) == LUA_TFUNCTION )
   {
      f->use_lines = 4;
   } else {
      f->use_lines = 0;
   }

   git_diff_foreach( *diff,
                   diff_file_callback,
                   diff_hunk_callback,
                   diff_line_callback,
                   f );

   free( f );

   return 0;
}
int luagi_diff_print( lua_State *L )
{
   git_diff **diff = checkdiff_at( L, 1 );
   git_diff_format_t format = GIT_DIFF_FORMAT_PATCH;
   const char *format_str = luaL_checkstring( L, 2 );

   if( strncmp( format_str, PATCH_HEADER, sizeof( PATCH_HEADER ) ) ) 
   {
        format = GIT_DIFF_FORMAT_PATCH; 
   }
   else if( strncmp( format_str, RAW, sizeof( RAW ) ) ) 
   {
        format = GIT_DIFF_FORMAT_RAW; 
   }
   else if( strncmp( format_str, NAME_ONLY, sizeof( NAME_ONLY ) ) )
   {
        format = GIT_DIFF_FORMAT_NAME_ONLY;
   }
   else if( strncmp( format_str, NAME_STATUS, sizeof( NAME_STATUS ) ) )
   {
        format = GIT_DIFF_FORMAT_NAME_STATUS;
   }
   luaL_checktype( L, 3, LUA_TFUNCTION );

   struct foreach_f *f = malloc( sizeof( struct foreach_f) );
   f->L = L;
   f->use_hunks=0;
   f->use_lines=3;

   if( git_diff_print( *diff, format, diff_line_callback, f ) )
   {
      free( f );
      const git_error *err = giterr_last();
      luaL_error(L, err->message );
      return 0;
   }
   free( f );
   return 0; 
}

int luagi_diff_free( lua_State *L )
{
   git_diff **diff = checkdiff_at( L, 1 );
   git_diff_free( *diff );
   return 0;
}

int luagi_diff_get_stats( lua_State *L )
{
   git_diff **diff = checkdiff_at( L, 1 );
   git_diff_stats **out = lua_newuserdata( L, sizeof( git_diff_stats *) );
   if( git_diff_get_stats( out, *diff ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_DIFF_STATS_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

static int stats_infos( lua_State *L, size_t (*func)( const git_diff_stats *stats ) )
{
   git_diff_stats **stats = checkdiffstats_at( L, 1 );
   size_t changes = func( *stats );
   lua_pushinteger( L, changes );
   return 1;
}

int luagi_diff_stats_files_changed( lua_State *L )
{
   return stats_infos( L, git_diff_stats_files_changed );
}

int luagi_diff_stats_insertions( lua_State *L )
{
   return stats_infos( L, git_diff_stats_insertions );
}

int luagi_diff_stats_deletions( lua_State *L )
{
   return stats_infos( L, git_diff_stats_deletions );
}

int luagi_diff_stats_to_buf( lua_State *L )
{
   git_diff_stats **stats = checkdiffstats_at( L, 1 );
   //TODO stats format
   git_diff_stats_format_t format = GIT_DIFF_STATS_NONE;
   size_t width = 0;

   git_buf out;
   if( git_diff_stats_to_buf( &out, *stats, format, width ) )
   {
      ERROR_PUSH( L )
   }
   lua_pushlstring( L, out.ptr, out.size );
   return 1;
}

int luagi_diff_stats_free( lua_State *L )
{
   git_diff_stats **stats = checkdiffstats_at( L, 1 );
   git_diff_stats_free( *stats );
   return 0;
}
static int luagi_diff_format_email_init_options( lua_State *L __attribute__((unused)), int index __attribute__((unused)), git_diff_format_email_options *opts )
{
   int ret =  git_diff_format_email_init_options( opts, GIT_DIFF_FORMAT_EMAIL_OPTIONS_VERSION );
   //TODO email format options
   return ret;
}

int luagi_diff_format_email( lua_State *L )
{
   git_diff **diff = checkdiff_at( L, 1 );

   git_buf out;
   git_diff_format_email_options opts;
   luagi_diff_format_email_init_options( L, 2, &opts );

   if( git_diff_format_email( &out, *diff, &opts ) )
   {
      ERROR_PUSH( L )
   }

   lua_pushlstring( L, out.ptr, out.size );
   return 1;
}
int luagi_diff_commit_as_email( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
   return 0;
}

void add_flags( lua_State *L, int idx, int32_t flags )
{
   if( flags & GIT_DIFF_FLAG_BINARY )
   {
      lua_pushboolean( L, 1 );
      lua_setfield( L, idx, BINARY );
   }
   if( flags & GIT_DIFF_FLAG_NOT_BINARY )
   {
      lua_pushboolean( L, 1 );
      lua_setfield( L, idx, NOT_BINARY );
   }
   if( flags & GIT_DIFF_FLAG_VALID_ID )
   {
      lua_pushboolean( L, 1 );
      lua_setfield( L, idx, VALID_ID );
   }
}

void diff_file_to_table( lua_State *L, const git_diff_file file )
{
   lua_newtable( L );
   char buf [GIT_OID_HEXSZ+1];
   lua_pushstring( L, git_oid_tostr( buf, GIT_OID_HEXSZ+1, &(file.id) ) );
   lua_setfield( L, -2, DIFF_FILE_ID );

   lua_pushstring( L, file.path );
   lua_setfield( L, -2, DIFF_FILE_PATH );
  
   lua_pushinteger( L, file.size );
   lua_setfield( L, -2, DIFF_FILE_SIZE );

   add_flags( L, -2, file.flags );

   lua_pushinteger( L, file.mode );
   lua_setfield( L, -2, DIFF_FILE_MODE );

   debugStack( L );
}


void diff_delta_to_table( lua_State *L, const git_diff_delta *delta )
{
   lua_newtable( L );

   lua_pushinteger( L, delta->status );
   lua_setfield( L, -2, DIFF_DELTA_STATUS );

   add_flags( L, -2, delta->flags );

   lua_pushinteger( L, delta->similarity );
   lua_setfield( L, -2, DIFF_DELTA_SIMI );

   lua_pushinteger( L, delta->nfiles );
   lua_setfield( L, -2, DIFF_DELTA_NFILES );
   
   diff_file_to_table( L, delta->old_file );
   lua_setfield( L, -2, DIFF_DELTA_OLD_FILE );

   diff_file_to_table( L, delta->new_file );
   lua_setfield( L, -2, DIFF_DELTA_NEW_FILE );
  
   debugStack( L );

}

static git_delta_t delta_t_from_string( const char *type )
{
   git_delta_t t = GIT_DELTA_UNMODIFIED;
   if( strncmp( type, D_ADDED, strlen( D_ADDED ) ) )
   {
      t = GIT_DELTA_ADDED;
   }
   else if( strncmp( type, D_DELETED, strlen( D_DELETED ) ) )
   {
      t = GIT_DELTA_DELETED;
   }
   else if( strncmp( type, D_MODIFIED, strlen( D_MODIFIED ) ) )
   {
      t = GIT_DELTA_MODIFIED;
   }
   else if( strncmp( type, D_RENAMED, strlen( D_RENAMED ) ) )
   {
      t = GIT_DELTA_RENAMED;
   }
   else if( strncmp( type, D_COPIED, strlen( D_COPIED ) ) )
   {
      t = GIT_DELTA_COPIED;
   }
   else if( strncmp( type, D_IGNORED, strlen( D_IGNORED ) ) )
   {
      t = GIT_DELTA_IGNORED;
   }
   else if( strncmp( type, D_UNTRACKED, strlen( D_UNTRACKED ) ) )
   {
      t = GIT_DELTA_UNTRACKED;
   }
   else if( strncmp( type, D_TYPECHANGE, strlen( D_TYPECHANGE ) ) )
   {
      t = GIT_DELTA_TYPECHANGE;
   }
   return t;
}
