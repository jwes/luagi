#ifndef IS_LUAGI_CLONE
#define IS_LUAGI_CLONE
#include <lua.h>

#define LUAGI_CLONE_OPTS_VERSION "version"
#define LUAGI_CLONE_OPTS_CHECKOUT_OPTS "checkout_options"
#define LUAGI_CLONE_OPTS_REMOTE_CALLBACKS "remote_callbacks"
#define LUAGI_CLONE_OPTS_BARE "bare"
#define LUAGI_CLONE_OPTS_IGNORE_CERT_ERR "ignore_cert_errors"
#define LUAGI_CLONE_OPTS_REMOTE "remote_name"
#define LUAGI_CLONE_OPTS_CHECKOUT_BRANCH "checkout_branch"
#define LUAGI_CLONE_OPTS_SIGNATURE "signature"

int luagi_clone( lua_State *L );
int luagi_clone_into( lua_State *L );
#endif
