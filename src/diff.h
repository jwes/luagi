#ifndef IS_LGIT_DIFF
#define IS_LGIT_DIFF

#include <lua.h>
#include <lauxlib.h>
#include "git2/diff.h"

int lgit_diff_tree_to_tree( lua_State *L );
int lgit_diff_tree_to_index( lua_State *L );
int lgit_diff_index_to_workdir( lua_State *L );
int lgit_diff_tree_to_workdir( lua_State *L );
int lgit_diff_tree_to_workdir_with_index( lua_State *L );

// diff functions
int lgit_diff_merge( lua_State *L );
int lgit_diff_find_similar( lua_State *L );
int lgit_diff_num_deltas( lua_State *L ); // includes num deltas of type
int lgit_diff_get_delta( lua_State *L );
int lgit_diff_is_sorted_icase( lua_State *L );
int lgit_diff_foreach( lua_State *L );
int lgit_diff_print( lua_State *L );

//TODO git_diff_blob*
//TODO git_diff_buffer
//
static const struct luaL_Reg lgit_diff_funcs [] = {
   { "merge", lgit_diff_merge },
   { "find_similar", lgit_diff_find_similar },
   { "num_deltas", lgit_diff_num_deltas },
   { "get_delta", lgit_diff_get_delta },
   { "is_sorted", lgit_diff_is_sorted_icase },
   { "foreach", lgit_diff_foreach },
   { "__tostring", lgit_diff_print },
   { NULL, NULL }
};

//helper
void diff_file_to_table( lua_State *L, const git_diff_file file );
void diff_delta_to_table( lua_State *L, const git_diff_delta *delta );

#define LGIT_DIFF_FUNCS "is.westh.lgit.diff"
#define checkdiff_at(L, N) \
      (git_diff**) luaL_checkudata( L, N, LGIT_DIFF_FUNCS )
#endif // IS_LGIT_DIFF
