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

int luagi_git_diff_blobs( lua_State *L );
int luagi_git_diff_blob_to_buffer( lua_State *L );
int luagi_git_diff_buffers( lua_State *L );

//stats
int luagi_diff_get_stats( lua_State *L );
int luagi_diff_stats_files_changed( lua_State *L );
int luagi_diff_stats_insertions( lua_State *L );
int luagi_diff_stats_deletions( lua_State *L );
int luagi_diff_stats_to_buf( lua_State *L );
int luagi_diff_stats_free( lua_State *L );

int luagi_diff_format_email( lua_State *L );
int luagi_diff_commit_as_email( lua_State *L );

extern int luagi_patch_from_diff( lua_State *L );

static const struct luaL_Reg luagi_diff_funcs [] = {
   { "merge", luagi_diff_merge },
   { "find_similar", luagi_diff_find_similar },
   { "num_deltas", luagi_diff_num_deltas },
   { "get_delta", luagi_diff_get_delta },
   { "is_sorted", luagi_diff_is_sorted_icase },
   { "foreach", luagi_diff_foreach },
   { "print", luagi_diff_print },
   { "format_email", luagi_diff_format_email },
   { "get_stats", luagi_diff_get_stats },
   { "patch", luagi_patch_from_diff },
   { "__gc", luagi_diff_free },
   { NULL, NULL }
};

//helper
void diff_file_to_table( lua_State *L, const git_diff_file file );
void diff_delta_to_table( lua_State *L, const git_diff_delta *delta );
void diff_hunk_to_table( lua_State *L, const git_diff_hunk *hunk );
void diff_line_to_table( lua_State *L, const git_diff_line *line );
int luagi_diff_init_options( lua_State *L, int index, git_diff_options *opts );

struct foreach_f
{
   lua_State *L;
   int use_hunks;
   int use_lines;
};

int diff_line_callback( const git_diff_delta *delta,
                const git_diff_hunk *hunk,
                const git_diff_line *line,
                void *payload);
#define LUAGI_DIFF_FUNCS "is.westh.luagi.diff"
#define checkdiff_at(L, N) \
      (git_diff**) luaL_checkudata( L, N, LUAGI_DIFF_FUNCS )

static const struct luaL_Reg luagi_diff_stats_funcs [] = {
   { "files_changed", luagi_diff_stats_files_changed },
   { "insertions", luagi_diff_stats_insertions },
   { "deletions", luagi_diff_stats_deletions },
   { "__tostring", luagi_diff_stats_to_buf },
   { "__gc", luagi_diff_stats_free },
   { NULL, NULL }
};

#define LUAGI_DIFF_STATS_FUNCS "is.westh.luagi.diff.stats"
#define checkdiffstats_at(L, N) \
      (git_diff_stats**) luaL_checkudata( L, N, LUAGI_DIFF_STATS_FUNCS )
#endif // IS_LUAGI_DIFF
