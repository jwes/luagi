#ifndef IS_LGIT_INDEX
#define IS_LGIT_INDEX

#include <lua.h>
#include <lauxlib.h>
#include <git2/index.h>

int lgit_index_new( lua_State *L );
int lgit_index_free( lua_State *L );
int lgit_index_owner( lua_State *L );
int lgit_index_caps( lua_State *L );
int lgit_index_set_caps( lua_State *L );
int lgit_index_read( lua_State *L );
int lgit_index_write( lua_State *L );
int lgit_index_path( lua_State *L );
int lgit_index_read_tree( lua_State *L );
int lgit_index_write_tree( lua_State *L );
int lgit_index_write_tree_to( lua_State *L );
int lgit_index_entrycount( lua_State *L );
int lgit_index_clear( lua_State *L );
int lgit_index_get_byindex( lua_State *L );
int lgit_index_get_bypath( lua_State *L );
int lgit_index_remove( lua_State *L );
int lgit_index_remove_directory( lua_State *L );
int lgit_index_add( lua_State *L );
int lgit_index_entry_stage( lua_State *L );
int lgit_index_add_bypath( lua_State *L );
int lgit_index_remove_bypath( lua_State *L );
int lgit_index_add_all( lua_State *L );
int lgit_index_remove_all( lua_State *L );
int lgit_index_update_all( lua_State *L );
int lgit_index_find( lua_State *L );
int lgit_index_conflict_add( lua_State *L );
int lgit_index_conflict_get( lua_State *L );
int lgit_index_conflict_remove( lua_State *L );
int lgit_index_conflict_cleanup( lua_State *L );
int lgit_index_has_conflicts( lua_State *L );
// TODO conflict iterator
#define LGIT_INDEX_FUNCS "is.westhu.lgit.index"

#define checkindex_at(L, N) \
      (git_index**) luaL_checkudata( L, N, LGIT_INDEX_FUNCS )

static const struct luaL_Reg lgit_index_funcs [] = {
   { "owner",   lgit_index_owner },
   { "caps",   lgit_index_caps },
   { "set_caps",   lgit_index_set_caps },
   { "read",   lgit_index_read },
   { "write",   lgit_index_write },
   { "path",   lgit_index_path },
   { "read_tree",   lgit_index_read_tree },
   { "write_tree",   lgit_index_write_tree },
   { "__len",   lgit_index_entrycount },
   { "by_index",   lgit_index_get_byindex }, //TODO __inedx?
   { "clear",   lgit_index_clear },
   { "by_path",   lgit_index_get_bypath },
   { "remove",   lgit_index_remove },
   { "remove_directory",   lgit_index_remove_directory },
   { "add",   lgit_index_add },
   { "entry_stage",   lgit_index_entry_stage },
   { "add_by_path",   lgit_index_add_bypath },
   { "remove_by_path",   lgit_index_remove_bypath },
   { "add_all",   lgit_index_add_all },
   { "remove_all",   lgit_index_remove_all },
   { "update_all",   lgit_index_update_all },
   { "find",   lgit_index_find },
   { "conflict_add",   lgit_index_conflict_add },
   { "conflict_get",   lgit_index_conflict_get },
   { "conflict_remove",   lgit_index_conflict_remove },
   { "conflice_cleanup",   lgit_index_conflict_cleanup },
   { "has_conflicts",   lgit_index_has_conflicts },
   { "__gc",   lgit_index_free },
   { NULL, NULL }
};
#endif
