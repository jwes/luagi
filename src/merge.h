#ifndef IS_LUAGI_MERGE
#define IS_LUAGI_MERGE
#include <lua.h>
#include <lauxlib.h>

int luagi_merge_analysis( lua_State *L );
int luagi_merge_base( lua_State *L );
int luagi_merge_base_many( lua_State *L );
int luagi_merge_base_octopus( lua_State *L );

int luagi_merge_head_from_ref( lua_State *L );
int luagi_merge_head_from_fetchhead( lua_State *L );
int luagi_merge_head_from_id( lua_State *L );

int luagi_merge_head_id( lua_State *L );
int luagi_merge_head_free( lua_State *L );

int luagi_merge_file( lua_State *L );
int luagi_merge_file_from_index( lua_State *L );
int luagi_merge_file_result_free( lua_State *L );

int luagi_merge_trees( lua_State *L );
int luagi_merge_commits( lua_State *L );
int luagi_merge( lua_State *L );

#define LUAGI_MERGEHEAD_FUNCS "is.luagi.merge.head"

#define check_mergehead_at( L, N ) \
   (git_merge_head **) luaL_checkudata( L, N, LUAGI_MERGEHEAD_FUNCS )

static const struct luaL_Reg luagi_mergehead_funcs [] = {
   { "__gc", luagi_merge_head_free },
   { "__tostring", luagi_merge_head_id },
   { NULL, NULL }
};

#endif
