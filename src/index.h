/*
 * MIT License
 * 
 * Copyright (c) 2016 Johannes Westhuis, Alexander Schulz
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef IS_LUAGI_INDEX
#define IS_LUAGI_INDEX

#include <lua.h>
#include <lauxlib.h>
#include <git2/index.h>

int luagi_index_new( lua_State *L );
int luagi_index_free( lua_State *L );
int luagi_index_caps( lua_State *L );
int luagi_index_set_caps( lua_State *L );
int luagi_index_read( lua_State *L );
int luagi_index_write( lua_State *L );
int luagi_index_path( lua_State *L );
int luagi_index_read_tree( lua_State *L );
int luagi_index_write_tree( lua_State *L );
int luagi_index_write_tree_to( lua_State *L );
int luagi_index_entrycount( lua_State *L );
int luagi_index_clear( lua_State *L );
int luagi_index_get_byindex( lua_State *L );
int luagi_index_get_bypath( lua_State *L );
int luagi_index_remove( lua_State *L );
int luagi_index_remove_directory( lua_State *L );
int luagi_index_add( lua_State *L );
int luagi_index_entry_stage( lua_State *L );
int luagi_index_add_bypath( lua_State *L );
int luagi_index_remove_bypath( lua_State *L );
int luagi_index_add_all( lua_State *L );
int luagi_index_remove_all( lua_State *L );
int luagi_index_update_all( lua_State *L );
int luagi_index_find( lua_State *L );
int luagi_index_conflict_add( lua_State *L );
int luagi_index_conflict_get( lua_State *L );
int luagi_index_conflict_remove( lua_State *L );
int luagi_index_conflict_cleanup( lua_State *L );
int luagi_index_has_conflicts( lua_State *L );
int luagi_index_conflict_iterator( lua_State *L );
int luagi_index_conflict_free( lua_State *L );

#define LUAGI_INDEX_FUNCS "is.westhu.luagi.index"
#define LUAGI_INDEX_CONFLICT_FUNCS "is.westhu.luagi.index.conflict"

#define checkindex_at(L, N) \
      (git_index**) luaL_checkudata( L, N, LUAGI_INDEX_FUNCS )

int luagi_check_index_entry( git_index_entry *entry, lua_State *L, const int index);
int luagi_push_index_entry( lua_State *L, const git_index_entry *entry );

#define checkindexconflict_at(L, N) \
      ( git_index_conflict_iterator**) luaL_checkudata( L, N, LUAGI_INDEX_CONFLICT_FUNCS )

static const struct luaL_Reg luagi_index_funcs [] = {
   { "caps",   luagi_index_caps },
   { "set_caps",   luagi_index_set_caps },
   { "read",   luagi_index_read },
   { "write",   luagi_index_write },
   { "path",   luagi_index_path },
   { "read_tree",   luagi_index_read_tree },
   { "write_tree",   luagi_index_write_tree },
   { "__len",   luagi_index_entrycount },
   { "by_index",   luagi_index_get_byindex }, //TODO __inedx?
   { "clear",   luagi_index_clear },
   { "by_path",   luagi_index_get_bypath },
   { "remove",   luagi_index_remove },
   { "remove_directory",   luagi_index_remove_directory },
   { "add",   luagi_index_add },
   { "add_by_path",   luagi_index_add_bypath },
   { "remove_by_path",   luagi_index_remove_bypath },
   { "add_all",   luagi_index_add_all },
   { "remove_all",   luagi_index_remove_all },
   { "update_all",   luagi_index_update_all },
   { "find",   luagi_index_find },
   { "conflict_add",   luagi_index_conflict_add },
   { "conflict_get",   luagi_index_conflict_get },
   { "conflict_remove",   luagi_index_conflict_remove },
   { "conflict_cleanup",   luagi_index_conflict_cleanup },
   { "has_conflicts",   luagi_index_has_conflicts },
   { "iterate_conflict", luagi_index_conflict_iterator },
   { "__gc",   luagi_index_free },
   { NULL, NULL }
};

static const struct luaL_Reg luagi_index_conflict_funcs [] = {
   { "__gc", luagi_index_conflict_free },
   { NULL, NULL }
};
#endif
