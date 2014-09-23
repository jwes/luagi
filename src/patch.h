#ifndef IS_LUAGI_PATCH
#define IS_LUAGI_PATCH
#include <lua.h>
#include <lauxlib.h>

int luagi_patch_from_diff( lua_State *L );
int luagi_patch_from_blobs( lua_State *L );
int luagi_patch_from_blob_and_buffer( lua_State *L );
int luagi_patch_from_buffers( lua_State *L );

int luagi_patch_free( lua_State *L );
int luagi_patch_get_delta( lua_State *L );
int luagi_patch_num_hunks( lua_State *L );
int luagi_patch_line_stats( lua_State *L );
int luagi_patch_get_hunk( lua_State *L );
int luagi_patch_num_lines_in_hunk( lua_State *L );
int luagi_patch_get_line_in_hunk( lua_State *L );
int luagi_patch_size( lua_State *L );
int luagi_patch_print( lua_State *L );
int luagi_patch_to_buf( lua_State *L );

#define LUAGI_PATCH_FUNCS "is.luagi.patch"

#define checkpatch( L ) \
   luaL_checkudata( L, 1, LUAGI_PATCH_FUNCS )

static const struct luaL_Reg luagi_patch_funcs [] = {
   { "__gc", luagi_patch_free },
   { "get_delta", luagi_patch_get_delta },
   { "num_hunks", luagi_patch_num_hunks },
   { "line_stats", luagi_patch_line_stats },
   { "get_hunk",  luagi_patch_get_hunk },
   { "num_lines_in_hunk", luagi_patch_num_lines_in_hunk },
   { "get_line_in_hunk", luagi_patch_get_line_in_hunk },
   { "size", luagi_patch_size },
   { "print", luagi_patch_print },
   { "to_buf",  luagi_patch_to_buf },
   { NULL, NULL }
};

#endif

