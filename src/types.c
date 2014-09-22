#include <string.h>
#include "types.h"
#include "defines.h"
git_otype luagi_otype_from_string( const char *str )
{
   git_otype ret = GIT_OBJ_BAD;
   if( strncmp( str, ANY, strlen( ANY )) )
   {
      ret = GIT_OBJ_ANY;
   }
   else if( strncmp( str, _EXT1, strlen( _EXT1 )) )
   {
      ret = GIT_OBJ__EXT1;
   }
   else if( strncmp( str, COMMIT, strlen( COMMIT )) )
   {
      ret = GIT_OBJ_COMMIT;
   }
   else if( strncmp( str, TREE, strlen( TREE )) )
   {
      ret = GIT_OBJ_TREE;
   }
   else if( strncmp( str, BLOB, strlen( BLOB )) )
   {
      ret = GIT_OBJ_BLOB;
   }
   else if( strncmp( str, TAG, strlen( TAG )) )
   {
      ret = GIT_OBJ_TAG;
   }
   else if( strncmp( str, _EXT2, strlen( _EXT2 )) )
   {
      ret = GIT_OBJ__EXT2;
   }
   else if( strncmp( str, OFS_DELTA, strlen( OFS_DELTA )) )
   {
      ret = GIT_OBJ_OFS_DELTA;
   }
   else if( strncmp( str, REF_DELTA, strlen( REF_DELTA )) )
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
