#include "index.h"
#include <git2/errors.h>

int lgit_index_new( lua_State *L )
{
   const char *path = luaL_optstring( L, 1, NULL );
   git_index **index = lua_newuserdata( L, sizeof( git_index *) ); 
   if( path == NULL )
   {
      if( git_index_new( index ) )
      {
         const git_error *err = giterr_last();
         lua_pushnil( L );
         lua_pushstring( L, err->message );
         return 2;
      }
   }
   else 
   {
      if( git_index_open( index, path ) )
      {
         const git_error *err = giterr_last();
         lua_pushnil( L );
         lua_pushstring( L, err->message );
         return 2;
      }
   }

   luaL_getmetatable(L, LGIT_INDEX_FUNCS);
   lua_setmetatable(L, -2);
   return 1;
}

int lgit_index_free( lua_State *L )
{ 
   git_index **index = checkindex_at( L, 1 );
   git_index_free( *index );
   return 0;
}

int lgit_index_owner( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_caps( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_set_caps( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_read( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_write( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_path( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_read_tree( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_write_tree( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_write_tree_to( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_entrycount( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_clear( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_get_byindex( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_get_bypath( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_remove( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_remove_directory( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_add( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_entry_stage( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_add_bypath( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_remove_bypath( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_add_all( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_remove_all( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_update_all( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_find( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_conflict_add( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_conflict_get( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_conflict_remove( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_conflict_cleanup( lua_State *L ){ lua_pushnil( L ); return 1; }
int lgit_index_has_conflicts( lua_State *L ){ lua_pushnil( L ); return 1; }
