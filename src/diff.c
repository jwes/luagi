#include "diff.h"
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

int lgit_diff_tree_to_index( lua_State *L )
{
   lua_pushnil( L );
   return 1; 
}

int lgit_diff_index_to_workdir( lua_State *L )
{
   lua_pushnil( L );
   return 1; 
}
int lgit_diff_tree_to_workdir( lua_State *L )
{
   lua_pushnil( L );
   return 1; 
}
int lgit_diff_tree_to_workdir_with_index( lua_State *L )
{
   lua_pushnil( L );
   return 1; 
}

// diff functions
int lgit_diff_merge( lua_State *L )
{
   lua_pushnil( L );
   return 1; 
}
int lgit_diff_find_similar( lua_State *L )
{
   lua_pushnil( L );
   return 1;
}
int lgit_diff_num_deltas( lua_State *L )
{ 
   lua_pushnil( L );
   return 1; 
} // includes num deltas of type

int lgit_diff_get_delta( lua_State *L )
{
   lua_pushnil( L );
   return 1; 
}
int lgit_diff_is_sorted_icase( lua_State *L )
{
   lua_pushnil( L );
   return 1; 
}
int lgit_diff_foreach( lua_State *L )
{
   lua_pushnil( L );
   return 1;
}
int lgit_diff_print( lua_State *L )
{
   lua_pushnil( L );
   return 1; 
}
void add_flags( lua_State *L, int idx, int32_t flags )
{
   switch( flags )
   {
   case GIT_DIFF_FLAG_BINARY:
      lua_pushstring( L, BINARY );
      break;
   case GIT_DIFF_FLAG_NOT_BINARY:
   default:
      lua_pushstring( L, NOT_BINARY );
      break;
   case GIT_DIFF_FLAG_VALID_ID:
      lua_pushstring( L, VALID_ID );
      break;
   }
   lua_setfield( L, idx, DIFF_FILE_FLAGS );
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
}

