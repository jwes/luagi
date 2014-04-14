#ifndef IS_LGIT_TREE
#define IS_LGIT_TREE
#include <lua.h>

int lgit_tree_lookup( lua_State *L );
int lgit_tree_lookup_prefix( lua_State *L );

int lgit_tree_id( lua_State *L );
int lgit_tree_entrycount( lua_State *L );

int lgit_tree_gc( lua_State *L );

/* tree entry genereation */

int lgit_tree_entry_byname( lua_State *L );
int lgit_tree_entry_byindex( lua_State *L );
int lgit_tree_entry_byid( lua_State *L );

/* user owned tree entries */
int lgit_tree_entry_bypath( lua_State *L );
int lgit_tree_entry_dup( lua_State *L );
int lgit_tree_entry_gc( lua_State *L );

/* entry stuff */
int lgit_tree_entry_name( lua_State *L );
int lgit_tree_entry_id( lua_State *L );
int lgit_tree_entry_type( lua_State *L );
int lgit_tree_entry_filemode( lua_State *L );
int lgit_tree_entry_filemode_raw( lua_State *L );

int lgit_tree_entry_cmp( lua_State *L );
// to_object


/* treebuilder stuff */
int lgit_tree_builder_create( lua_State *L );

#endif
