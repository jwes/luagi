#ifndef IS_LGIT_CLONE
#define IS_LGIT_CLONE
#include <lua.h>

#define LGIT_CLONE_OPTS_VERSION "version"
#define LGIT_CLONE_OPTS_CHECKOUT_OPTS "checkout_options"
#define LGIT_CLONE_OPTS_REMOTE_CALLBACKS "remote_callbacks"
#define LGIT_CLONE_OPTS_BARE "bare"
#define LGIT_CLONE_OPTS_IGNORE_CERT_ERR "ignore_cert_errors"
#define LGIT_CLONE_OPTS_REMOTE "remote_name"
#define LGIT_CLONE_OPTS_CHECKOUT_BRANCH "checkout_branch"
#define LGIT_CLONE_OPTS_SIGNATURE "signature"

int lgit_clone( lua_State *L );
int lgit_clone_into( lua_State *L );
#endif
