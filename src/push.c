#include <git2/buffer.h>
#include <git2/push.h>
#include "push.h"
#include "luagi.h"
#include "remote.h"

int luagi_push_new( lua_State *L )
{
   git_remote **remote = checkremote( L );
   git_push **out = lua_newuserdata( L, sizeof( git_push *) );

   if( git_push_new( out, *remote ) )
   {
      ERROR_PUSH( L )
   } 
   luaL_getmetatable( L, LUAGI_PUSH_FUNCS );
   lua_setmetatable( L, -2 );
   return 1;
}
static int luagi_push_init_options( lua_State *L __attribute__(( unused )), int index __attribute__(( unused )), git_push_options *opts )
{
   int ret = git_push_init_options( opts, GIT_PUSH_OPTIONS_VERSION );
   //TODO push options
   return ret;
}

int luagi_push_set_options( lua_State *L )
{
   git_push **push = checkpush_at( L, 1 );
   git_push_options opts;
   luagi_push_init_options( L, 2, &opts );

   if( git_push_set_options( *push, &opts ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_push_set_callbacks( lua_State *L );

int luagi_push_add_refspec( lua_State *L )
{
   git_push **push = checkpush_at( L, 1 );
   const char *refspec = luaL_checkstring( L, 2 );

   if( git_push_add_refspec( *push, refspec ) )
   {
      ERROR_ABORT( L );
   }
   return 0;
}

int luagi_push_update_tips( lua_State *L )
{
   git_push **push = checkpush_at( L, 1 );
   git_signature signature;
   table_to_signature( L, &signature,  2 );
   const char *message = luaL_checkstring( L, 3 );

   if( git_push_update_tips( *push, &signature, message ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_push_finish( lua_State *L )
{
   git_push **push = checkpush_at( L, 1 );
   if( git_push_finish( *push ) )
   {
      ERROR_ABORT( L )
   }
   return 0;
}

int luagi_push_unpack_ok( lua_State *L )
{
   git_push **push = checkpush_at( L, 1 );
   lua_pushboolean( L, git_push_unpack_ok( *push ) );
   return 1;
}

int luagi_push_status_foreach( lua_State *L );

int luagi_push_free( lua_State *L )
{
   git_push **push = checkpush_at( L, 1 );
   git_push_free( *push );
   return 0;
}


