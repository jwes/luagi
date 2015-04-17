#include "diff.h" 
#include <git2/diff.h>
#include <git2/errors.h>
#include <stdlib.h>
#include <string.h>

#include "luagi.h" 
#include "index.h"
#include "commit.h"
#include "submodule.h"
#include "oid.h"

int luagi_diff_init_options( lua_State *L, int idx, git_diff_options *opts )
{
   int ret = git_diff_init_options( opts, GIT_DIFF_OPTIONS_VERSION );
   if( lua_type( L, idx ) == LUA_TTABLE )
   {

      add_flag( opts->flags, L, idx,
               NORMAL,
               GIT_DIFF_NORMAL );
      add_flag( opts->flags, L, idx,
               REVERSE,
               GIT_DIFF_REVERSE );
      add_flag( opts->flags, L, idx,
               INCLUDE_IGNORED,
               GIT_DIFF_INCLUDE_IGNORED );
      add_flag( opts->flags, L, idx,
               RECURSE_IGNORED_DIRS,
               GIT_DIFF_RECURSE_IGNORED_DIRS );
      add_flag( opts->flags, L, idx,
               INCLUDE_UNTRACKED,
               GIT_DIFF_INCLUDE_UNTRACKED );
      add_flag( opts->flags, L, idx,
               RECURSE_UNTRACKED_DIRS,
               GIT_DIFF_RECURSE_UNTRACKED_DIRS );
      add_flag( opts->flags, L, idx,
               INCLUDE_UNMODIFIED,
               GIT_DIFF_INCLUDE_UNMODIFIED );
      add_flag( opts->flags, L, idx,
               INCLUDE_TYPECHANGE,
               GIT_DIFF_INCLUDE_TYPECHANGE );
      add_flag( opts->flags, L, idx,
               INCLUDE_TYPECHANGE_TREES,
               GIT_DIFF_INCLUDE_TYPECHANGE_TREES );
      add_flag( opts->flags, L, idx,
               IGNORE_FILEMODE,
               GIT_DIFF_IGNORE_FILEMODE );
      add_flag( opts->flags, L, idx,
               IGNORE_SUBMODULES,
               GIT_DIFF_IGNORE_SUBMODULES );
      add_flag( opts->flags, L, idx,
               IGNORE_CASE,
               GIT_DIFF_IGNORE_CASE );
      add_flag( opts->flags, L, idx,
               DISABLE_PATHSPEC_MATCH,
               GIT_DIFF_DISABLE_PATHSPEC_MATCH );
      add_flag( opts->flags, L, idx,
               SKIP_BINARY_CHECK,
               GIT_DIFF_SKIP_BINARY_CHECK );
      add_flag( opts->flags, L, idx,
               ENABLE_FAST_UNTRACKED_DIRS,
               GIT_DIFF_ENABLE_FAST_UNTRACKED_DIRS );
      add_flag( opts->flags, L, idx,
               UPDATE_INDEX,
               GIT_DIFF_UPDATE_INDEX );
      add_flag( opts->flags, L, idx,
               FORCE_TEXT,
               GIT_DIFF_FORCE_TEXT );
      add_flag( opts->flags, L, idx,
               FORCE_BINARY,
               GIT_DIFF_FORCE_BINARY );
      add_flag( opts->flags, L, idx,
               IGNORE_WHITESPACE,
               GIT_DIFF_IGNORE_WHITESPACE );
      add_flag( opts->flags, L, idx,
               IGNORE_WHITESPACE_CHANGE,
               GIT_DIFF_IGNORE_WHITESPACE_CHANGE );
      add_flag( opts->flags, L, idx,
               IGNORE_WHITESPACE_EOL,
               GIT_DIFF_IGNORE_WHITESPACE_EOL );
      add_flag( opts->flags, L, idx,
               SHOW_UNTRACKED_CONTENT,
               GIT_DIFF_SHOW_UNTRACKED_CONTENT );
      add_flag( opts->flags, L, idx,
               SHOW_UNMODIFIED,
               GIT_DIFF_SHOW_UNMODIFIED );
      add_flag( opts->flags, L, idx,
               PATIENCE,
               GIT_DIFF_PATIENCE );
      add_flag( opts->flags, L, idx,
               MINIMAL,
               GIT_DIFF_MINIMAL );
      add_flag( opts->flags, L, idx,
               SHOW_BINARY,
               GIT_DIFF_SHOW_BINARY );
   
      lua_getfield( L, idx, IGNORE_SUBMODULES_TYPE );
      opts->ignore_submodules = luagi_sub_check_ignore( L, -1 );
   
      lua_getfield( L, idx, PATHSPEC );
      opts->pathspec = luagi_strings_from_lua_list( L, -1 );

      //TODO notify_cb
   
      lua_getfield( L, idx, CONTEXT_LINES );
      int context_lines = luaL_optinteger(L, -1, -1 );
      if( context_lines >= 0 )
      {
         opts->context_lines = context_lines;
      }

      lua_getfield( L, idx, INTERHUNK_LINES );
      int ihunk = luaL_optinteger( L, -1, -1 );
      if( ihunk >= 0 )
      {
         opts->interhunk_lines = ihunk;
      }
   
      lua_getfield( L, idx, ID_ABBREV );
      int ida = luaL_optinteger( L, -1, -1 );
      if( ida >= 0 )
      {
         opts->id_abbrev = ida;
      }
   
      lua_getfield( L, idx, MAX_SIZE );
      git_off_t msize = luaL_optinteger( L, -1, -1 );
      if( msize >= 0 )
      {
         opts->max_size = msize;
      }

      lua_getfield( L, idx, OLD_PREFIX );
      opts->old_prefix = luaL_checkstring( L, -1 );
   
      lua_getfield( L, idx, NEW_PREFIX );
      opts->new_prefix = luaL_checkstring( L, -1 );
   } 
   return ret;
}

