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
#ifndef IS_LUAGI_REPOSITORY
#define IS_LUAGI_REPOSITORY

#include <lua.h>
int luagi_open( lua_State *L );
int luagi_gc( lua_State *L );
int luagi_repository_wrap_odb( lua_State *L );
int luagi_repository_discover( lua_State *L );
int luagi_repository_open_ext( lua_State *L );
int luagi_repository_open_bare( lua_State *L );
int luagi_repository_init( lua_State *L );
int luagi_repository_init_ext( lua_State *L );
int luagi_repository_head( lua_State *L );
int luagi_repository_head_detached( lua_State *L );
int luagi_repository_head_unborn( lua_State *L );
int luagi_repository_is_empty( lua_State *L );
int luagi_repository_path( lua_State *L );
int luagi_repository_workdir( lua_State *L );
int luagi_repository_set_workdir( lua_State *L );
int luagi_repository_is_bare( lua_State *L );
int luagi_repository_config( lua_State *L );
int luagi_repository_config_snapshot( lua_State *L );
int luagi_repository_odb( lua_State *L );
int luagi_repository_refdb( lua_State *L );
int luagi_repository_index( lua_State *L );
int luagi_repository_message( lua_State *L );
int luagi_repository_message_remove( lua_State *L );
int luagi_repository_state_cleanup( lua_State *L );
int luagi_repository_fetchhead_foreach( lua_State *L );
int luagi_repository_mergehead_foreach( lua_State *L );
int luagi_repository_hashfile( lua_State *L );
int luagi_repository_set_head( lua_State *L );
int luagi_repository_set_head_detached( lua_State *L );
int luagi_repository_detach_head( lua_State *L );
int luagi_repository_state( lua_State *L );
int luagi_repository_set_namespace( lua_State *L );
int luagi_repository_get_namespace( lua_State *L );
int luagi_repository_is_shallow( lua_State *L );

#endif
