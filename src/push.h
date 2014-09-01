#ifndef IS_LUAGI_PUSH
#define IS_LUAGI_PUSH
#include <lua.h>
#include <lauxlib.h>

int luagi_push_new( lua_State *L );
int luagi_push_set_options( lua_State *L );
int luagi_push_set_callbacks( lua_State *L );
int luagi_push_add_refspec( lua_State *L );
int luagi_push_update_tips( lua_State *L );
int luagi_push_finish( lua_State *L );
int luagi_push_unpack_ok( lua_State *L );

int luagi_push_status_foreach( lua_State *L );

int luagi_push_free( lua_State *L );

#define LUAGI_PUSH_FUNCS "is.luagi.push"

#define checkpush_at( L, N ) \
   (git_push **) luaL_checkudata( L, N, LUAGI_PUSH_FUNCS )

static const struct luaL_Reg luagi_push_funcs [] = {
   { "set_options", luagi_push_set_options },
   { "set_callbacks", luagi_push_set_callbacks },
   { "add_refspec", luagi_push_add_refspec },
   { "update_tips", luagi_push_update_tips },
   { "finish", luagi_push_finish },
   { "unpack_ok", luagi_push_unpack_ok },
   { "foreach_status", luagi_push_status_foreach },
   { "__gc", luagi_push_free },
   { NULL, NULL }
};

#endif
