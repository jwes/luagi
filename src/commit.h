#ifndef IS_LUAGI_COMMIT
#define IS_LUAGI_COMMIT
#include <lua.h>

int luagi_commit_lookup( lua_State *L );
int luagi_commit_gc( lua_State *L );
int luagi_commit_id( lua_State *L );
int luagi_commit_encoding( lua_State *L );
int luagi_commit_message( lua_State *L );
int luagi_commit_summary( lua_State *L );
int luagi_commit_author( lua_State *L );
int luagi_commit_committer( lua_State *L );
int luagi_commit_tree( lua_State *L );

int luagi_commit_parentcount( lua_State *L );
int luagi_commit_parent( lua_State *L );
int luagi_commit_parent_id( lua_State *L );
int luagi_commit_nth_gen_ancestor( lua_State *L );
int luagi_commit_create( lua_State *L );
int luagi_commit_amend( lua_State *L );

#define LUAGI_COMMIT_FUNCS "is.westh.luagi.commit"

#define checkcommit_at( L, N ) \
   (git_commit**) luaL_checkudata( L, N, LUAGI_COMMIT_FUNCS )
#define checkcommit( L ) \
   checkcommit_at( L, 1 )
static const struct luaL_Reg luagi_commit_funcs [] = {
   { "id", luagi_commit_id },
   { "summary", luagi_commit_summary },
   { "message", luagi_commit_message },
   { "encoding", luagi_commit_encoding },
   { "committer", luagi_commit_committer },
   { "author", luagi_commit_author },
   { "tree", luagi_commit_tree },
   { "parent_count", luagi_commit_parentcount },
   { "parent", luagi_commit_parent },
   { "parent_id", luagi_commit_parent_id },
   { "ancestor", luagi_commit_nth_gen_ancestor },
   { "amend", luagi_commit_amend },
   { "__gc", luagi_commit_gc },
   { NULL, NULL }
};
#endif
