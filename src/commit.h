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

#endif
