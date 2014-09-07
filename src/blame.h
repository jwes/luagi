#ifndef IS_LUAGI_BLAME
#define IS_LUAGI_BLAME
#include <lua.h>
#include <lauxlib.h>
//repository
int luagi_blame_file( lua_State *L );

int luagi_blame_buffer( lua_State *L );
int luagi_blame_get_hunk_count( lua_State *L );
int luagi_blame_get_hunk_byindex( lua_State *L );
int luagi_blame_get_hunk_byline( lua_State *L );

int luagi_blame_free( lua_State *L );

#define LUAGI_BLAME_FUNCS "is.luagi.blame"

#define checkblame_at( L, N ) \
   (git_blame **) luaL_checkudata( L, N, LUAGI_BLAME_FUNCS )

static const struct luaL_Reg luagi_blame_funcs [] = {
   { "__gc", luagi_blame_free },
   { "buffer", luagi_blame_buffer },
   { "count", luagi_blame_get_hunk_count },
   { "byindex", luagi_blame_get_hunk_byindex },
   { "byline", luagi_blame_get_hunk_byline },
   { NULL, NULL }
};

#endif
