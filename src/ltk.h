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

inline int ltk_push_error_msg( lua_State *L, const char *msg )
{
   lua_pushnil( L );
   lua_pushstring( L, msg );
   return 2;
}

inline int ltk_push_error( lua_State *L )
{
   const git_error *err = giterr_last();
   return ltk_push_error_msg( L, err ? err->message : "unknown" );
}

inline void ltk_setmetatable( lua_State *L, const char *name )
{
   luaL_getmetatable( L, name );
   lua_setmetatable( L, -2 );
}

#endif
