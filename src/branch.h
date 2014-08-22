#ifndef IS_LUAGI_BRANCH
#define IS_LUAGI_BRANCH
#include <lua.h>

#define LUAGI_BRANCH_FUNCS "is.westh.luagi.branch"
#define LUAGI_BRANCH_STATICS "is.westhu.luagi.branch.iter"

int luagi_create_branch( lua_State *L );
int luagi_branch_lookup( lua_State *L );
int luagi_branch_gc( lua_State *L );

int luagi_branch_name( lua_State *L );
int luagi_delete_branch( lua_State *L );
int luagi_move_branch( lua_State *L );

int luagi_branch_upstream_get( lua_State *L );
int luagi_branch_upstream_set( lua_State *L );

int luagi_branches( lua_State *L );
int luagi_branch_iter_gc( lua_State *L );

int luagi_branch_is_head( lua_State *L );

static const struct luaL_Reg luagi_branch_funcs [] = {
   { "delete", luagi_delete_branch },
   { "move", luagi_move_branch },
   { "get_upstream", luagi_branch_upstream_get },
   { "set_upstream", luagi_branch_upstream_set },
   { "name", luagi_branch_name },
   { "is_head", luagi_branch_is_head },
   { "__gc", luagi_branch_gc },
   { NULL, NULL }
};
#endif
