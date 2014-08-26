#ifndef IS_LUAGI_H
#define IS_LUAGI_H
#include <git2/types.h>
#include <git2/oid.h>
#include <git2/strarray.h>
#include <git2/errors.h>

#define REPO_NAME "is.westhu.luagi.repo"
#define LUAGI_TREE_FUNCS "is.westh.luagi.tree"
#define LUAGI_TREE_ENTRY_FUNCS "is.westh.luagi.tree.entry"
#define LUAGI_TREE_BUILDER_FUNCS "is.westh.luagi.tree.builder"
#define LUAGI_TREE_WALK_STATICS "is.westhu.luagi.tree.walk"
#define LUAGI_COMMIT_FUNCS "is.westh.luagi.commit"
#define LUAGI_REMOTE_FUNCS "is.westh.luagi.remote"

#define SIG_NAME "name"
#define SIG_EMAIL "email"
#define SIG_TIME "time"
#define SIG_TIME_OFF "time_offset"

#define ERROR_ABORT( L ) \
   const git_error *err = giterr_last(); \
   luaL_error( L, err->message ); 

#define ERROR_PUSH( L ) \
   const git_error *err = giterr_last(); \
   lua_pushnil( L ); \
   lua_pushstring( L, err->message ); \
   return 2;

#define checkrepo(L, n) \
      (git_repository**) luaL_checkudata( L, n, REPO_NAME )
#define checktree_at(L, n) \
      (git_tree**) luaL_checkudata( L, n, LUAGI_TREE_FUNCS )
#define checktree(L) \
        checktree_at( L, 1 )
#define checkremote(L) \
      (git_remote**) luaL_checkudata( L, 1, LUAGI_REMOTE_FUNCS )

int signature_to_table( lua_State *L, const git_signature* sig);
int table_to_signature( lua_State *L, git_signature* sig, int position );
const char* luagi_otype_to_string( git_otype type );

int luagi_oid_fromstr( git_oid *oid, const char *ref );
void luagi_lua_list_from_string( lua_State *L, git_strarray *array );
git_strarray luagi_strings_from_lua_list( lua_State *L, int table_idx );

typedef struct luagi_foreach_payload
{
   lua_State *L;
   int callback_pos;
} luagi_foreach_t;

void dumpStack( lua_State* L );
#ifdef STACK_DEBUG
#define debugStack( L ); dumpStack( L );
#else
#define debugStack( L );
#endif
#endif
