#include "patch.h"

#include <git2/patch.h>

#include "blob.h"
#include "diff.h"
#include "ltk.h"
#include "luagi.h"

int luagi_patch_from_diff( lua_State *L )
{
   git_diff **diff = checkdiff_at( L, 1 );
   size_t idx = luaL_checkinteger( L, 2 );
   size_t size = git_diff_num_deltas( *diff );
   if( idx <= 0 || idx > size )
   {
      return ltk_push_error_msg( L, "index out of bounds" );
   }

   idx--;

   git_patch **out = lua_newuserdata( L, sizeof( git_patch *) );
   if( git_patch_from_diff( out, *diff, idx ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_PATCH_FUNCS );
   return 1;
}

int luagi_patch_from_blobs( lua_State *L )
{
   git_blob **old = check_blob_at( L, 1 );
   const char *old_as_path = luaL_optstring( L, 2, NULL );
   
   git_blob **new = check_blob_at( L, 3 );
   const char *new_as_path = luaL_optstring( L, 4, NULL );

   git_diff_options opts;
   luagi_diff_init_options( L, 5, &opts ); 

   git_patch **out = lua_newuserdata( L, sizeof( git_patch *) );
   if( git_patch_from_blobs( out, *old, old_as_path, 
                               *new, new_as_path, &opts ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_PATCH_FUNCS );
   return 1;
}

int luagi_patch_from_blob_and_buffer( lua_State *L )
{
   git_blob **old = check_blob_at( L, 1 );
   const char *old_as_path = luaL_optstring( L, 2, NULL );
   size_t new_len = luaL_len( L, 3 );
   const char *new = luaL_checkstring( L, 3 );
   const char *new_as_path = luaL_optstring( L, 4, NULL );

   git_diff_options opts;
   luagi_diff_init_options( L, 5, &opts ); 

   git_patch **out = lua_newuserdata( L, sizeof( git_patch *) );
   if( git_patch_from_blob_and_buffer( out, *old, old_as_path, 
                               new, new_len, new_as_path, &opts ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_PATCH_FUNCS );
   return 1;
}

int luagi_patch_from_buffers( lua_State *L )
{
   size_t old_len = luaL_len( L, 1 );
   const char *old = luaL_checkstring( L, 1 );
   const char *old_as_path = luaL_optstring( L, 2, NULL );
   size_t new_len = luaL_len( L, 3 );
   const char *new = luaL_checkstring( L, 3 );
   const char *new_as_path = luaL_optstring( L, 4, NULL );

   git_diff_options opts;
   luagi_diff_init_options( L, 5, &opts ); 

   git_patch **out = lua_newuserdata( L, sizeof( git_patch *) );
   if( git_patch_from_buffers( out, old, old_len, old_as_path, 
                               new, new_len, new_as_path, &opts ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_PATCH_FUNCS );
   return 1;
}

int luagi_patch_free( lua_State *L )
{
   git_patch **patch = checkpatch( L );
   git_patch_free( *patch );
   return 0;
}

int luagi_patch_get_delta( lua_State *L )
{
   git_patch **patch = checkpatch( L );

   const git_diff_delta *delta = git_patch_get_delta( *patch );
   diff_delta_to_table( L, delta );
   return 1;
}
   
int luagi_patch_num_hunks( lua_State *L )
{
   git_patch **patch = checkpatch( L );

   lua_pushinteger( L, git_patch_num_hunks( *patch ) );
   return 1;
}

int luagi_patch_line_stats( lua_State *L )
{
   git_patch **patch = checkpatch( L );
   size_t context, additions, deletions;
   if( git_patch_line_stats( &context, &additions, &deletions, *patch ) )
   {
      return ltk_push_git_error( L );
   }
   lua_pushinteger( L, context );
   lua_pushinteger( L, additions );
   lua_pushinteger( L, deletions );
   return 3;
}

int luagi_patch_get_hunk( lua_State *L )
{
   git_patch **patch = checkpatch( L );
   size_t hunk_idx = luaL_checkinteger( L, 2 );
   size_t size = git_patch_num_hunks( *patch );
   if( hunk_idx <= 0 || hunk_idx > size )
      return ltk_push_error_msg( L, "index out of bounds");

   hunk_idx--;

   const git_diff_hunk *out;
   size_t lines_in_hunk;

   if( git_patch_get_hunk( &out, &lines_in_hunk, *patch, hunk_idx ) )
   {
      return ltk_push_git_error( L );
   }

   diff_hunk_to_table( L, out );
   lua_pushinteger( L, lines_in_hunk );
   return 2;
}
int luagi_patch_num_lines_in_hunk( lua_State *L )
{
   git_patch **patch = checkpatch( L );
   size_t hunk_idx = luaL_checkinteger( L, 2 );
   size_t size = git_patch_num_hunks( *patch );
   if( hunk_idx <= 0 || hunk_idx > size )
      return ltk_push_error_msg( L, "index out of bounds");

   hunk_idx--;

   lua_pushinteger( L,  git_patch_num_lines_in_hunk( *patch, hunk_idx ) );
   return 1;
}

int luagi_patch_get_line_in_hunk( lua_State *L )
{
   git_patch **patch = checkpatch( L );

   size_t hunk_idx = luaL_checkinteger( L, 2 );
   size_t size = git_patch_num_hunks( *patch );
   if( hunk_idx <= 0 || hunk_idx > size )
      return ltk_push_error_msg( L, "index out of bounds");

   hunk_idx--;
   size_t line_of_hunk = luaL_checkinteger( L, 3 );
   size = git_patch_num_lines_in_hunk( *patch, hunk_idx );
   if( line_of_hunk <= 0 || line_of_hunk > size )
      return ltk_push_error_msg( L, "line index out of bounds");

   line_of_hunk--;

   const git_diff_line *out;

   if( git_patch_get_line_in_hunk( &out, *patch, hunk_idx, line_of_hunk ) )
   {
      return ltk_push_git_error( L );
   }

   diff_line_to_table( L, out );
   return 1;
}

int luagi_patch_size( lua_State *L )
{
   git_patch **patch = checkpatch( L );
   int include_context = lua_toboolean( L, 2 );
   int include_hunk_headers = lua_toboolean( L, 3 );
   int include_file_headers =  lua_toboolean( L, 4 );

   lua_pushinteger( L, git_patch_size( *patch, 
                                    include_context,
                                    include_hunk_headers,
                                    include_file_headers ) );
   return 1;
}

int luagi_patch_print( lua_State *L )
{
   git_patch **patch =checkpatch( L );
   
   luaL_checktype( L, 2, LUA_TFUNCTION );
   struct foreach_f *f = malloc( sizeof( struct foreach_f ) );
   f->L = L;
   f->use_lines = 2;

   int ret = git_patch_print( *patch, diff_line_callback, f );
   free( f );
   if( ret )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_patch_to_buf( lua_State *L )
{
   git_patch **patch = checkpatch( L );

   git_buf out = GIT_BUF_INIT_CONST(NULL, 0);
   if( git_patch_to_buf( &out, *patch ) )
   {
      return ltk_push_git_error( L );
   }
   lua_pushlstring( L, out.ptr, out.size );
   return 1;
}

