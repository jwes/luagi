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
#ifndef IS_LUAGI_TAG
#define IS_LUAGI_TAG
#include <lua.h>
#include <lauxlib.h>

// repo funcs
int luagi_tag_lookup( lua_State *L );
int luagi_tag_create( lua_State *L );
int luagi_tag_annotation_create( lua_State *L );
int luagi_tag_create_frombuffer( lua_State *L );
int luagi_tag_create_lightweight( lua_State *L );
int luagi_tag_delete( lua_State *L );

int luagi_tag_list( lua_State *L );
int luagi_tag_list_match( lua_State *L );
int luagi_tag_foreach( lua_State *L );

#define LUAGI_TAG_FUNCS "is.luagi.tag"

#define checktag_at( L, N ) \
   (git_tag **) luaL_checkudata( L, N, LUAGI_TAG_FUNCS );

// tag functions
int luagi_tag_free( lua_State *L );
int luagi_tag_id( lua_State *L );
int luagi_tag_target( lua_State *L );
int luagi_tag_target_id( lua_State *L );
int luagi_tag_target_type( lua_State *L );
int luagi_tag_name( lua_State *L );
int luagi_tag_tagger( lua_State *L );
int luagi_tag_message( lua_State *L );
int luagi_tag_peel( lua_State *L );

static const struct luaL_Reg luagi_tag_funcs [] = {
   { "__gc", luagi_tag_free },
   { "id", luagi_tag_id },
   { "target", luagi_tag_target },
   { "target_id", luagi_tag_target_id },
   { "name", luagi_tag_name },
   { "__tostring", luagi_tag_name },
   { "tagger", luagi_tag_tagger },
   { "message", luagi_tag_message },
   { "peel", luagi_tag_peel },
   { NULL, NULL }
};

#endif
