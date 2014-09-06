#ifndef IS_LUAGI_NOTE
#define IS_LUAGI_NOTE
#include <lua.h>
#include <lauxlib.h>

//repository
int luagi_note_iterator( lua_State *L );
int luagi_note_iterator_free( lua_State *L );

int luagi_note_foreach( lua_State *L );

int luagi_note_read( lua_State *L );
int luagi_note_create( lua_State *L );
int luagi_note_remove( lua_State *L );
int luagi_note_default_ref( lua_State *L );

int luagi_note_message( lua_State *L );
int luagi_note_id( lua_State *L );
int luagi_note_free( lua_State *L );

#define LUAGI_NOTE_FUNCS "is.luagi.note"
#define LUAGI_NOTE_ITER_FUNCS "is.luagi.note.iter"
#define checknote_at( L, N ) \
   (git_note **) luaL_checkudata( L, N, LUAGI_NOTE_FUNCS )

static const struct luaL_Reg luagi_note_funcs [] = {
   { "message", luagi_note_message },
   { "__tostring", luagi_note_message },
   { "id", luagi_note_id },
   { "__gc", luagi_note_free },
   { NULL, NULL }
};

#endif
