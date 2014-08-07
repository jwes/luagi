#include "diff.h"
#include "wien.h"
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

void options_from_lua( lua_State *L, int idx, git_diff_options *opts )
{
   lua_isboolean( L, idx );
   git_diff_init_options( opts, GIT_DIFF_OPTIONS_VERSION );
}

int lgit_diff_tree_to_tree( lua_State *L )
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

   luaL_getmetatable(L, LGIT_DIFF_FUNCS);
   lua_setmetatable(L, -2);

   return 1;
}

int lgit_diff_tree_to_index( lua_State *L )
{
//TODO index
   lua_pushnil( L );
   return 1; 
}

int lgit_diff_index_to_workdir( lua_State *L )
{
//TODO index
   lua_pushnil( L );
   return 1; 
}
int lgit_diff_tree_to_workdir( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_tree **tree = checktree_at( L, 2 );

   git_diff_options opts;
   options_from_lua( L, 3, &opts );
   
   git_diff **diff = (git_diff **) lua_newuserdata( L, sizeof( git_diff *) );

   if( git_diff_tree_to_workdir( diff, *repo, *tree, &opts ) )
   {
      const git_error *err = giterr_last();
      lua_pushnil( L );
      lua_pushstring( L, err->message );
      return 2;
   }

   luaL_getmetatable(L, LGIT_DIFF_FUNCS);
   lua_setmetatable(L, -2);

   return 1;

   lua_pushnil( L );
   return 1; 
}
int lgit_diff_tree_to_workdir_with_index( lua_State *L )
{
//TODO index
   lua_pushnil( L );
   return 1; 
}

// diff functions
int lgit_diff_merge( lua_State *L )
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

int lgit_diff_find_similar( lua_State *L )
{
   //TODO find options
   lua_pushnil( L );
   return 1;
}
int lgit_diff_num_deltas( lua_State *L )
{ 
   git_diff **diff = checkdiff_at( L, 1 );
   //TODO: type parameter
   lua_pushinteger( L, git_diff_num_deltas( *diff ) );
   return 1; 
} // includes num deltas of type

int lgit_diff_get_delta( lua_State *L )
{
   git_diff **diff = checkdiff_at( L, 1 );
   int idx = luaL_checkinteger( L, 2 );
   const git_diff_delta *delta = git_diff_get_delta( *diff, idx );

   diff_delta_to_table( L, delta );
   return 1; 
}
int lgit_diff_is_sorted_icase( lua_State *L )
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
   return luaL_checkinteger( f->L, -1 );
}

static int diff_hunk_callback( const git_diff_delta *delta,
                const git_diff_hunk *hunk,
                void *payload)
{
   struct foreach_f *f = payload;
   if( ! f->use_hunks )
      return 1;

   lua_pushvalue( f->L, 3 );
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
   return luaL_checkinteger( f->L, -1 );
}

static int diff_line_callback( const git_diff_delta *delta,
                const git_diff_hunk *hunk,
                const git_diff_line *line,
                void *payload)
{
   struct foreach_f *f = payload;

   if( ! f->use_lines )
      return 1;

   lua_pushvalue( f->L, 4 );
   diff_delta_to_table( f->L, delta );

   lua_pushstring( f->L, hunk->header );
   lua_pushfstring( f->L, "%c", line->origin ); 
   lua_pushinteger( f->L, line->old_lineno );
   lua_pushinteger( f->L, line->new_lineno );
   lua_pushinteger( f->L, line->num_lines );
   
   char content [ line->content_len + 1 ];
   memcpy( content, line->content, line->content_len );
   content[ line->content_len ] = 0;
   lua_pushstring( f->L, content ); 

   if( lua_pcall( f->L, 7, 1, 0 ) )
   {
      luaL_error( f->L, "can not call line callback" );
   }
   return luaL_checkinteger( f->L, -1 );
}

int lgit_diff_foreach( lua_State *L )
{
   git_diff **diff = checkdiff_at( L, 1 );

   luaL_checktype( L, 2, LUA_TFUNCTION );
   
   struct foreach_f *f = malloc( sizeof( struct foreach_f) );
   f->L = L;
   f->use_hunks = lua_type( L, 3) == LUA_TFUNCTION ;
   f->use_lines = lua_type( L, 4) == LUA_TFUNCTION ;

   git_diff_foreach( *diff,
                   diff_file_callback,
                   diff_hunk_callback,
                   diff_line_callback,
                   f );

   free( f );

   return 0;
}
int lgit_diff_print( lua_State *L )
{
   //TODO callbacks
   lua_pushnil( L );
   return 1; 
}

int lgit_diff_free( lua_State *L )
{
   git_diff **diff = checkdiff_at( L, 1 );
   git_diff_free( *diff );
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

