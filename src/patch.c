#include <git2/patch.h>

#include "patch.h"
#include "diff.h"
#include "luagi.h"

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
