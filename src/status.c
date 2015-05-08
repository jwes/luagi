#include "status.h"
#include "luagi.h"
#include "diff.h"
#include <git2/strarray.h>
#include <git2/diff.h>
#include <git2/status.h>
#include <git2/errors.h>
#define __USE_BSD
#include <string.h>
#undef __USE_BSD
#include "ltk.h"

void  luagi_status_flags_to_table( lua_State *L, const int f )
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
   lua_setfield( L, -2, IGNORED );
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

static int luagi_status_callback( const char *path, unsigned int flags, void *payload)
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

static int luagi_status_walker( lua_State *L );
//function on repos
int luagi_status_foreach( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   struct status_list* list = (struct status_list*) lua_newuserdata(L, sizeof(struct status_list ) );
   list->next = NULL;
   list->last = NULL;
   if( git_status_foreach( *repo, luagi_status_callback, list ) )
   {
      ltk_error_abort( L );
   }
   ltk_setmetatable(L, LUAGI_STATUS_FOREACH);

   lua_pushcclosure( L, luagi_status_walker, 1 );
   return 1;
}

static int flags_from_lua( lua_State *L, int idx );

static int luagi_status_init_options( lua_State *L, int idx, git_status_options *opts )
{
   if( ! lua_istable(L, idx) )
   {
      // no options given, use empty options
      return git_status_init_options( opts, GIT_STATUS_OPTIONS_VERSION );
   }
   
   // first version with integer flags
   lua_getfield( L, idx, SHOW );
   git_status_show_t show = luaL_optinteger( L, -1, 0 );

   // init options from lua_State
   int ret = git_status_init_options( opts, GIT_STATUS_OPTIONS_VERSION );
   if( ret == 0 )
   {
      opts->show = show;
      opts->flags = flags_from_lua( L, idx );
      opts->pathspec = luagi_strings_from_lua_list( L, idx );
   }

   return ret; 
}

int luagi_status_foreach_ext( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_status_options opts;
   if( luagi_status_init_options( L, 2, &opts ) )
   {
      luaL_error( L, "could not set up options" );
   }

   struct status_list* list = (struct status_list*) lua_newuserdata(L, sizeof(struct status_list ) );
   list->next = NULL;
   list->last = NULL;
   if( git_status_foreach_ext( *repo, &opts, luagi_status_callback, list ) )
   {
      ltk_error_abort( L );
   }
   ltk_setmetatable(L, LUAGI_STATUS_FOREACH);

   lua_pushcclosure( L, luagi_status_walker, 1 );
   return 1;
}

int luagi_status_file ( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *path = luaL_checkstring( L, 2 );

   unsigned int flags;

   if( git_status_file( &flags, *repo, path ) )
   {
      ltk_error_abort( L );
   }

   luagi_status_flags_to_table( L, flags );
   return 1;
}

int luagi_status_should_ignore( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *path = luaL_checkstring( L, 2 );
   int ignored;

   if( git_status_should_ignore( &ignored, *repo, path ) )
   {
      ltk_error_abort( L );
   }

   lua_pushboolean( L,  ignored );
   return 1;
}

int luagi_status_list_new ( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_status_options opts;
   if( luagi_status_init_options( L, 2, &opts ) )
   {
      luaL_error( L, "init options failed" );
   }

   git_status_list **out = (git_status_list **)lua_newuserdata( L, sizeof( git_status_list *) );

   if( git_status_list_new( out, *repo, &opts ) )
   {
      return ltk_push_error( L );
   }

   ltk_setmetatable(L, LUAGI_STATUS_FUNCS);
   return 1;
}
// list operations
int luagi_status_list_entrycount( lua_State *L )
{
   git_status_list **list = checkstatuslist( L );
   lua_pushnumber( L, git_status_list_entrycount( *list ) );
   return 1;
}

int luagi_status_by_index( lua_State *L )
{
   git_status_list **list = checkstatuslist( L );
   int idx = luaL_checkinteger( L, 2 );
   // the index in lua is 1 based
   idx = idx - 1;

   const git_status_entry *entry = git_status_byindex( *list, idx );

   if( entry == NULL )
   {
      return ltk_push_error_msg( L, "index out of bounds" );
   }

   lua_newtable( L );

   luagi_status_flags_to_table( L, entry->status );
   lua_setfield( L , -2, STATUS );

   // git_diff_delta

   if( entry->head_to_index != NULL )
   {
      diff_delta_to_table( L, entry->head_to_index );
      lua_setfield( L , -2, HEAD_TO_IDX );
   }

   if( entry->index_to_workdir != NULL )
   {
      diff_delta_to_table( L, entry->index_to_workdir );
      lua_setfield( L , -2, IDX_TO_WRK );
   }
   
   return 1;
}

int luagi_status_list_free( lua_State *L )
{
   git_status_list **list = checkstatuslist( L );
   git_status_list_free( *list ); 
   return 0;
}

static int luagi_status_walker( lua_State *L )
{
   struct status_list* info = (struct status_list*) lua_touserdata( L, lua_upvalueindex( 1 ) );
   if ( info != NULL && info->next != NULL )
   {
      struct status_element *elem = info->next;
      info->next = elem->next;
      lua_pushstring( L, elem->path );
      // prepare boolean table to get rid of the integer flags
      luagi_status_flags_to_table( L, elem->status_flags );
      free_elem( elem );
      return 2;
   }//iteration finished
   return 0;
}

static int flags_from_lua( lua_State *L, int idx )
{
   int flags = 0;
   add_flag( flags, L, idx, 
                   INC_UNTRACKED, 
                   GIT_STATUS_OPT_INCLUDE_UNTRACKED );
   add_flag( flags, L, idx, 
                   INC_IGNORED, 
                   GIT_STATUS_OPT_INCLUDE_IGNORED );
   add_flag( flags, L, idx, 
                   INC_UNMODIFIED, 
                   GIT_STATUS_OPT_INCLUDE_UNMODIFIED );
   add_flag( flags, L, idx, 
                   EX_SUBMODULES, 
                   GIT_STATUS_OPT_EXCLUDE_SUBMODULES );
   add_flag( flags, L, idx, 
                   REC_UNTRACKED_DIRS, 
                   GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS );
   add_flag( flags, L, idx, 
                   DISABLE_PATHSPEC, 
                   GIT_STATUS_OPT_DISABLE_PATHSPEC_MATCH );
   add_flag( flags, L, idx, 
                   REC_IGNORED_DIRS, 
                   GIT_STATUS_OPT_RECURSE_IGNORED_DIRS );
   add_flag( flags, L, idx, 
                   REN_HEAD_TO_IDX, 
                   GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX );
   add_flag( flags, L, idx, 
                   REN_IDX_TO_WDIR, 
                   GIT_STATUS_OPT_RENAMES_INDEX_TO_WORKDIR );
   add_flag( flags, L, idx, 
                   SORT_SENSI, 
                   GIT_STATUS_OPT_SORT_CASE_SENSITIVELY );
   add_flag( flags, L, idx, 
                   SORT_INSENSI, 
                   GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY );
   add_flag( flags, L, idx, 
                   REN_FROM_REWRITE, 
                   GIT_STATUS_OPT_RENAMES_FROM_REWRITES );
   add_flag( flags, L, idx, 
                   NO_REFRESH, 
                   GIT_STATUS_OPT_NO_REFRESH );
   return flags;
}
