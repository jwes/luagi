#ifndef IS_LUAGI_REFLOG
#define IS_LUAGI_REFLOG

#include <lua.h>
#include <lauxlib.h>
#include <git2/reflog.h>

// repo
int luagi_reflog_read( lua_State *L );
int luagi_reflog_rename( lua_State *L );
int luagi_reflog_delete( lua_State *L );

//reflog
int luagi_reflog_write( lua_State *L );
int luagi_reflog_append( lua_State *L );
int luagi_reflog_entrycount( lua_State *L );
int luagi_reflog_entry_byindex( lua_State *L );
int luagi_reflog_drop( lua_State *L );
int luagi_reflog_entry_totable( lua_State *L, const git_reflog_entry *entry );
int luagi_reflog_free( lua_State *L );

#define LUAGI_REFLOG_FUNCS "is.luagi.reflog"
#define checkreflog_at( L, N )\
   luaL_checkudata( L, N, LUAGI_REFLOG_FUNCS )

#define checkreflog( L ) \
   checkreflog_at( L, 1 )

static const struct luaL_Reg luagi_reflog_funcs [] = {
   { "__gc", luagi_reflog_free },
   { "write", luagi_reflog_write },
   { "append", luagi_reflog_append },
   { "count", luagi_reflog_entrycount },
   { "entry_by_index", luagi_reflog_entry_byindex }, // TODO __index ?
   { "drop", luagi_reflog_drop },
   { NULL, NULL }
};

#endif
