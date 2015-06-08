#ifndef LUAGI_ANNOTATED_COMMIT
#define LUAGI_ANNOTATED_COMMIT

#include <git2/annotated_commit.h>

#include <lua.h>
#include <lauxlib.h>

int luagi_annotated_commit_from_ref( lua_State *L );
int luagi_annotated_commit_from_fetchhead( lua_State *L );
int luagi_annotated_commit_lookup( lua_State *L );

int luagi_annotated_commit_id( lua_State *L );
int luagi_annotated_commit_free( lua_State *L );

void fill_annotated_commit( lua_State *L, int index, const git_annotated_commit **commits, int len );

#define LUAGI_ANNOTATED_COMMIT_FUNCS "is.luagi.annotated.commit"

#define check_annotated_commit_at( L, N ) \
   (git_annotated_commit **) luaL_checkudata( L, N, LUAGI_ANNOTATED_COMMIT_FUNCS )

static const struct luaL_Reg luagi_annotated_commit_funcs [] = {
   { "__gc", luagi_annotated_commit_free },
   { "__tostring", luagi_annotated_commit_id },
   { NULL, NULL }
};
#endif
