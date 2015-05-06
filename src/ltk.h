#ifndef IS_LUAGI_LTK_H
#define IS_LUAGI_LTK_H

#include <git2/errors.h>
#include <lua.h>
#include <lauxlib.h>

inline void ltk_error_abort( lua_State *L )
{
   const git_error *err = giterr_last();
   luaL_error( L, err ? err->message : "unknown" );
}

inline int ltk_push_error( lua_State *L )
{
   const git_error *err = giterr_last();
   lua_pushnil( L );
   lua_pushstring( L, err ? err->message : "unknown" );
   return 2;
}

#endif
