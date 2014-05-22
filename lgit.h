#ifndef IS_LGIT_H
#define IS_LGIT_H
#include <git2/types.h>
#include <git2/oid.h>

#define REPO_NAME "is.westhu.lgit.repo"
#define LGIT_BRANCH_STATICS "is.westhu.lgit.branch.iter"
#define LGIT_BRANCH_FUNCS "is.westh.lgit.branch"
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
#define checktree(L) \
		(git_tree**) luaL_checkudata( L, 1, LGIT_TREE_FUNCS )
#define checkremote(L) \
		(git_remote**) luaL_checkudata( L, 1, LGIT_REMOTE_FUNCS )

int signature_to_table( lua_State *L, const git_signature* sig);
int table_to_signature( lua_State *L, git_signature* sig, int position );
const char* lgit_otype_to_string( git_otype type );

int lgit_oid_fromstr( git_oid *oid, const char *ref );

#endif
