#include "status.h"
#include "wien.h"
#include <git2/strarray.h>
#include <git2/diff.h>
#include <git2/status.h>
#include <git2/errors.h>
#define __USE_BSD
#include <string.h>
#undef __USE_BSD

#define INEW "index_new"
#define IMOD "index_modified"
#define IDEL "index_deleted"
#define IREN "index_renamed"
#define ITYP "index_typechange"

#define WNEW "workdir_new"
#define WMOD "workdir_modified"
#define WDEL "workdir_deleted"
#define WTYP "workdir_typechange"
#define WREN "workdir_renamed"

#define IGN "ignored"

void  lgit_status_flags_to_table( lua_State *L, const int f )
{      
   lua_newtable( L );
   lua_pushboolean( L, f & GIT_STATUS_INDEX_NEW );
   lua_setfield( L, -2, INEW );
   lua_pushboolean( L, f & GIT_STATUS_INDEX_MODIFIED );
   lua_setfield( L, -2, IMOD );
   lua_pushboolean( L, f & GIT_STATUS_INDEX_DELETED );
   lua_setfield( L, -2, IDEL );
   lua_pushboolean( L, f & GIT_STATUS_INDEX_RENAMED );
   lua_setfield( L, -2, IREN );
   lua_pushboolean( L, f & GIT_STATUS_INDEX_TYPECHANGE );
   lua_setfield( L, -2, ITYP );
   lua_pushboolean( L, f & GIT_STATUS_WT_NEW );
   lua_setfield( L, -2, WNEW );
   lua_pushboolean( L, f & GIT_STATUS_WT_MODIFIED );
   lua_setfield( L, -2, WMOD );
   lua_pushboolean( L, f & GIT_STATUS_WT_DELETED );
   lua_setfield( L, -2, WDEL );
   lua_pushboolean( L, f & GIT_STATUS_WT_TYPECHANGE );
   lua_setfield( L, -2, WTYP );
   lua_pushboolean( L, f & GIT_STATUS_WT_RENAMED );
   lua_setfield( L, -2, WREN );
   lua_pushboolean( L, f & GIT_STATUS_IGNORED );
   lua_setfield( L, -2, IGN );
}

struct status_element {
   char *path;
   unsigned int status_flags;
   struct status_element *next;
};

struct status_list {
   struct status_element *next;
   struct status_element *last;
};

static void free_elem( struct status_element *elem )
{
   free( elem->path );
   free( elem );
}

static int lgit_status_callback( const char *path, unsigned int flags, void *payload)
{
   struct status_list* info = (struct status_list*) payload;

   struct status_element *elem = (struct status_element*) malloc( sizeof( struct status_element ));
   elem->path = strdup( path );
   elem->status_flags= flags;
   elem->next = NULL;

   if( info->last == NULL )
   {
      info->next = elem;
      info->last = elem;
   }
   else
   {
      info->last->next = elem;
      info->last = elem;
   }
   return 0;
}

static int lgit_status_walker( lua_State *L );
//function on repos
int lgit_status_foreach( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   struct status_list* list = (struct status_list*) lua_newuserdata(L, sizeof(struct status_list ) );
   if( git_status_foreach( *repo, lgit_status_callback, list ) )
   {
      const git_error* err = giterr_last();
      luaL_error( L, "error iterating the files %s", err->message );
   }
   luaL_getmetatable(L, LGIT_STATUS_FOREACH);
   lua_setmetatable(L, -2);

   lua_pushcclosure( L, lgit_status_walker, 1 );
   return 1;
}

static int lgit_status_init_options( lua_State *L, int idx, git_status_options *opts )
{
   // first version with integer flags
   git_status_show_t show = luaL_optinteger( L, idx, 0 );
   unsigned int flags = luaL_optinteger( L, idx + 1, 0 );

   // TODO init options from lua_State
   int ret = git_status_init_options( opts, GIT_STATUS_OPTIONS_VERSION );
   if( ret == 0 )
   {
      opts->show = show;
      opts->flags = flags;
   }

   return ret; 
}

int lgit_status_foreach_ext( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_status_options opts;
   if( lgit_status_init_options( L, 2, &opts ) )
   {
      luaL_error( L, "could not set up options" );
   }

   struct status_list* list = (struct status_list*) lua_newuserdata(L, sizeof(struct status_list ) );
   if( git_status_foreach_ext( *repo, &opts, lgit_status_callback, list ) )
   {
      const git_error* err = giterr_last();
      luaL_error( L, "error iterating the files %s", err->message );
   }
   luaL_getmetatable(L, LGIT_STATUS_FOREACH);
   lua_setmetatable(L, -2);

   lua_pushcclosure( L, lgit_status_walker, 1 );
   return 1;
}

int lgit_status_file ( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *path = luaL_checkstring( L, 2 );

   unsigned int flags;

   if( git_status_file( &flags, *repo, path ) )
   {
      const git_error* err = giterr_last();
      luaL_error( L, "status for the fle %s failed: %s", path, err->message );
   }

   lgit_status_flags_to_table( L, flags );
   return 1;
}

int lgit_status_should_ignore( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *path = luaL_checkstring( L, 2 );
   int ignored;

   if( git_status_should_ignore( &ignored, *repo, path ) )
   {
      const git_error* err = giterr_last();
      luaL_error( L, "ignore failed %s", err->message );
   }

   lua_pushboolean( L,  ignored );
   return 1;
}

int lgit_status_list_new ( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_status_options opts;
   if( lgit_status_init_options( L, 2, &opts ) )
   {
      luaL_error( L, "init options failed" );
   }

   git_status_list **out = (git_status_list **)lua_newuserdata( L, sizeof( git_status_list *) );

   if( git_status_list_new( out, *repo, &opts ) )
   {
      const git_error *err = giterr_last();
      lua_pushnil( L );
      lua_pushfstring( L, "new failed, %s", err->message );
      return 2;
   }

   luaL_getmetatable(L, LGIT_STATUS_FUNCS);
   lua_setmetatable(L, -2);
   return 1;
}
// list operations
int lgit_status_list_entrycount( lua_State *L )
{
   git_status_list **list = checkstatuslist( L );
   lua_pushnumber( L, git_status_list_entrycount( *list ) );
   return 1;
}

int lgit_status_by_index( lua_State *L )
{
   git_status_list **list = checkstatuslist( L );
   int idx = luaL_checkinteger( L, 2 );
   // the index in lua is 1 based
   idx = idx - 1;

   const git_status_entry *entry = git_status_byindex( *list, idx );

   if( entry == NULL )
   {
      lua_pushnil( L );
      lua_pushstring( L, "index out of bounds");
      return 2;
   }

   lgit_status_flags_to_table( L, entry->status );

   //TODO git_diff_delta

   return 1;
}

int lgit_status_list_free( lua_State *L )
{
   git_status_list **list = checkstatuslist( L );
   git_status_list_free( *list ); 
   return 0;
}

static int lgit_status_walker( lua_State *L )
{
   struct status_list* info = (struct status_list*) lua_touserdata( L, lua_upvalueindex( 1 ) );
   if ( info != NULL && info->next != NULL )
   {
      struct status_element *elem = info->next;
      info->next = elem->next;
      lua_pushstring( L, elem->path );
      // prepare boolean table to get rid of the integer flags
      lgit_status_flags_to_table( L, elem->status_flags );
      free_elem( elem );
      return 2;
   }//iteration finished
   return 0;
}
   
