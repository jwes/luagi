#ifndef IS_LUAGI_BRANCH
#define IS_LUAGI_BRANCH
#include <lua.h>

#define LUAGI_BRANCH_FUNCS "is.westh.luagi.branch"
#define LUAGI_BRANCH_STATICS "is.westhu.luagi.branch.iter"

int luagi_reference_branch_target( lua_State *L );
int luagi_reference_branch_target_peel( lua_State *L );
int luagi_reference_branch_symbolic_target( lua_State *L );
int luagi_reference_branch_type( lua_State *L );
int luagi_reference_branch_resolve( lua_State *L );
int luagi_reference_branch_owner( lua_State *L );
int luagi_reference_branch_symbolic_set_target( lua_State *L );
int luagi_reference_branch_set_target( lua_State *L );
int luagi_reference_branch_rename( lua_State *L );
int luagi_reference_branch_foreach( lua_State *L );
int luagi_reference_branch_is_branch( lua_State *L );
int luagi_reference_branch_is_remote( lua_State *L );
int luagi_reference_branch_is_tag( lua_State *L );
int luagi_reference_branch_is_note( lua_State *L );
int luagi_reference_branch_peel( lua_State *L );
int luagi_reference_branch_shorthand( lua_State *L );
int luagi_reference_branch_free( lua_State *L );


int luagi_create_branch( lua_State *L );
int luagi_branch_lookup( lua_State *L );

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
   { "target", luagi_reference_branch_target },
   { "target_peel", luagi_reference_branch_target_peel },
   { "symbolic_target", luagi_reference_branch_symbolic_target },
   { "type", luagi_reference_branch_type },
   { "resolve", luagi_reference_branch_resolve },
   { "owner", luagi_reference_branch_owner },
   { "set_symbolic_target", luagi_reference_branch_symbolic_set_target },
   { "set_target", luagi_reference_branch_set_target },
   { "rename", luagi_reference_branch_rename },
   { "is_branch", luagi_reference_branch_is_branch },
   { "is_remote", luagi_reference_branch_is_remote },
   { "is_tag",  luagi_reference_branch_is_tag },
   { "is_note", luagi_reference_branch_is_note },
   { "peel", luagi_reference_branch_peel },
   { "shorthand", luagi_reference_branch_shorthand },
   { "__gc", luagi_reference_branch_free },
   { NULL, NULL }
};
#endif
