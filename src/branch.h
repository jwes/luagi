#ifndef IS_LGIT_BRANCH
#define IS_LGIT_BRANCH
#include <lua.h>

int lgit_create_branch( lua_State *L );
int lgit_branch_lookup( lua_State *L );
int lgit_branch_gc( lua_State *L );

int lgit_branch_name( lua_State *L );
int lgit_delete_branch( lua_State *L );
int lgit_move_branch( lua_State *L );

int lgit_branch_upstream_get( lua_State *L );
int lgit_branch_upstream_set( lua_State *L );

int lgit_branches( lua_State *L );
int lgit_branch_iter_gc( lua_State *L );

int lgit_branch_is_head( lua_State *L );

static const struct luaL_Reg lgit_branch_funcs [] = {
   { "delete", lgit_delete_branch },
   { "move", lgit_move_branch },
   { "get_upstream", lgit_branch_upstream_get },
   { "set_upstream", lgit_branch_upstream_set },
   { "name", lgit_branch_name },
   { "is_head", lgit_branch_is_head },
   { "__gc", lgit_branch_gc },
   { NULL, NULL }
};
#endif
