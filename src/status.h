#ifndef IS_LUAGI_STATUS
#define IS_LUAGI_STATUS

#include <lua.h>
#include <lauxlib.h>

//function on repos
int luagi_status_foreach( lua_State *L );
int luagi_status_foreach_ext( lua_State *L );
int luagi_status_file ( lua_State *L );
int luagi_status_list_new ( lua_State *L );
int luagi_status_should_ignore( lua_State *L );

// list operations
int luagi_status_list_entrycount( lua_State *L );
int luagi_status_by_index( lua_State *L );

int luagi_status_list_free( lua_State *L );

static const struct luaL_Reg luagi_status_funcs [] = {
   { "__index", luagi_status_by_index },
   { "__len", luagi_status_list_entrycount },
   { "__gc", luagi_status_list_free },
   { NULL, NULL }
};

#define LUAGI_STATUS_FUNCS "is.westh.luagi.status"
#define LUAGI_STATUS_FOREACH "is.westh.luagi.status.foreach"

#define checkstatuslist(L) \
      (git_status_list **) luaL_checkudata( L, 1, LUAGI_STATUS_FUNCS )
#endif //IS_LUAGI_STATUS
