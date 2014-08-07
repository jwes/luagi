#ifndef IS_LGIT_H
#define IS_LGIT_H
#include <git2/types.h>
#include <git2/oid.h>
#include <git2/strarray.h>

#define REPO_NAME "is.westhu.lgit.repo"
#define LGIT_TREE_FUNCS "is.westh.lgit.tree"
#define LGIT_TREE_ENTRY_FUNCS "is.westh.lgit.tree.entry"
#define LGIT_TREE_BUILDER_FUNCS "is.westh.lgit.tree.builder"
#define LGIT_TREE_WALK_STATICS "is.westhu.lgit.tree.walk"
#define LGIT_COMMIT_FUNCS "is.westh.lgit.commit"
#define LGIT_REMOTE_FUNCS "is.westh.lgit.remote"

#define SIG_NAME "name"
#define SIG_EMAIL "email"
#define SIG_TIME "time"
#define SIG_TIME_OFF "time_offset"

#define checkrepo(L, n) \
      (git_repository**) luaL_checkudata( L, n, REPO_NAME )
#define checktree_at(L, n) \
      (git_tree**) luaL_checkudata( L, n, LGIT_TREE_FUNCS )
#define checktree(L) \
        checktree_at( L, 1 )
#define checkremote(L) \
      (git_remote**) luaL_checkudata( L, 1, LGIT_REMOTE_FUNCS )

int signature_to_table( lua_State *L, const git_signature* sig);
int table_to_signature( lua_State *L, git_signature* sig, int position );
const char* lgit_otype_to_string( git_otype type );

int lgit_oid_fromstr( git_oid *oid, const char *ref );
git_strarray lgit_strings_from_lua_list( lua_State *L, int table_idx );

void dumpStack( lua_State* L );
#define STACK_DEBUG
#ifdef STACK_DEBUG
#define debugStack( L ); dumpStack( L );
#else
#define debugStack( L );
#endif
#endif
