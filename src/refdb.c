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
#include "refdb.h"

#include <git2/refdb.h>

#include "ltk.h"
#include "luagi.h"

int luagi_refdb_new( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_refdb **refdb = lua_newuserdata( L, sizeof( git_refdb * ) );

   if( git_refdb_new( refdb, *repo ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_REFDB_FUNCS );
   return 1;
}

int luagi_refdb_open( lua_State *L )
{
   git_repository **repo = checkrepo( L, 1 );
   git_refdb **refdb = lua_newuserdata( L, sizeof( git_refdb * ) );

   if( git_refdb_open( refdb, *repo ) )
   {
      return ltk_push_git_error( L );
   }
   ltk_setmetatable( L, LUAGI_REFDB_FUNCS );
   return 1;
}

   

//refdb
int luagi_refdb_compress( lua_State *L )
{
   git_refdb **refdb = checkrefdb_at( L, 1 );

   if( git_refdb_compress( *refdb ) )
   {
      ltk_error_abort( L );
   }
   return 0;
}

int luagi_refdb_free( lua_State *L )
{
   git_refdb **refdb = checkrefdb_at( L, 1 );
   git_refdb_free( *refdb );
   return 0;
}

