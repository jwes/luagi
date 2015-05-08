#ifndef IS_LUAGI_LTK_H
#define IS_LUAGI_LTK_H

#include <git2/errors.h>
#include <git2/strarray.h>
#include <lua.h>
#include <lauxlib.h>

/*******************************************************************************
 lua util functions
*******************************************************************************/

inline void ltk_setmetatable( lua_State *L, const char *name )
{
   luaL_getmetatable( L, name );
   lua_setmetatable( L, -2 );
}

inline int ltk_push_error_msg( lua_State *L, const char *msg )
{
   lua_pushnil( L );
   lua_pushstring( L, msg );
   return 2;
}

void ltk_dump_stack( lua_State* L );

#ifdef STACK_DEBUG
#define ltk_debug_stack( L ) ltk_dump_stack( L )
#else
#define ltk_debug_stack( L )
#endif

/*******************************************************************************
 lua+libgit2 util functions
*******************************************************************************/

inline void ltk_error_abort( lua_State *L )
{
   const git_error *err = giterr_last();
   luaL_error( L, err ? err->message : "unknown" );
}

inline int ltk_push_error( lua_State *L )
{
   const git_error *err = giterr_last();
   return ltk_push_error_msg( L, err ? err->message : "unknown" );
}

void ltk_push_strarray( lua_State *L, git_strarray array );

git_strarray ltk_check_strarray( lua_State* L, int table_idx );

#endif