int luagi_diff_tree_to_tree( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_tree **old_tree = checktree_at( L, 2 );
   git_tree **new_tree = checktree_at( L, 3 );

   git_diff_options opts;
   luagi_diff_init_options( L, 4, &opts );
   
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
   luagi_diff_init_options( L, 4, &opts );

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
   luagi_diff_init_options( L, 3, &opts );
   
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
   luagi_diff_init_options( L, 3, &opts );
   
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

static int luagi_diff_find_init_options( lua_State *L, int index, git_diff_find_options *opts )
{
   int ret = git_diff_find_init_options( opts, GIT_DIFF_FIND_OPTIONS_VERSION );

   add_flag( opts->flags, L, index,
            BY_CONFIG,
            GIT_DIFF_FIND_BY_CONFIG );
   add_flag( opts->flags, L, index,
            RENAMES,
            GIT_DIFF_FIND_RENAMES );
   add_flag( opts->flags, L, index,
            RENAMES_FROM_REWRITES,
            GIT_DIFF_FIND_RENAMES_FROM_REWRITES );
   add_flag( opts->flags, L, index,
            COPIES,
            GIT_DIFF_FIND_COPIES );
   add_flag( opts->flags, L, index,
            COPIES_FROM_UNMODIFIED,
            GIT_DIFF_FIND_COPIES_FROM_UNMODIFIED );
   add_flag( opts->flags, L, index,
            REWRITES,
            GIT_DIFF_FIND_REWRITES );
   add_flag( opts->flags, L, index,
            BREAK_REWRITES,
            GIT_DIFF_BREAK_REWRITES );
   add_flag( opts->flags, L, index,
            FOR_UNTRACKED,
            GIT_DIFF_FIND_FOR_UNTRACKED );
   add_flag( opts->flags, L, index,
            ALL,
            GIT_DIFF_FIND_ALL );
   add_flag( opts->flags, L, index,
            IGNORE_WHITESPACE,
            GIT_DIFF_FIND_IGNORE_WHITESPACE );
   add_flag( opts->flags, L, index,
            DONT_IGNORE_WHITESPACE,
            GIT_DIFF_FIND_DONT_IGNORE_WHITESPACE );
   add_flag( opts->flags, L, index,
            EXACT_MATCH_ONLY,
            GIT_DIFF_FIND_EXACT_MATCH_ONLY );
   add_flag( opts->flags, L, index,
            REWRITES_FOR_RENAMES_ONLY,
            GIT_DIFF_BREAK_REWRITES_FOR_RENAMES_ONLY );
   add_flag( opts->flags, L, index,
            REMOVE_UNMODIFIED,
            GIT_DIFF_FIND_REMOVE_UNMODIFIED );
   
   lua_getfield( L, index, RENAME_THRESHOLD );
   int rthresh = luaL_optinteger( L, -1, -1 );
   if( rthresh >= 0 )
   {
      opts->rename_threshold = rthresh;
   }

   lua_getfield( L, index, RENAME_FROM_REWRITE_THRESHOLD );
   int rwthresh = luaL_optinteger( L, -1, -1 );
   if( rwthresh >= 0 )
   {
      opts->rename_from_rewrite_threshold = rwthresh;
   }

   lua_getfield( L, index, COPY_THRESHOLD );
   int copy = luaL_optinteger( L, -1, -1 );
   if( copy >= 0 )
   {
      opts->copy_threshold = copy;
   }

   lua_getfield( L, index, BREAK_REWRITE_THRESHOLD );
   int brthresh = luaL_optinteger( L, -1, -1 );
   if( brthresh >= 0 )
   {
      opts->break_rewrite_threshold = brthresh;
   }

   lua_getfield( L, index, RENAME_LIMIT );
   int rename_limit = luaL_optinteger( L, -1, -1 );
   if( rename_limit >= 0 )
   {
      opts->rename_limit = rename_limit;
   }

   //TODO similarity mertic
   return ret;
}

int luagi_diff_find_similar( lua_State *L )
{
   git_diff **diff = checkdiff_at( L, 1 );

   git_diff_find_options options;
   if( luagi_diff_find_init_options( L, 2, &options ) ) 
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
   int num = git_diff_num_deltas( *diff );
   if( idx <= 0 || idx > num )
   {
        lua_pushnil( L );
        lua_pushstring( L, "index out of bounds" );
        return 2;
   }
   idx--;

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

void diff_line_to_table( lua_State *L, const git_diff_line *line )
{
   lua_newtable( L );
   lua_pushfstring( L, "%c", line->origin ); 
   lua_setfield( L, -2, ORIGIN ); 
   lua_pushinteger( L, line->old_lineno );
   lua_setfield( L, -2, OLD_LINENO ); 
   lua_pushinteger( L, line->new_lineno );
   lua_setfield( L, -2, NEW_LINENO ); 
   lua_pushinteger( L, line->num_lines );
   lua_setfield( L, -2, LINES ); 
   
   if( line->content && *line->content )
   {
      char content [ line->content_len + 1 ];
      memcpy( content, line->content, line->content_len );
      content[ line->content_len ] = 0;
      lua_pushstring( L, content ); 
      lua_setfield( L, -2, CONTENT );
   }

}

void diff_hunk_to_table( lua_State *L, const git_diff_hunk *hunk )
{
   lua_newtable( L );
   lua_pushinteger( L, hunk->old_start );
   lua_setfield( L, -2, OLD_START );
   lua_pushinteger( L, hunk->old_lines );
   lua_setfield( L, -2, OLD_LINES );

   lua_pushinteger( L, hunk->new_start );
   lua_setfield( L, -2, NEW_START );
   lua_pushinteger( L, hunk->new_lines );
   lua_setfield( L, -2, NEW_LINES );
   lua_pushstring( L, hunk->header );
   lua_setfield( L, -2, HEADER );
 
}


static int diff_hunk_callback( const git_diff_delta *delta,
                const git_diff_hunk *hunk,
                void *payload)
{
   struct foreach_f *f = payload;
   if( ! f->use_hunks )
      return 1;

   lua_pushvalue( f->L, f->use_hunks );
   if( delta )
   {
      diff_delta_to_table( f->L, delta );
   }
   else lua_pushnil( f->L );

   if( hunk )
   {
      diff_hunk_to_table( f->L, hunk );
   }
   else lua_pushnil( f->L );
  
   if( lua_pcall( f->L, 2, 1, 0 ) )
   {
      luaL_error( f->L, "can not call hunk callback" );
   }
   int ret = luaL_checkinteger( f->L, -1 );
   lua_pop( f->L, 1 );
   return ret;
}

int diff_line_callback( const git_diff_delta *delta,
                const git_diff_hunk *hunk,
                const git_diff_line *line,
                void *payload)
{
   struct foreach_f *f = payload;

   if( ! f->use_lines )
   {
      return -1;
   }

   lua_pushvalue( f->L, f->use_lines );
   if( delta )
   {
      diff_delta_to_table( f->L, delta );
   }
   else lua_pushnil( f->L );

   if( hunk )
   {
      diff_hunk_to_table( f->L, hunk );
   }
   else lua_pushnil( f->L );

   if( line )
   {
      diff_line_to_table( f->L, line );
   }
   else lua_pushnil( f->L );

   if( lua_pcall( f->L, 3, 1, 0 ) )
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

   if( strncmp( format_str, PATCH, sizeof( PATCH ) ) ) 
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
   git_diff_stats_format_t format = GIT_DIFF_STATS_NONE;
   const char *formatstr = luaL_optstring( L, 2, NULL );

   if( formatstr != NULL )
   {
      switch ( formatstr[0] )
      {
         case 'n':
            if( formatstr[1] == 'u' )
            {
               format = GIT_DIFF_STATS_NUMBER;
            }
            break;
         case 'f':
            format = GIT_DIFF_STATS_FULL;
            break;
         case 's':
            format = GIT_DIFF_STATS_SHORT;
            break;
         case 'i':
            format = GIT_DIFF_STATS_INCLUDE_SUMMARY;
            break;
      }
   }

   size_t width = luaL_optinteger( L, 3, 0 );

   git_buf out = GIT_BUF_INIT_CONST(NULL, 0);
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
static int luagi_diff_format_email_init_options( lua_State *L, int index, git_diff_format_email_options *opts )
{
   int ret =  git_diff_format_email_init_options( opts, GIT_DIFF_FORMAT_EMAIL_OPTIONS_VERSION );
   
   add_flag( opts->flags, L, index, NONE, GIT_DIFF_FORMAT_EMAIL_NONE );
   add_flag( opts->flags, L, index, EXCLUDE_SUBJECT_PATCH_MARKER, GIT_DIFF_FORMAT_EMAIL_EXCLUDE_SUBJECT_PATCH_MARKER );

   lua_getfield( L, index, PATCH_NO );
   if( lua_type( L, -1 ) == LUA_TNUMBER )
   {
      opts->patch_no = luaL_checkinteger( L, -1 );
   }

   lua_getfield( L, index, TOTAL_PATCHES );
   if( lua_type( L, -1) == LUA_TNUMBER )
   {
      opts->total_patches = luaL_checkinteger( L, -1 );
   }

   lua_getfield( L, index, OID );
   luagi_check_oid( (git_oid *)opts->id, L, -1 );

   lua_getfield( L, index, SUMMARY );
   const char *summary = luaL_optstring( L, -1, NULL );
   if( summary )
   {
      opts->summary = summary;
   }

   lua_getfield( L, index, AUTHOR );
   if( lua_type( L, -1 ) == LUA_TTABLE )
   {
      table_to_signature( L, (git_signature **)&opts->author, -1 );
   }

   return ret;
}

int luagi_diff_format_email( lua_State *L )
{
   git_diff **diff = checkdiff_at( L, 1 );

   git_buf out = GIT_BUF_INIT_CONST(NULL, 0);
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
   git_repository **repo = checkrepo( L, 1 );
   git_commit **commit = checkcommit_at( L, 2 );

   int patch_no = luaL_checkinteger( L, 3 );
   int total_patches = luaL_checkinteger( L, 4 );

   git_diff_format_email_flags_t flags = GIT_DIFF_FORMAT_EMAIL_NONE;

   if( lua_type( L, 5 ) == LUA_TTABLE )
   {
      // if available, the flags can be combined 
      // with the other options in the table
      add_flag( flags, L, 5, 
               EXCLUDE_SUBJECT_PATCH_MARKER, 
               GIT_DIFF_FORMAT_EMAIL_EXCLUDE_SUBJECT_PATCH_MARKER );
   
   }

   git_diff_options diff_opts;
   luagi_diff_init_options( L, 5, &diff_opts );

   git_buf out = GIT_BUF_INIT_CONST(NULL, 0);
   if( git_diff_commit_as_email( &out, *repo, *commit, patch_no, total_patches, flags, &diff_opts ) )
   {
      ERROR_PUSH( L )
   }

   lua_pushlstring( L, out.ptr, out.size );
   return 1;
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
      lua_setfield( L, idx, ID );
   }
}

void diff_file_to_table( lua_State *L, const git_diff_file file )
{
   lua_newtable( L );
   char buf [GIT_OID_HEXSZ+1];
   lua_pushstring( L, git_oid_tostr( buf, GIT_OID_HEXSZ+1, &(file.id) ) );
   lua_setfield( L, -2, ID );

   lua_pushstring( L, file.path );
   lua_setfield( L, -2, PATH );
  
   lua_pushinteger( L, file.size );
   lua_setfield( L, -2, SIZE );

   add_flags( L, -2, file.flags );

   lua_pushinteger( L, file.mode );
   lua_setfield( L, -2, MODE );

   debugStack( L );
}


void diff_delta_to_table( lua_State *L, const git_diff_delta *delta )
{
   lua_newtable( L );

   lua_pushinteger( L, delta->status );
   lua_setfield( L, -2, STATUS );

   add_flags( L, -2, delta->flags );

   lua_pushinteger( L, delta->similarity );
   lua_setfield( L, -2, SIMI );

   lua_pushinteger( L, delta->nfiles );
   lua_setfield( L, -2, NFILES );
   
   diff_file_to_table( L, delta->old_file );
   lua_setfield( L, -2, OLD_FILE );

   diff_file_to_table( L, delta->new_file );
   lua_setfield( L, -2, NEW_FILE );
  
   debugStack( L );

}

static git_delta_t delta_t_from_string( const char *type )
{
   git_delta_t t = GIT_DELTA_UNMODIFIED;
   if( strncmp( type, ADDED, strlen( ADDED ) ) )
   {
      t = GIT_DELTA_ADDED;
   }
   else if( strncmp( type, DELETED, strlen( DELETED ) ) )
   {
      t = GIT_DELTA_DELETED;
   }
   else if( strncmp( type, MODIFIED, strlen( MODIFIED ) ) )
   {
      t = GIT_DELTA_MODIFIED;
   }
   else if( strncmp( type, RENAMED, strlen( RENAMED ) ) )
   {
      t = GIT_DELTA_RENAMED;
   }
   else if( strncmp( type, COPIED, strlen( COPIED ) ) )
   {
      t = GIT_DELTA_COPIED;
   }
   else if( strncmp( type, IGNORED, strlen( IGNORED ) ) )
   {
      t = GIT_DELTA_IGNORED;
   }
   else if( strncmp( type, UNTRACKED, strlen( UNTRACKED ) ) )
   {
      t = GIT_DELTA_UNTRACKED;
   }
   else if( strncmp( type, TYPECHANGE, strlen( TYPECHANGE ) ) )
   {
      t = GIT_DELTA_TYPECHANGE;
   }
   return t;
}
