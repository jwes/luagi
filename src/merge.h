#ifndef IS_LUAGI_MERGE
#define IS_LUAGI_MERGE
#include <lua.h>
#include <lauxlib.h>
#include <git2/merge.h>

int luagi_merge_analysis( lua_State *L );
int luagi_merge_base( lua_State *L );
int luagi_merge_base_many( lua_State *L );
int luagi_merge_base_octopus( lua_State *L );

int luagi_merge_file( lua_State *L );
int luagi_merge_file_from_index( lua_State *L );
int luagi_merge_file_result_free( lua_State *L );

int luagi_merge_trees( lua_State *L );
int luagi_merge_commits( lua_State *L );
int luagi_merge( lua_State *L );

int luagi_merge_init_options( lua_State *L, int index, git_merge_options *opts );
#endif
