#include <git2/buffer.h>
#include <git2/pack.h>
#include "pack.h"
#include "oid.h"
#include "luagi.h"

int luagi_packbuilder_new( lua_State *L );

static int generic_insert( lua_State *L, int (*func)( git_packbuilder *pb, const git_oid *oid ) )
{
   git_packbuilder **pack = checkpack( L );
   git_oid oid;
   luagi_check_oid( &oid,  L, 2 );

   if( func( *pack, &oid ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_packbuilder_set_threads( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   unsigned int num = luaL_checkinteger( L, 2 );

   unsigned int used = git_packbuilder_set_threads( *pack, num );

   lua_pushinteger( L, used );
   return 1;
}

int luagi_packbuilder_insert( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   git_oid oid;
   luagi_check_oid( &oid, L, 2 );
   const char *name = luaL_checkstring( L, 3 );

   if( git_packbuilder_insert( *pack, &oid, name ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_packbuilder_insert_tree( lua_State *L )
{
   return generic_insert( L, git_packbuilder_insert_tree );
}
int luagi_packbuilder_insert_commit( lua_State *L )
{
   return generic_insert( L, git_packbuilder_insert_commit );
}

int luagi_packbuilder_write_buf( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   git_buf buf;
   if( git_packbuilder_write_buf( &buf, *pack ) )
   {
      ERROR_PUSH( L )
   }
   lua_pushlstring( L, buf.ptr, buf.size );
   return 1;
}

int luagi_packbuilder_write( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   const char *path = luaL_checkstring( L, 2 );
   unsigned int mode = 0; //TODO mode
   //TODO progress_cb
   
   if( git_packbuilder_write( *pack, path, mode, NULL, NULL ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_packbuilder_hash( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   const git_oid *oid = git_packbuilder_hash( *pack );

   return luagi_push_oid( L, oid );
}

int luagi_packbuilder_foreach( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
   //TODO callbacks 
   return 0;
}

int luagi_packbuilder_object_count( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   lua_pushinteger( L, git_packbuilder_object_count( *pack ) );
   return 1;
}

int luagi_packbuilder_written( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   lua_pushinteger( L, git_packbuilder_written( *pack ) );
   return 1;
}

int luagi_packbuilder_set_callbacks( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
   //TODO callbacks 
   return 0;
}

int luagi_packbuilder_free( lua_State *L )
{
   git_packbuilder **pack = checkpack( L );
   git_packbuilder_free( *pack );
   return 0;
}

