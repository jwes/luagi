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
int luagi_note_author( lua_State *L );
int luagi_note_committer( lua_State *L );

#define LUAGI_NOTE_FUNCS "is.luagi.note"
#define LUAGI_NOTE_ITER_FUNCS "is.luagi.note.iter"
#define checknote_at( L, N ) \
   (git_note **) luaL_checkudata( L, N, LUAGI_NOTE_FUNCS )

static const struct luaL_Reg luagi_note_funcs [] = {
   { "message", luagi_note_message },
   { "__tostring", luagi_note_message },
   { "id", luagi_note_id },
   { "author", luagi_note_author },
   { "committer", luagi_note_committer },
   { "__gc", luagi_note_free },
   { NULL, NULL }
};

#endif
