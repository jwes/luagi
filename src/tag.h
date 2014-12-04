#ifndef IS_LUAGI_TAG
#define IS_LUAGI_TAG
#include <lua.h>
#include <lauxlib.h>

// repo funcs
int luagi_tag_lookup( lua_State *L );
int luagi_tag_create( lua_State *L );
int luagi_tag_annotation_create( lua_State *L );
int luagi_tag_create_frombuffer( lua_State *L );
int luagi_tag_create_lightweight( lua_State *L );
int luagi_tag_delete( lua_State *L );

int luagi_tag_list( lua_State *L );
int luagi_tag_list_match( lua_State *L );
int luagi_tag_foreach( lua_State *L );

#define LUAGI_TAG_FUNCS "is.luagi.tag"

#define checktag_at( L, N ) \
   (git_tag **) luaL_checkudata( L, N, LUAGI_TAG_FUNCS );

// tag functions
int luagi_tag_free( lua_State *L );
int luagi_tag_id( lua_State *L );
int luagi_tag_owner( lua_State *L );
int luagi_tag_target( lua_State *L );
int luagi_tag_target_id( lua_State *L );
int luagi_tag_target_type( lua_State *L );
int luagi_tag_name( lua_State *L );
int luagi_tag_tagger( lua_State *L );
int luagi_tag_message( lua_State *L );
int luagi_tag_peel( lua_State *L );

static const struct luaL_Reg luagi_tag_funcs [] = {
   { "__gc", luagi_tag_free },
   { "id", luagi_tag_id },
   { "owner", luagi_tag_owner },
   { "target", luagi_tag_target },
   { "targer_id", luagi_tag_target_id },
   { "name", luagi_tag_name },
   { "__tostring", luagi_tag_name },
   { "tagger", luagi_tag_tagger },
   { "message", luagi_tag_message },
   { "peel", luagi_tag_peel },
   { NULL, NULL }
};

#endif
