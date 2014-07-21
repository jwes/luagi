#ifndef IS_LGIT_COMMIT
#define IS_LGIT_COMMIT
#include <lua.h>

int lgit_commit_lookup( lua_State *L );
int lgit_commit_gc( lua_State *L );
int lgit_commit_id( lua_State *L );
int lgit_commit_encoding( lua_State *L );
int lgit_commit_message( lua_State *L );
int lgit_commit_summary( lua_State *L );
int lgit_commit_author( lua_State *L );
int lgit_commit_committer( lua_State *L );
int lgit_commit_tree( lua_State *L );

int lgit_commit_parentcount( lua_State *L );
int lgit_commit_parent( lua_State *L );
int lgit_commit_parent_id( lua_State *L );
int lgit_commit_nth_gen_ancestor( lua_State *L );
int lgit_commit_create( lua_State *L );
int lgit_commit_amend( lua_State *L );

static const struct luaL_Reg lgit_commit_funcs [] = {
   { "id", lgit_commit_id },
   { "summary", lgit_commit_summary },
   { "message", lgit_commit_message },
   { "encoding", lgit_commit_encoding },
   { "committer", lgit_commit_committer },
   { "author", lgit_commit_author },
   { "tree", lgit_commit_tree },
   { "parent_count", lgit_commit_parentcount },
   { "parent", lgit_commit_parent },
   { "parent_id", lgit_commit_parent_id },
   { "ancestor", lgit_commit_nth_gen_ancestor },
   { "amend", lgit_commit_amend },
   { "__gc", lgit_commit_gc },
   { NULL, NULL }
};
#endif
