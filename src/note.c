#include "note.h"

#include <git2/notes.h>
#include <git2/signature.h>

#include "ltk.h"
#include "luagi.h"
#include "oid.h"

static int luagi_push_note( lua_State *L, git_repository *repo, git_oid *oid, const char * notes_ref )
{
   git_note **note = lua_newuserdata( L, sizeof( git_note *) );

   if( git_note_read( note, repo, notes_ref, oid ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_NOTE_FUNCS );
   return 1;
}

static int note_iter( lua_State *L )
{
   git_note_iterator **iter = lua_touserdata( L, lua_upvalueindex( 1 ) );

   git_oid note_id, annotated_id;

   int ret = git_note_next( &note_id, &annotated_id, *iter );
   if( ret == 0 )
   {

      luagi_push_oid( L, &note_id );
      luagi_push_oid( L, &annotated_id );

      return 2;
   } 
   else if( ret == GIT_ITEROVER )
   {
      return 0;
   }
   else 
   {
      ltk_error_abort( L );
      return 0;
   }
}

int luagi_note_iterator( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   const char *note_ref = luaL_optstring( L, 2, NULL );

   git_note_iterator** iter =(git_note_iterator**) lua_newuserdata(L, sizeof(git_note_iterator*));

   if( git_note_iterator_new( iter, *repo, note_ref ) )
   {
      ltk_error_abort( L );
   }

   ltk_setmetatable( L, LUAGI_NOTE_ITER_FUNCS );

   lua_pushcclosure( L, note_iter, 1 );
   return 1;
}

int luagi_note_iterator_free( lua_State *L )
{
   git_note_iterator **it = luaL_checkudata( L, 1, LUAGI_NOTE_ITER_FUNCS );
   git_note_iterator_free( *it );
   return 0;
}

static int note_foreach_cb( const git_oid *blob_id, const git_oid *annotated_object_id, void *payload )
{
   luagi_foreach_t *p = payload;
   lua_pushvalue( p->L, p->callback_pos );
   luagi_push_oid( p->L, blob_id );
   luagi_push_oid( p->L, annotated_object_id );

   if( lua_pcall( p->L, 2, 1, 0 ) )
   {
      ltk_error_abort( p->L );
      return 1;
   }

   int ret = luaL_optinteger( p->L, -1, 0 );
   lua_pop( p->L, 1 );
   return ret;
}

int luagi_note_foreach( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   luaL_checktype( L, 2, LUA_TFUNCTION );
   const char *notes_ref = luaL_optstring( L, 3, NULL );
   luagi_foreach_t *p = malloc( sizeof( luagi_foreach_t ) );
   
   p->L = L;
   p->callback_pos = 2;
   
   if( git_note_foreach( *repo, notes_ref, note_foreach_cb, p ) )
   {
      ltk_error_abort( L );
   }

   return 0;
}

int luagi_note_read( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_oid oid;
   luagi_check_oid( &oid, L, 2 );
   const char *notes_ref = luaL_optstring( L, 3, NULL );
   return luagi_push_note( L, *repo, &oid, notes_ref );
}

int luagi_note_create( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_signature *author, *committer;
   table_to_signature( L, &author, 2 ); 
   table_to_signature( L, &committer, 3 ); 

   git_oid oid, out;
   luagi_check_oid( &oid, L, 4 );
   const char *note = luaL_checkstring( L, 5 );
   const char *notes_ref = luaL_optstring( L, 6, NULL );
   int force = lua_toboolean( L, 7 );

   int ret = git_note_create( &out, *repo, notes_ref, author, committer, &oid, note, force );

   git_signature_free( author );
   git_signature_free( committer );
   if( ret )
   {
      return ltk_push_git_error( L );
   }
   ret = 0;
   ret += luagi_push_note( L, *repo, &oid, notes_ref );
   ret += luagi_push_oid( L, &out );
   return ret; 
}

int luagi_note_remove( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_signature *author, *committer;
   table_to_signature( L, &author, 2); 
   table_to_signature( L, &committer, 3 ); 

   git_oid oid;
   luagi_check_oid( &oid, L, 4 );
   const char *notes_ref = luaL_optstring( L, 5, NULL );

   int ret = git_note_remove( *repo, notes_ref, author, committer, &oid );
   git_signature_free( author );
   git_signature_free( committer );

   if( ret )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_note_default_ref( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );

   const char *out;

   if( git_note_default_ref( &out, *repo ) )
   {
      return ltk_push_git_error( L );
   }
   lua_pushstring( L, out );
   return 1;
}

int luagi_note_message( lua_State *L )
{
   git_note **note = checknote_at( L, 1 );

   lua_pushstring( L, git_note_message( *note ) );
   return 1;
}

int luagi_note_id( lua_State *L )
{
   git_note **note = checknote_at( L, 1 );

   const git_oid *oid = git_note_id( *note );

   return luagi_push_oid( L, oid );
}

static int luagi_note_signature( lua_State *L, const git_signature * (*func)(const git_note*))
{
   git_note **note = checknote_at( L, 1 );
   return signature_to_table( L, func( *note ) );
}

int luagi_note_author( lua_State *L )
{
   return luagi_note_signature( L, git_note_author );
}

int luagi_note_committer( lua_State *L )
{
   return luagi_note_signature( L, git_note_committer );
}

int luagi_note_free( lua_State *L )
{
   git_note **note = checknote_at( L, 1 );
   git_note_free( *note );
   return 0;
}
