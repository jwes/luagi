#include "odb.h"

#include <git2/odb.h>

#include "ltk.h"
#include "luagi.h"
#include "oid.h"
#include "types.h"

//general
int luagi_odb_new( lua_State *L )
{
   git_odb **odb = lua_newuserdata( L, sizeof( git_odb *) );

   if( git_odb_new( odb ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_ODB_FUNCS );
   return 1;
}

int luagi_odb_open( lua_State *L )
{
   const char *objects_dir = luaL_checkstring( L, 1 );
   
   git_odb **odb = lua_newuserdata( L, sizeof( git_odb *) );

   if( git_odb_open( odb, objects_dir ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_ODB_FUNCS );
   return 1;
}


//odb functions
int luagi_odb_add_disk_alternate( lua_State *L )
{
   git_odb **odb = checkodb_at( L, 1 );
   const char *path = luaL_checkstring( L, 2 );

   if( git_odb_add_disk_alternate( *odb, path ) )
   {
      ltk_error_abort( L );
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
   int len, ret = 0;
   luagi_check_oid_prefix( &oid, &len, L, 2 );

   git_odb_object **obj = lua_newuserdata( L, sizeof( git_odb_object *) );
   if( len == GIT_OID_HEXSZ )
   {
      ret = git_odb_read( obj, *odb, &oid ); 
   }
   else
   {
      ret = git_odb_read_prefix( obj, *odb, &oid, len ); 
   }
   if( ret )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_ODB_OBJECT_FUNCS );
   return 1;
}

int luagi_odb_read_header( lua_State *L )
{
   git_odb **odb = checkodb_at( L, 1 );
   git_oid oid;
   luagi_check_oid( &oid, L, 2 );

   size_t len_out;
   git_otype type;

   if( git_odb_read_header( &len_out, &type, *odb, &oid ) )
   {
      return ltk_push_git_error( L );
   }

   lua_pushinteger( L, len_out );
   lua_pushstring( L, luagi_string_from_otype( type ) );
   return 2;
}

int luagi_odb_exists( lua_State *L )
{
   git_odb **odb = checkodb_at( L, 1 );
   git_oid oid, out;
   int len, ret = 0;
   int ambiguous = 0, num = 0;
   luagi_check_oid_prefix( &oid, &len,  L, 2 );
   if( len == GIT_OID_HEXSZ )
   {
      ret = git_odb_exists( *odb, &oid );
      out = oid;
   }
   else
   {
      ret = git_odb_exists_prefix( &out, *odb, &oid, len );
      switch( ret )
      {
         case GIT_EAMBIGUOUS:
            ambiguous = 1;
            /* no break */
         case 0:
            ret = 1;
            break;
         case GIT_ENOTFOUND:
            ret = 0;
            break;
      }

   }
   if( ret < 0 )
   {
      return ltk_push_git_error( L );
   }

   lua_pushboolean( L, ret );
   if( ! ambiguous )
   {
        num = luagi_push_oid( L, &out );
   }
   return num + 1;
}

int luagi_odb_refresh( lua_State *L )
{
   git_odb **odb = checkodb_at( L, 1 );

   if( git_odb_refresh( *odb ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int odb_cb( const git_oid *id, void *payload )
{
   luagi_foreach_t *p = payload;

   lua_pushvalue( p->L, p->callback_pos );
   luagi_push_oid( p->L, id );

   if( lua_pcall( p->L, 1, 1, 0) )
   {
      luaL_error( p->L, "error calling odb foreach callback" );
      return 1;
   }
   int ret = luaL_checkinteger( p->L, -1 );
   lua_pop( p->L, 1 );
   return ret;
}

int luagi_odb_foreach( lua_State *L )
{
   git_odb **odb = checkodb_at( L, 1 );

   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );

   luaL_checktype( L, 2, LUA_TFUNCTION );
   p->L = L;
   p->callback_pos = 2;

   if( git_odb_foreach( *odb, odb_cb, p ) )
   {
      ltk_error_abort( L );
   }
   free( p );

   return 0;
}

int luagi_odb_write( lua_State *L )
{
   git_odb **odb = checkodb_at( L, 1 );

   int len = luaL_len( L, 2 );
   const char *data = luaL_checkstring( L, 2 );

   git_otype type = luagi_otype_from_string( luaL_checkstring( L, 3 ) );
   git_oid oid;
   if( git_odb_write( &oid, *odb, data, len, type ) )
   {
      return ltk_push_git_error( L );
   }

   return luagi_push_oid( L, &oid );
}

int luagi_odb_open_wstream( lua_State *L )
{
   git_odb **odb = checkodb_at( L, 1 );
   int size = luaL_checkinteger( L, 2 );
   git_otype type = luagi_otype_from_string( luaL_checkstring( L, 3 ) );

   git_odb_stream **out = lua_newuserdata( L, sizeof( git_odb_stream *) );

   if( git_odb_open_wstream( out, *odb, size, type ) )
   {
      return ltk_push_git_error( L );
   }

   ltk_setmetatable( L, LUAGI_ODB_STREAM_FUNCS );
   return 1;
}

int luagi_odb_open_rstream( lua_State *L )
{
   git_odb **odb = checkodb_at( L, 1 );

   git_oid oid;
   luagi_check_oid( &oid, L, 2 );

   git_odb_stream **out = lua_newuserdata( L, sizeof( git_odb_stream *) );

   if( git_odb_open_rstream( out, *odb, &oid ) )
   {
      return ltk_push_git_error( L );
   }
   
   ltk_setmetatable( L, LUAGI_ODB_STREAM_FUNCS );
   return 1;
} 

//TODO write pack
int luagi_odb_write_pack( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_hash( lua_State *L )
{
   int len = luaL_len( L, 1 );
   const char *data = luaL_checkstring( L, 1 );
   git_otype type = luagi_otype_from_string( luaL_checkstring( L, 2 ) );

   git_oid oid;
   if( git_odb_hash( &oid, data, len, type ) )
   {
      return ltk_push_git_error( L );
   }

   return luagi_push_oid( L, &oid );
}

int luagi_odb_hashfile( lua_State *L )
{
   const char *path = luaL_checkstring( L, 1 );

   git_otype type = luagi_otype_from_string( luaL_checkstring( L, 2 ) );

   git_oid oid;
   if( git_odb_hashfile( &oid, path, type ) )
   {
      return ltk_push_git_error( L );
   }

   return luagi_push_oid( L, &oid );
}

//TODO backend
int luagi_odb_add_backend( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_odb_add_alternate( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_odb_num_backends( lua_State *L )
{
   git_odb **odb = checkodb_at( L, 1 );
   lua_pushinteger( L, git_odb_num_backends( *odb ) );
   return 1;
}

int luagi_odb_get_backend( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }


int luagi_odb_stream_write( lua_State *L )
{
   git_odb_stream **stream = checkodbstream_at( L, 1 );
   int len = luaL_len( L, 2 );
   const char *buffer = luaL_checkstring( L, 2 );

   if( git_odb_stream_write( *stream, buffer, len ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_odb_stream_finalize_write( lua_State *L )
{
   git_odb_stream **stream = checkodbstream_at( L, 1 );
   git_oid oid;

   if( git_odb_stream_finalize_write( &oid, *stream ) )
   {
      return ltk_push_git_error( L );
   }
   return luagi_push_oid( L, &oid );
}

#define bufsize 2048
int luagi_odb_stream_read( lua_State *L )
{
   git_odb_stream **stream = checkodbstream_at( L, 1 );

   char buffer [ bufsize ];
   int bytes = git_odb_stream_read( *stream, buffer, bufsize );

   if( bytes < 0 )
   {
      ltk_error_abort( L );
      return 0;
   }
   else if( bytes == 0 )
   {
      return 0;
   }

   lua_pushlstring( L, buffer, bytes );
   return 1;
}


int luagi_odb_stream_free( lua_State *L )
{
   git_odb_stream **stream = checkodbstream_at( L, 1 );
   git_odb_stream_free( *stream );
   return 0;
}

int luagi_odb_object_free( lua_State *L )
{
   git_odb_object **obj = checkodbobject_at( L, 1 );
   git_odb_object_free( *obj );
   return 0;
}

int luagi_odb_object_id( lua_State *L )
{
   git_odb_object **obj = checkodbobject_at( L, 1 );
   const git_oid *oid = git_odb_object_id( *obj );
   if( oid == NULL )
   {
      return ltk_push_git_error( L );
   }

   return luagi_push_oid( L, oid );
}
   
int luagi_odb_object_data( lua_State *L )
{
   git_odb_object **obj = checkodbobject_at( L, 1 );
   const char *data = git_odb_object_data( *obj );

   lua_pushlstring( L, data, git_odb_object_size( *obj)  );
   return 1;
}

int luagi_odb_object_size( lua_State *L )
{
   git_odb_object **obj = checkodbobject_at( L, 1 );
   lua_pushinteger( L, git_odb_object_size( *obj ) );
   return 1;
}

int luagi_odb_object_type( lua_State *L )
{
   git_odb_object **obj = checkodbobject_at( L, 1 );

   git_otype t = git_odb_object_type( *obj );
   lua_pushstring( L, luagi_string_from_otype( t ) );
   return 1;
}

