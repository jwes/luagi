#include <git2/patch.h>

#include "patch.h"
#include "diff.h"
#include "luagi.h"
#include "blob.h"

int luagi_patch_from_diff( lua_State *L )
{
   git_diff **diff = checkdiff_at( L, 1 );
   size_t idx = luaL_checkinteger( L, 2 );

   git_patch **out = lua_newuserdata( L, sizeof( git_patch *) );
   if( git_patch_from_diff( out, *diff, idx ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_PATCH_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_patch_from_blobs( lua_State *L )
{
   git_blob **old = check_blob_at( L, 1 );
   const char *old_as_path = luaL_optstring( L, 2, NULL );
   
   git_blob **new = check_blob_at( L, 3 );
   const char *new_as_path = luaL_optstring( L, 4, NULL );

   //TODO git_diff_options
   git_patch **out = lua_newuserdata( L, sizeof( git_patch *) );
   if( git_patch_from_blobs( out, *old, old_as_path, 
                               *new, new_as_path, NULL ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_PATCH_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_patch_from_blob_and_buffer( lua_State *L )
{
   git_blob **old = check_blob_at( L, 1 );
   const char *old_as_path = luaL_optstring( L, 2, NULL );
   size_t new_len = luaL_len( L, 3 );
   const char *new = luaL_checkstring( L, 3 );
   const char *new_as_path = luaL_optstring( L, 4, NULL );

   //TODO git_diff_options
   git_patch **out = lua_newuserdata( L, sizeof( git_patch *) );
   if( git_patch_from_blob_and_buffer( out, *old, old_as_path, 
                               new, new_len, new_as_path, NULL ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_PATCH_FUNCS );
   lua_setmetatable( L, -2 );
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

   //TODO git_diff_options
   git_patch **out = lua_newuserdata( L, sizeof( git_patch *) );
   if( git_patch_from_buffers( out, old, old_len, old_as_path, 
                               new, new_len, new_as_path, NULL ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_PATCH_FUNCS );
   lua_setmetatable( L, -2 );
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
   luaL_error( L, "not yet implemented" );
   //TODO export git_diff_delta to diff.h
   return 0;
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
      ERROR_PUSH( L )
   }
   lua_pushinteger( L, context );
   lua_pushinteger( L, additions );
   lua_pushinteger( L, deletions );
   return 3;
}

int luagi_patch_get_hunk( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
   //TODO export git_diff_hunk to diff.h
   return 0;
}
int luagi_patch_num_lines_in_hunk( lua_State *L )
{
   git_patch **patch = checkpatch( L );
   size_t hunk_idx = luaL_checkinteger( L, 2 );

   lua_pushinteger( L,  git_patch_num_lines_in_hunk( *patch, hunk_idx ) );
   return 1;
}

int luagi_patch_get_line_in_hunk( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
   //TODO export git_diff_line to diff.h
   return 0;
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
   
   struct foreach_f *f = malloc( sizeof( struct foreach_f ) );

   int ret = git_patch_print( *patch, diff_line_callback, f );
   free( f );
   if( ret )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_patch_to_buf( lua_State *L )
{
   git_patch **patch = checkpatch( L );

   git_buf out;
   if( git_patch_to_buf( &out, *patch ) )
   {
      ERROR_PUSH( L )
   }
   lua_pushlstring( L, out.ptr, out.size );
   return 1;
}

