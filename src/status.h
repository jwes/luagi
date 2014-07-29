#ifndef IS_LGIT_STATUS
#define IS_LGIT_STATUS

#include <lua.h>
#include <lauxlib.h>

//function on repos
int lgit_status_foreach( lua_State *L );
int lgit_status_foreach_ext( lua_State *L );
int lgit_status_file ( lua_State *L );
int lgit_status_list_new ( lua_State *L );
int lgit_status_should_ignore( lua_State *L );

// list operations
int lgit_status_list_entrycount( lua_State *L );
int lgit_status_by_index( lua_State *L );

int lgit_status_list_free( lua_State *L );


static const struct luaL_Reg lgit_status_funcs [] = {
   { "__index", lgit_status_by_index },
   { "__len", lgit_status_list_entrycount },
   { "__gc", lgit_status_list_free },
   { NULL, NULL }
};

#define LGIT_STATUS_FUNCS "is.westh.lgit.status"
#define LGIT_STATUS_FOREACH "is.westh.lgit.status.foreach"

#define checkstatuslist(L) \
      (git_status_list **) luaL_checkudata( L, 1, LGIT_STATUS_FUNCS )
#endif //IS_LGIT_STATUS
