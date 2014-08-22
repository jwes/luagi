#ifndef IS_LUAGI_DIFF
#define IS_LUAGI_DIFF

#include <lua.h>
#include <lauxlib.h>
#include "git2/diff.h"

int luagi_diff_tree_to_tree( lua_State *L );
int luagi_diff_tree_to_index( lua_State *L );
int luagi_diff_index_to_workdir( lua_State *L );
int luagi_diff_tree_to_workdir( lua_State *L );
int luagi_diff_tree_to_workdir_with_index( lua_State *L );

// diff functions
int luagi_diff_merge( lua_State *L );
int luagi_diff_find_similar( lua_State *L );
int luagi_diff_num_deltas( lua_State *L ); // includes num deltas of type
int luagi_diff_get_delta( lua_State *L );
int luagi_diff_is_sorted_icase( lua_State *L );
int luagi_diff_foreach( lua_State *L );
int luagi_diff_print( lua_State *L );

int luagi_diff_free( lua_State *L );

//TODO git_diff_blob*
//TODO git_diff_buffer
//
static const struct luaL_Reg luagi_diff_funcs [] = {
   { "merge", luagi_diff_merge },
   { "find_similar", luagi_diff_find_similar },
   { "num_deltas", luagi_diff_num_deltas },
   { "get_delta", luagi_diff_get_delta },
   { "is_sorted", luagi_diff_is_sorted_icase },
   { "foreach", luagi_diff_foreach },
   { "print", luagi_diff_print },
   { "__gc", luagi_diff_free },
   { NULL, NULL }
};

//helper
void diff_file_to_table( lua_State *L, const git_diff_file file );
void diff_delta_to_table( lua_State *L, const git_diff_delta *delta );

#define LUAGI_DIFF_FUNCS "is.westh.luagi.diff"
#define checkdiff_at(L, N) \
      (git_diff**) luaL_checkudata( L, N, LUAGI_DIFF_FUNCS )
#endif // IS_LUAGI_DIFF
