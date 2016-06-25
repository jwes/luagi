/*
 * MIT License
 * 
 * Copyright (c) 2016 Johannes Westhuis, Alexander Schulz
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "config.h"

#include <git2/config.h>

#include "ltk.h"
#include "luagi.h"

//general
static int luagi_find_config( lua_State *L, int (*func)( git_buf *out ) )
{
   git_buf buf = GIT_BUF_INIT_CONST(NULL, 0);
   if( func( &buf ) )
   {
      return ltk_push_git_error( L );
   }
   lua_pushlstring( L, buf.ptr, buf.size );
   git_buf_free( &buf );
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
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_CONFIG_FUNCS );
   return 1;
}

int luagi_config_new( lua_State *L )
{
   git_config **cfg = lua_newuserdata( L, sizeof( git_config *) );

   if( git_config_new( cfg ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_CONFIG_FUNCS );
   return 1;
}

int luagi_config_open_ondisk( lua_State *L )
{
   const char *path = luaL_checkstring( L, 1 );
   git_config **cfg = lua_newuserdata( L, sizeof( git_config *) );

   if( git_config_open_ondisk( cfg, path ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_CONFIG_FUNCS );
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
      case GIT_CONFIG_LEVEL_PROGRAMDATA:
         levelstr = PROGRAMDATA;
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
      ltk_error_abort( L );
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
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_CONFIG_FUNCS );
   return 1;
}

int luagi_config_open_global( lua_State *L )
{
   git_config **cfg = checkconfig( L );

   git_config **out = lua_newuserdata( L, sizeof( git_config * ) );

   if( git_config_open_global( out, *cfg ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_CONFIG_FUNCS );
   return 1;
}

int luagi_config_snapshot( lua_State *L )
{
   git_config **cfg = checkconfig( L );

   git_config **out = lua_newuserdata( L, sizeof( git_config * ) );

   if( git_config_snapshot( out, *cfg ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_CONFIG_FUNCS );
   return 1;
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

   git_config_entry *entry;
   if( git_config_get_entry( &entry, *cfg, name ) )
   {
      return ltk_push_git_error( L );
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
      return ltk_push_git_error( L );
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
      return ltk_push_git_error( L );
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
      return ltk_push_git_error( L );
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
      return ltk_push_git_error( L );
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
      lua_pushnil( p->L );
      lua_pushstring( p->L, "error calling config callback" );
      return 2;
   }
   int ret = luaL_optinteger( p->L, -1, 0 );
   lua_pop( p->L, 1 );
   return ret;
}
static int check_regexp( const char *regexp )
{
   if( regexp == NULL )
      return 0;

   return regexp[0] == '*'; // segmentation faults occur if the first char is a *
}

int luagi_config_get_multivar_foreach( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );
   luaL_checktype( L, 3, LUA_TFUNCTION );
   const char *regexp = luaL_optstring( L, 4, NULL );
   if( check_regexp( regexp ) )
   {
      luaL_error( L, "invalid regexp" );
      return 0;
   }
        

   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );
   p->L = L;
   p->callback_pos = 3;

   int ret = git_config_get_multivar_foreach( *cfg, name, regexp, config_foreach_cb, p ); 
   free( p );
   if( ret )
   {
      ltk_error_abort( L );
   }
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
      return ltk_push_git_error( L );
   }

   return luagi_push_config_entry( L, entry );
}

int luagi_config_iterator_new( lua_State *L )
{
   git_config **cfg = checkconfig( L );

   git_config_iterator **out = lua_newuserdata( L, sizeof( git_config_iterator *) );

   if( git_config_iterator_new( out, *cfg ) )
   {
      return ltk_push_git_error( L );
   }
   
   ltk_setmetatable( L, LUAGI_CONFIG_ITERATOR_FUNCS );

   lua_pushcclosure( L, config_iter, 1 );
   return 1;
}

int luagi_config_multivar_iterator( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );
   const char *regexp = luaL_optstring( L, 3, NULL );
   if( check_regexp( regexp ) )
   {
      luaL_error( L, "invalid regexp" );
   }

   git_config_iterator **out = lua_newuserdata( L, sizeof( git_config_iterator *) );

   if( git_config_multivar_iterator_new( out, *cfg, name, regexp ) )
   {
      return ltk_push_git_error( L );
   }
   
   ltk_setmetatable( L, LUAGI_CONFIG_ITERATOR_FUNCS );

   lua_pushcclosure( L, config_iter, 1 );
   return 1;
}
int luagi_config_iterator_glob_new( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *regexp = luaL_checkstring( L, 2 );
   if( check_regexp( regexp ) )
   {
      luaL_error( L, "invalid regexp" );
   }

   git_config_iterator **out = lua_newuserdata( L, sizeof( git_config_iterator *) );

   if( git_config_iterator_glob_new( out, *cfg, regexp ) )
   {
      return ltk_push_git_error( L );
   }
   
   ltk_setmetatable( L, LUAGI_CONFIG_ITERATOR_FUNCS );

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
      ltk_error_abort( L );
   }

   return 0;
}

int luagi_config_set_int64( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );
   int64_t value = luaL_checkinteger( L, 3 );

   if( git_config_set_int64( *cfg, name, value ) )
   {
      ltk_error_abort( L );
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
      ltk_error_abort( L );
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
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_config_set_multivar( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );
   const char *regexp = luaL_checkstring( L, 3 );
   if( check_regexp( regexp ) )
   {
      luaL_error( L, "invalid regexp" );
   }
   const char *value = luaL_checkstring( L, 4 );

   if( git_config_set_multivar( *cfg, name, regexp, value ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_config_delete_entry( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );

   if( git_config_delete_entry( *cfg, name ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_config_delete_multivar( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *name = luaL_checkstring( L, 2 );
   const char *regexp = luaL_checkstring( L, 3 );
   if( check_regexp( regexp ) )
   {
      luaL_error( L, "invalid regexp" );
   }

   if( git_config_delete_multivar( *cfg, name, regexp ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_config_foreach( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   luaL_checktype( L, 2, LUA_TFUNCTION );

   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );
   p->L = L;
   p->callback_pos = 2;
   int ret = git_config_foreach( *cfg, config_foreach_cb, p );
   free(p);
   if( ret )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_config_foreach_match( lua_State *L )
{
   git_config **cfg = checkconfig( L );
   const char *regexp = luaL_checkstring( L, 2 );
   if( check_regexp( regexp ) )
   {
      luaL_error( L, "invalid regexp" );
   }
   luaL_checktype( L, 3, LUA_TFUNCTION );

   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );
   p->L = L;
   p->callback_pos = 3;

   int ret = git_config_foreach_match( *cfg, regexp, config_foreach_cb, p ); 
   free( p );
   if( ret )
   {
      ltk_error_abort( L );
   }
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
      ltk_error_abort( L );
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
      ltk_error_abort( L );
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
      ltk_error_abort( L );
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

