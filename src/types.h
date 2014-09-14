#ifndef IS_LUAGI_TYPES
#define IS_LUAGI_TYPES
#include <git2/types.h>

git_otype luagi_otype_from_string( const char *tstring );
const char *luagi_string_from_otype( const git_otype type );

#endif
