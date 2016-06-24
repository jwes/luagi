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

#include <string.h>
#include "types.h"
#include "defines.h"

git_otype luagi_otype_from_string( const char *str )
{
   git_otype ret = GIT_OBJ_BAD;
   if( strncmp( str, ANY, strlen( ANY )) == 0 )
   {
      ret = GIT_OBJ_ANY;
   }
   else if( strncmp( str, _EXT1, strlen( _EXT1 )) == 0 )
   {
      ret = GIT_OBJ__EXT1;
   }
   else if( strncmp( str, COMMIT, strlen( COMMIT )) == 0 )
   {
      ret = GIT_OBJ_COMMIT;
   }
   else if( strncmp( str, TREE, strlen( TREE )) == 0 )
   {
      ret = GIT_OBJ_TREE;
   }
   else if( strncmp( str, BLOB, strlen( BLOB )) == 0 )
   {
      ret = GIT_OBJ_BLOB;
   }
   else if( strncmp( str, TAG, strlen( TAG )) == 0 )
   {
      ret = GIT_OBJ_TAG;
   }
   else if( strncmp( str, _EXT2, strlen( _EXT2 )) == 0 )
   {
      ret = GIT_OBJ__EXT2;
   }
   else if( strncmp( str, OFS_DELTA, strlen( OFS_DELTA )) == 0 )
   {
      ret = GIT_OBJ_OFS_DELTA;
   }
   else if( strncmp( str, REF_DELTA, strlen( REF_DELTA )) == 0 )
   {
      ret = GIT_OBJ_REF_DELTA;
   }
   return ret;
}

const char *luagi_string_from_otype( const git_otype type )
{
   switch( type )
   {
      default:
      case GIT_OBJ_ANY:
         return ANY;
      case GIT_OBJ_BAD:
         return BAD;
      case GIT_OBJ__EXT1:
         return _EXT1;
      case GIT_OBJ_COMMIT:
         return COMMIT;
      case GIT_OBJ_TREE:
         return TREE;
      case GIT_OBJ_BLOB:
         return BLOB;
      case GIT_OBJ_TAG:
         return TAG;
      case GIT_OBJ__EXT2:
         return _EXT2;
      case GIT_OBJ_OFS_DELTA:
         return OFS_DELTA;
      case GIT_OBJ_REF_DELTA:
         return REF_DELTA;
   }
}
