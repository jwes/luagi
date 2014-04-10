#ifndef IS_LGIT_H
#define IS_LGIT_H

#define REPO_NAME "is.westhu.lgit.repo"
#define LGIT_BRANCH_STATICS "is.westhu.lgit.branch.iter"
#define LGIT_BRANCH_FUNCS "is.westh.lgit.branch"

#define checkrepo(L, n) \
		(git_repository**) luaL_checkudata( L, n, REPO_NAME )

#endif
