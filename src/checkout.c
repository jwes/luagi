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
#include "checkout.h"

#include <git2/checkout.h>

#include "index.h"
#include "ltk.h"
#include "luagi.h"
#include "object.h"
#include "tree.h"

int luagi_parse_checkout_options( git_checkout_options *opts, lua_State *L, const int tableIndex )
{
   lua_getfield( L, tableIndex, LUAGI_CHECKOUT_OPTS_VERSION );
   int version = luaL_optinteger( L, -1, GIT_CHECKOUT_OPTIONS_VERSION );

   git_checkout_init_options( opts, version );
   opts->checkout_strategy = GIT_CHECKOUT_NONE;

   lua_getfield( L, tableIndex, FLAGS );
   if( lua_type( L, -1 ) == LUA_TTABLE )
   {
      add_flag( opts->checkout_strategy, L, -1, 
                      SAFE, GIT_CHECKOUT_SAFE );
      add_flag( opts->checkout_strategy, L, -1, 
                      SAFE_CREATE, GIT_CHECKOUT_SAFE_CREATE );
      add_flag( opts->checkout_strategy, L, -1, 
                      FORCE, GIT_CHECKOUT_FORCE );
      add_flag( opts->checkout_strategy, L, -1, 
                      ALLOW_CONFLICTS, GIT_CHECKOUT_ALLOW_CONFLICTS );
      add_flag( opts->checkout_strategy, L, -1, 
                      REMOVE_UNTRACKED, GIT_CHECKOUT_REMOVE_UNTRACKED );
      add_flag( opts->checkout_strategy, L, -1, 
                      REMOVE_IGNORED, GIT_CHECKOUT_REMOVE_IGNORED );
      add_flag( opts->checkout_strategy, L, -1, 
                      UPDATE_ONLY, GIT_CHECKOUT_UPDATE_ONLY );
      add_flag( opts->checkout_strategy, L, -1, 
                      DONT_UPDATE_INDEX, GIT_CHECKOUT_DONT_UPDATE_INDEX );
      add_flag( opts->checkout_strategy, L, -1, 
                      NO_REFRESH, GIT_CHECKOUT_NO_REFRESH );
      add_flag( opts->checkout_strategy, L, -1, 
                      SKIP_UNMERGED, GIT_CHECKOUT_SKIP_UNMERGED );
      add_flag( opts->checkout_strategy, L, -1, 
                      USE_OURS, GIT_CHECKOUT_USE_OURS );
      add_flag( opts->checkout_strategy, L, -1, 
                      USE_THEIRS, GIT_CHECKOUT_USE_THEIRS );
      add_flag( opts->checkout_strategy, L, -1, 
                      DISABLE_PATHSPEC_MATCH, GIT_CHECKOUT_DISABLE_PATHSPEC_MATCH );
      add_flag( opts->checkout_strategy, L, -1, 
                      SKIP_LOCKED_DIRECTORIES, GIT_CHECKOUT_SKIP_LOCKED_DIRECTORIES );
      add_flag( opts->checkout_strategy, L, -1, 
                      DONT_OVERWRITE_IGNORED, GIT_CHECKOUT_DONT_OVERWRITE_IGNORED );
      add_flag( opts->checkout_strategy, L, -1, 
                      CONFLICT_STYLE_MERGE, GIT_CHECKOUT_CONFLICT_STYLE_MERGE );
      add_flag( opts->checkout_strategy, L, -1, 
                      CONFLICT_STYLE_DIFF3, GIT_CHECKOUT_CONFLICT_STYLE_DIFF3 );
      add_flag( opts->checkout_strategy, L, -1, 
                      UPDATE_SUBMODULES, GIT_CHECKOUT_UPDATE_SUBMODULES );
      add_flag( opts->checkout_strategy, L, -1, 
                      UPDATE_SUBMODULES_IF_CHANGED, GIT_CHECKOUT_UPDATE_SUBMODULES_IF_CHANGED );
   }

   lua_getfield( L, tableIndex, DISABLE_FILTERS );
   opts->disable_filters = lua_toboolean( L, -1 );

   lua_getfield( L, tableIndex, DIR_MODE );
   unsigned int dir_mode = luaL_optinteger(L, -1, -1 );
   if( dir_mode < 0xFFFFu )
   {
      opts->dir_mode = dir_mode;
   }

   lua_getfield( L, tableIndex, FILE_MODE );
   unsigned int file_mode = luaL_optinteger(L, -1, -1 );
   if( file_mode < 0xFFFFu )
   {
      opts->file_mode = file_mode;
   }

   lua_getfield( L, tableIndex, FILE_OPEN_FLAGS );
   int file_open_flags = luaL_optinteger(L, -1, -1 );
   if( file_open_flags >= 0 )
   {
      opts->file_open_flags = file_open_flags;
   }

   lua_getfield( L, tableIndex, NOTIFY_FLAGS );
   if( lua_type( L, -1 ) == LUA_TTABLE )
   {
      add_flag( opts->notify_flags, L, -1, 
                      NONE, GIT_CHECKOUT_NOTIFY_NONE );
      add_flag( opts->notify_flags, L, -1, 
                      CONFLICT, GIT_CHECKOUT_NOTIFY_CONFLICT );
      add_flag( opts->notify_flags, L, -1, 
                      DIRTY, GIT_CHECKOUT_NOTIFY_DIRTY );
      add_flag( opts->notify_flags, L, -1, 
                      UPDATED, GIT_CHECKOUT_NOTIFY_UPDATED );
      add_flag( opts->notify_flags, L, -1, 
                      UNTRACKED, GIT_CHECKOUT_NOTIFY_UNTRACKED );
      add_flag( opts->notify_flags, L, -1, 
                      IGNORED, GIT_CHECKOUT_NOTIFY_IGNORED );
      add_flag( opts->notify_flags, L, -1, 
                      ALL, GIT_CHECKOUT_NOTIFY_ALL );
   }
   // TODO: NOTIFY AND PROGRESS Callbacks 
   
   lua_getfield( L, tableIndex, PATHS );
   if( lua_type( L, -1 ) == LUA_TTABLE )
   {
      opts->paths = ltk_check_strarray( L, -1 );
   }

   lua_getfield( L, tableIndex, BASELINE );
   if( lua_type( L, -1 ) == LUA_TUSERDATA )
   {
      opts->baseline = *(git_tree **)luaL_checkudata( L, -1, LUAGI_TREE_FUNCS );
   }

   lua_getfield( L, tableIndex, TARGET );
   if( lua_type( L, -1 ) == LUA_TSTRING )
   {
      opts->target_directory = luaL_checkstring( L, -1 );
   }

   lua_getfield( L, tableIndex, ANCESTOR_LABEL );
   if( lua_type( L, -1 ) == LUA_TSTRING )
   {
      opts->ancestor_label = luaL_checkstring( L, -1 );
   }
   
   lua_getfield( L, tableIndex, OUR_LABEL );
   if( lua_type( L, -1 ) == LUA_TSTRING )
   {
      opts->our_label = luaL_checkstring( L, -1 );
   }

   lua_getfield( L, tableIndex, THEIR_LABEL );
   if( lua_type( L, -1 ) == LUA_TSTRING )
   {
      opts->their_label = luaL_checkstring( L, -1 );
   }
   return 0;
}

int luagi_checkout_head( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_checkout_options opts;
   luagi_parse_checkout_options( &opts, L, 2 );

   if( git_checkout_head( *repo, &opts ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_checkout_index( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_index **index = checkindex_at( L, 2 );

   git_checkout_options opts;
   luagi_parse_checkout_options( &opts, L, 3 );

   if( git_checkout_index( *repo, *index, &opts ) )
   {
      ltk_error_abort( L );
   }

   return 0;
}

int luagi_checkout_tree( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_object **obj = checkobject_at( L ,2 );

   git_checkout_options opts;
   luagi_parse_checkout_options( &opts, L, 3 );

   if( git_checkout_tree( *repo, *obj, &opts ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

