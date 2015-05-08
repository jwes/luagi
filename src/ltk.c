#include "ltk.h"

extern inline void ltk_error_abort( lua_State *L );

extern inline int ltk_push_error_msg( lua_State *L, const char *msg );
extern inline int ltk_push_error( lua_State *L );

extern inline void ltk_setmetatable( lua_State *L, const char *name );
