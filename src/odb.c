#include <git2/odb.h>

#include "odb.h"
#include "luagi.h"
#include "oid.h"

//general
int luagi_odb_new( lua_State *L )
{
   git_odb **odb = lua_newuserdata( L, sizeof( git_odb *) );

   if( git_odb_new( odb ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_ODB_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_odb_open( lua_State *L )
{
   const char *objects_dir = luaL_checkstring( L, 1 );
   
   git_odb **odb = lua_newuserdata( L, sizeof( git_odb *) );

   if( git_odb_open( odb, objects_dir ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_ODB_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}


//odb functions
int luagi_odb_add_disk_alternate( lua_State *L )
{
   git_odb **odb = checkodb_at( L, 1 );
   const char *path = luaL_checkstring( L, 2 );

   if( git_odb_add_disk_alternate( *odb, path ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_odb_free( lua_State *L )
{
   git_odb **odb = checkodb_at( L, 1 );
   git_odb_free( *odb );
   return 0;
}

int luagi_odb_read( lua_State *L )
{
   git_odb **odb = checkodb_at( L, 1 );
   git_oid oid;
   luagi_check_oid( &oid, L, 2 );

   git_odb_object **obj = lua_newuserdata( L, sizeof( git_odb_object *) );

   if( git_odb_read( obj, *odb, &oid ) )
   {
      ERROR_PUSH( L )
   }

   luaL_getmetatable( L, LUAGI_ODB_OBJECT_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_odb_read_prefix( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_read_header( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_exists( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_exists_prefix( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_refresh( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_foreach( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_write( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_odb_open_wstream( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_open_rstream( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_odb_write_pack( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_hash( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_hashfile( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_odb_add_backend( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_add_alternate( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_num_backends( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_get_backend( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }


int luagi_odb_stream_write( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_stream_finalize_write( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_stream_read( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_stream_free( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_odb_object_free( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_object_id( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_object_data( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_object_size( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_object_type( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
