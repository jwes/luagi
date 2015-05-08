#include "push.h"

#include <git2/buffer.h>
#include <git2/push.h>
#include <git2/signature.h>

#include "ltk.h"
#include "luagi.h"
#include "remote.h"

int luagi_push_new( lua_State *L )
{
   git_remote **remote = checkremote( L );
   git_push **out = lua_newuserdata( L, sizeof( git_push *) );

   if( git_push_new( out, *remote ) )
   {
      return ltk_push_git_error( L );
   } 
   ltk_setmetatable( L, LUAGI_PUSH_FUNCS );
   return 1;
}
static int luagi_push_init_options( lua_State *L __attribute__(( unused )), int index __attribute__(( unused )), git_push_options *opts )
{
   int ret = git_push_init_options( opts, GIT_PUSH_OPTIONS_VERSION );

   lua_getfield( L, index, THREADS );
   if( lua_type( L, -1 ) == LUA_TNUMBER )
   {
      opts->pb_parallelism = luaL_checkinteger( L, -1 );
   }
   return ret;
}

int luagi_push_set_options( lua_State *L )
{
   git_push **push = checkpush_at( L, 1 );
   git_push_options opts;
   luagi_push_init_options( L, 2, &opts );

   if( git_push_set_options( *push, &opts ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_push_set_callbacks( lua_State *L )
{
   luaL_error( L, "not yet implemented" );
   return 0;
}

int luagi_push_add_refspec( lua_State *L )
{
   git_push **push = checkpush_at( L, 1 );
   const char *refspec = luaL_checkstring( L, 2 );

   if( git_push_add_refspec( *push, refspec ) )
   {
      ltk_error_abort( L );;
   }
   return 0;
}

int luagi_push_update_tips( lua_State *L )
{
   git_push **push = checkpush_at( L, 1 );
   git_signature *signature;
   table_to_signature( L, &signature,  2 );
   const char *message = luaL_checkstring( L, 3 );

   int ret = git_push_update_tips( *push, signature, message );
   git_signature_free( signature );
   if( ret )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_push_finish( lua_State *L )
{
   git_push **push = checkpush_at( L, 1 );
   if( git_push_finish( *push ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_push_unpack_ok( lua_State *L )
{
   git_push **push = checkpush_at( L, 1 );
   lua_pushboolean( L, git_push_unpack_ok( *push ) );
   return 1;
}

static int foreach_callback( const char *ref, const char *msg, void *data )
{
   luagi_foreach_t *p = data;

   lua_pushvalue( p->L, p->callback_pos );
   lua_pushstring( p->L, ref );
   lua_pushstring( p->L, msg );

   if( lua_pcall( p->L, 2, 1, 0 ) )
   {
      ltk_error_abort( p->L );
      return -1;
   }
   int ret = luaL_checkinteger( p->L, -1 );
   lua_pop( p->L, 1 );
   return ret;
}

int luagi_push_status_foreach( lua_State *L )
{
   git_push **push = checkpush_at( L, 1 );
   luaL_checktype( L, 2, LUA_TFUNCTION );

   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );
   p->L = L;
   p->callback_pos = 2;

   if( git_push_status_foreach( *push, foreach_callback, p ) )
   {
      ltk_error_abort( L );
   }

   free( p );
   return 0;
}

int luagi_push_free( lua_State *L )
{
   git_push **push = checkpush_at( L, 1 );
   git_push_free( *push );
   return 0;
}


