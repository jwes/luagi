#include <git2/config.h>
#include "config.h"
#include "luagi.h"

//general
static int luagi_find_config( lua_State *L, int (*func)( git_buf *out ) )
{
   git_buf buf;
   if( func( &buf ) )
   {
      ERROR_PUSH( L )
   }
   lua_pushlstring( L, buf.ptr, buf.size );
   return 1;
}
   
int luagi_config_find_global( lua_State *L )
{
   return luagi_find_config( L, git_config_find_global );
}

int luagi_config_find_xdg( lua_State *L )
{
   return luagi_find_config( L, git_config_find_xdg );
}

int luagi_config_find_system( lua_State *L )
{
   return luagi_find_config( L, git_config_find_system );
}

int luagi_config_open_default( lua_State *L )
{
   git_config **cfg = lua_newuserdata( L, sizeof( git_config *) );

   if( git_config_open_default( cfg ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_CONFIG_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_config_new( lua_State *L )
{
   git_config **cfg = lua_newuserdata( L, sizeof( git_config *) );

   if( git_config_new( cfg ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_CONFIG_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_config_open_ondisk( lua_State *L )
{
   const char *path = luaL_checkstring( L, 1 );
   git_config **cfg = lua_newuserdata( L, sizeof( git_config *) );

   if( git_config_open_ondisk( cfg, path ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_CONFIG_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

static git_config_level_t luagi_check_level( lua_State *L, int index )
{
   const char *level = luaL_checkstring( L, index );

   switch ( *level )
   {
      case 's':
         return GIT_CONFIG_LEVEL_SYSTEM;
      case 'x':
         return GIT_CONFIG_LEVEL_XDG;
      case 'g':
         return GIT_CONFIG_LEVEL_GLOBAL;
      case 'l':
         return GIT_CONFIG_LEVEL_LOCAL;
      case 'a':
         return GIT_CONFIG_LEVEL_APP;
      default:
      case 'h':
         return GIT_CONFIG_HIGHEST_LEVEL;

   }
}
static int luagi_push_level( lua_State *L, git_config_level_t level )
{
   char *levelstr;
   switch( level )
   {
      case GIT_CONFIG_LEVEL_SYSTEM:
         levelstr = SYSTEM;
      case GIT_CONFIG_LEVEL_XDG:
         levelstr = XDG;
      case GIT_CONFIG_LEVEL_GLOBAL:
         levelstr = GLOBAL;
      case GIT_CONFIG_LEVEL_LOCAL:
         levelstr = LOCAL;
      case GIT_CONFIG_LEVEL_APP:
         levelstr = APP;
      case GIT_CONFIG_HIGHEST_LEVEL:
         levelstr = HIGHEST;
   }
      
   lua_pushstring( L, levelstr );
   return 1;
}
   
// config
int luagi_config_add_file_ondisk( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *path = luaL_checkstring( L, 2 );
   git_config_level_t level = luagi_check_level( L, 3 );
   int force = lua_toboolean( L, 4 );

   if( git_config_add_file_ondisk( *cfg, path, level, force ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_config_open_level( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   git_config_level_t level = luagi_check_level( L, 2 );

   git_config **out = lua_newuserdata( L, sizeof( git_config * ) );

   if( git_config_open_level( out, *cfg, level ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_CONFIG_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_config_open_global( lua_State *L )
{
   git_config **cfg = checkconfig( L );

   git_config **out = lua_newuserdata( L, sizeof( git_config * ) );

   if( git_config_open_global( out, *cfg ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_CONFIG_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_config_snapshot( lua_State *L )
{
   git_config **cfg = checkconfig( L );

   git_config **out = lua_newuserdata( L, sizeof( git_config * ) );

   if( git_config_snapshot( out, *cfg ) )
   {
      ERROR_PUSH( L )
   }
   luaL_getmetatable( L, LUAGI_CONFIG_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}

int luagi_config_refresh( lua_State *L )
{
   git_config **cfg = checkconfig( L );

   if( git_config_refresh( *cfg ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}
static int luagi_push_config_entry( lua_State *L, const git_config_entry *entry )
{
   lua_pushstring( L, entry->value );
   lua_pushstring( L, entry->name );
   luagi_push_level( L, entry->level );
   return 3;
}

int luagi_config_get_entry( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );

   const git_config_entry *entry;
   if( git_config_get_entry( &entry, *cfg, name ) )
   {
      ERROR_PUSH( L )
   }

   return luagi_push_config_entry( L, entry );
}
     
int luagi_config_get_int32( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );
   int32_t out;

   if( git_config_get_int32( &out, *cfg, name ) )
   {
      ERROR_PUSH( L )
   }

   lua_pushinteger( L, out );
   return 1;
}

int luagi_config_get_int64( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );
   int64_t out;

   if( git_config_get_int64( &out, *cfg, name ) )
   {
      ERROR_PUSH( L )
   }

   lua_pushinteger( L, out );
   return 1;
}

int luagi_config_get_bool( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );

   int out;
   if( git_config_get_bool( &out, *cfg, name ) )
   {
      ERROR_PUSH( L )
   }

   lua_pushboolean( L, out );
   return 1;
}

int luagi_config_get_string( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );

   const char *out;

   if( git_config_get_string( &out, *cfg, name ) )
   {
      ERROR_PUSH( L )
   }

   lua_pushstring( L, out );
   return 1;
}

static int config_foreach_cb( const git_config_entry *entry, void *payload )
{
   luagi_foreach_t *p = payload;

   lua_pushvalue( p->L, p->callback_pos );
   int elements = luagi_push_config_entry( p->L, entry );

   if( lua_pcall( p->L, elements, 1, 0 ) )
   {
      luaL_error( p->L, "error calling config callback" );
      return 1;
   }
   int ret = luaL_checkinteger( p->L, -1 );
   lua_pop( p->L, 1 );
   return ret;
}

int luagi_config_get_mulitvar_foreach( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );
   const char *regexp = luaL_checkstring( L, 3 );

   luaL_checktype( L, 4, LUA_TFUNCTION );

   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );

   if( git_config_get_multivar_foreach( *cfg, name, regexp, config_foreach_cb, p ) )
   {
      ERROR_ABORT( L )
   }
   free( p );
   return 0;
}

static int config_iter( lua_State *L )
{
   git_config_iterator **iter = lua_touserdata( L, lua_upvalueindex( 1 ) );
   git_config_entry *entry;
   int ret = git_config_next( &entry, *iter );
   if( ret == GIT_ITEROVER )
   {
      return 0;
   }
   else if( ret < 0 )
   {
      ERROR_PUSH( L )
   }

   return luagi_push_config_entry( L, entry );
}

int luagi_config_iterator_new( lua_State *L )
{
   git_config **cfg = checkconfig( L );

   git_config_iterator **out = lua_newuserdata( L, sizeof( git_config_iterator *) );

   if( git_config_iterator_new( out, *cfg ) )
   {
      ERROR_PUSH( L )
   }
   
   luaL_getmetatable( L, LUAGI_CONFIG_ITERATOR_FUNCS );
   lua_setmetatable( L, -2 );

   lua_pushcclosure( L, config_iter, 1 );
   return 1;
}

int luagi_config_multivar_iterator( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );
   const char *regexp = luaL_checkstring( L, 3 );

   git_config_iterator **out = lua_newuserdata( L, sizeof( git_config_iterator *) );

   if( git_config_multivar_iterator_new( out, *cfg, name, regexp ) )
   {
      ERROR_PUSH( L )
   }
   
   luaL_getmetatable( L, LUAGI_CONFIG_ITERATOR_FUNCS );
   lua_setmetatable( L, -2 );

   lua_pushcclosure( L, config_iter, 1 );
   return 1;
}
int luagi_config_iterator_glob_new( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *regexp = luaL_checkstring( L, 2 );

   git_config_iterator **out = lua_newuserdata( L, sizeof( git_config_iterator *) );

   if( git_config_iterator_glob_new( out, *cfg, regexp ) )
   {
      ERROR_PUSH( L )
   }
   
   luaL_getmetatable( L, LUAGI_CONFIG_ITERATOR_FUNCS );
   lua_setmetatable( L, -2 );

   lua_pushcclosure( L, config_iter, 1 );
   return 1;
}

int luagi_config_iterator_free( lua_State *L )
{
   git_config_iterator **iter = luaL_checkudata( L, 1, LUAGI_CONFIG_ITERATOR_FUNCS );
   git_config_iterator_free( *iter );
   return 0;
}

int luagi_config_set_int32( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );
   int32_t value = luaL_checkinteger( L, 3 );

   if( git_config_set_int32( *cfg, name, value ) )
   {
      ERROR_ABORT( L )
   }

   return 0;
}

int luagi_config_set_int64( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );
   int64_t value = luaL_checkinteger( L, 3 );

   if( git_config_set_int32( *cfg, name, value ) )
   {
      ERROR_ABORT( L )
   }

   return 0;
}

int luagi_config_set_bool( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );
   int value = lua_toboolean( L, 3 );

   if( git_config_set_bool( *cfg, name, value ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_config_set_string( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );
   const char *value = luaL_checkstring( L, 3 );

   if( git_config_set_string( *cfg, name, value ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_config_set_multivar( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );
   const char *regexp = luaL_checkstring( L, 3 );
   const char *value = luaL_checkstring( L, 4 );

   if( git_config_set_multivar( *cfg, name, regexp, value ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_config_delete_entry( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );

   if( git_config_delete_entry( *cfg, name ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_config_delete_mulitvar( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );
   const char *regexp = luaL_checkstring( L, 3 );

   if( git_config_delete_multivar( *cfg, name, regexp ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_config_foreach( lua_State *L )
{
   git_config **cfg = checkconfig( L );

   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );

   if( git_config_foreach( *cfg, config_foreach_cb, p ) )
   {
      ERROR_ABORT( L )
   }
   free( p );
   return 0;
}

int luagi_config_foreach_match( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *regexp = luaL_checkstring( L, 2 );

   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );

   if( git_config_foreach_match( *cfg, regexp, config_foreach_cb, p ) )
   {
      ERROR_ABORT( L )
   }
   free( p );
   return 0;
}

//TODO mapping
int luagi_config_get_mapped( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }
int luagi_config_lookup_map_value( lua_State *L ){ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_config_parse_bool( lua_State *L )
{
   const char *value = luaL_checkstring( L, 1 );
   int out;
   if( git_config_parse_bool( &out, value ))
   {
      ERROR_ABORT( L )
      return 0;
   }
   lua_pushboolean( L, out );
   return 1;
}
                   
int luagi_config_parse_int32( lua_State *L )
{
   const char *value = luaL_checkstring( L, 1 );
   int32_t out;
   if( git_config_parse_int32( &out, value ))
   {
      ERROR_ABORT( L )
      return 0;
   }
   lua_pushinteger( L, out );
   return 1;
}

int luagi_config_parse_int64( lua_State *L )
{
   const char *value = luaL_checkstring( L, 1 );
   int64_t out;
   if( git_config_parse_int64( &out, value ))
   {
      ERROR_ABORT( L )
      return 0;
   }
   lua_pushinteger( L, out );
   return 1;
}

int luagi_config_backend_foreach_match( lua_State *L )
{ luaL_error( L, "not yet implemented" ); return 0; }

int luagi_config_free( lua_State *L )
{
   git_config **cfg = checkconfig_at( L, 1 );
   git_config_free( *cfg );
   return 0;
}

