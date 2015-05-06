#ifndef IS_LUAGI_TREE
#define IS_LUAGI_TREE

#include <git2/tree.h>
#include <lauxlib.h>
#include <lua.h>

#include "luagi.h"

int luagi_tree_lookup( lua_State *L );

int luagi_tree_id( lua_State *L );
int luagi_tree_entrycount( lua_State *L );

int luagi_tree_gc( lua_State *L );

/* tree entry genereation */

int luagi_tree_entry_byname( lua_State *L );
int luagi_tree_entry_byindex( lua_State *L );
int luagi_tree_entry_byid( lua_State *L );

/* user owned tree entries */
int luagi_tree_entry_bypath( lua_State *L );
int luagi_tree_entry_dup( lua_State *L );
int luagi_tree_entry_gc( lua_State *L );

/* entry stuff */
int luagi_tree_entry_name( lua_State *L );
int luagi_tree_entry_id( lua_State *L );
int luagi_tree_entry_type( lua_State *L );
int luagi_tree_entry_filemode( lua_State *L );
int luagi_tree_entry_filemode_raw( lua_State *L );
int luagi_tree_entry_to_object( lua_State *L );

int luagi_tree_entry_cmp( lua_State *L );

#define LUAGI_TREE_PRE "pre"
#define LUAGI_TREE_POST "post"

int luagi_tree_walk( lua_State *L );

/* treebuilder stuff */
int luagi_tree_builder_create( lua_State *L );
int luagi_tree_builder_gc( lua_State *L );
int luagi_tree_builder_clear( lua_State *L );
int luagi_tree_builder_get( lua_State *L );
int luagi_tree_builder_insert( lua_State *L );
int luagi_tree_builder_remove( lua_State *L );
int luagi_tree_builder_entry_count( lua_State *L );
int luagi_tree_builder_write( lua_State *L );
int luagi_tree_builder_filter( lua_State *L );

static const struct luaL_Reg luagi_tree_entry_funcs [] = {
   { "name", luagi_tree_entry_name },
   { "type", luagi_tree_entry_type },
   { "filemode", luagi_tree_entry_filemode },
   { "compare", luagi_tree_entry_cmp },
   { "id", luagi_tree_entry_id },
   { "__gc", luagi_tree_entry_gc },
   { NULL, NULL }
};

static const struct luaL_Reg luagi_tree_builder_funcs [] = {
   { "clear", luagi_tree_builder_clear },
   { "entry_count", luagi_tree_builder_entry_count },
   { "get", luagi_tree_builder_get },
   { "insert", luagi_tree_builder_insert },
   { "remove", luagi_tree_builder_remove },
   { "write", luagi_tree_builder_write },
   { "filter", luagi_tree_builder_filter },
   { "__gc", luagi_tree_builder_gc },
   { NULL, NULL }
};

static const struct luaL_Reg luagi_tree_funcs [] = {
   { "id", luagi_tree_id },
   { "entry_count", luagi_tree_entrycount },
   { "entry_byname", luagi_tree_entry_byname },
   { "entry_byid", luagi_tree_entry_byid },
   { "entry_byindex", luagi_tree_entry_byindex },
   { "entry_bypath", luagi_tree_entry_bypath },
   { "walk", luagi_tree_walk },
   { "create_builder", luagi_tree_builder_create },
   { "__gc", luagi_tree_gc },
   { NULL, NULL }
};

#define checktreeentry(L, N) \
      (git_tree_entry**) luaL_checkudata( L, N, LUAGI_TREE_ENTRY_FUNCS )
#endif
