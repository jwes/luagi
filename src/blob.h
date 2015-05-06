#ifndef IS_LUAGI_BLOB
#define IS_LUAGI_BLOB
#include <lua.h>
#include <lauxlib.h>
//repo
int luagi_blob_lookup( lua_State *L );
int luagi_blob_create_fromworkdir( lua_State *L );
int luagi_blob_create_fromdisk( lua_State *L );
int luagi_blob_create_fromchunks( lua_State *L );
int luagi_blob_create_frombuffer( lua_State *L );

// blob
int luagi_blob_free( lua_State *L );
int luagi_blob_id( lua_State *L );
int luagi_blob_rawsize( lua_State *L );
int luagi_blob_rawcontent( lua_State *L );

int luagi_blob_filtered_content( lua_State *L );
int luagi_blob_is_binary( lua_State *L );

#define LUAGI_BLOB_FUNCS "is.luagi.blob"

#define check_blob_at(L, N) \
   (git_blob **) luaL_checkudata( L, N, LUAGI_BLOB_FUNCS );

extern int luagi_patch_from_blobs( lua_State *L );
extern int luagi_patch_from_blob_and_buffer( lua_State *L );

static const struct luaL_Reg luagi_blob_funcs [] = {
   { "__gc", luagi_blob_free },
   { "__tostring", luagi_blob_id },
   { "id", luagi_blob_id },
   { "filtered_content", luagi_blob_filtered_content },
   { "is_binary", luagi_blob_is_binary },
   { "raw_size", luagi_blob_rawsize },
   { "raw_content", luagi_blob_rawcontent },
   { "patch", luagi_patch_from_blobs },
   { "patch_with_buffer", luagi_patch_from_blob_and_buffer },
   { NULL, NULL }
};

#endif
