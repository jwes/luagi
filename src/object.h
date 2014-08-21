#ifndef IS_LUAGI_OBJECT
#define IS_LUAGI_OBJECT
#include <lua.h>
#include <lauxlib.h>

//acquire
int luagi_object_lookup( lua_State *L );
int luagi_object_lookup_bypath( lua_State *L );

//use
int luagi_object_id( lua_State *L );
int luagi_object_type( lua_State *L );
int luagi_object_owner( lua_State *L );
int luagi_object_free( lua_State *L );

//types
int luagi_object_type2string( lua_State *L );
int luagi_object_string2type( lua_State *L );
int luagi_object_typeisloose( lua_State *L );
int luagi_object__size( lua_State *L );
int luagi_object_peel( lua_State *L );

#define LGIT_OBJECT_FUNCS "is.luagi.object"

#define checkobject_at(L, N) \
      (git_object**) luaL_checkudata( L, N, LGIT_OBJECT_FUNCS )

static const struct luaL_Reg lgit_index_funcs [] = {
   { "id",   luagi_object_id },
   { "type",   luagi_object_type },
   { "owner",   luagi_object_owner },
   { "__gc",   luagi_object_free },
   { NULL, NULL }
};
#endif
