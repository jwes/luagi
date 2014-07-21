#ifndef IS_LGIT_REMOTE
#define IS_LGIT_REMOTE
#include <lua.h>
int lgit_remote_list( lua_State *L );
int lgit_remote_load( lua_State *L );
int lgit_remote_create_anonymous( lua_State *L );
int lgit_remote_create_with_fetchspec( lua_State *L );
int lgit_remote_create( lua_State *L );
int lgit_remote_save( lua_State *L );
int lgit_remote_owner( lua_State *L );
int lgit_remote_name( lua_State *L );
int lgit_remote_url( lua_State *L );
int lgit_remote_pushurl( lua_State *L );
int lgit_remote_set_url( lua_State *L );
int lgit_remote_set_pushurl( lua_State *L );
int lgit_remote_add_fetch( lua_State *L );
int lgit_remote_get_fetch_refspecs( lua_State *L );
int lgit_remote_set_fetch_refspecs( lua_State *L );
int lgit_remote_add_push( lua_State *L );
int lgit_remote_get_push_refspecs( lua_State *L );
int lgit_remote_set_push_refspecs( lua_State *L );
int lgit_remote_clear_refspecs( lua_State *L );
int lgit_remote_refspec_count( lua_State *L );
int lgit_remote_get_refspec( lua_State *L );
int lgit_remote_connect( lua_State *L );
int lgit_remote_ls( lua_State *L );
int lgit_remote_download( lua_State *L );
int lgit_remote_fetch( lua_State *L );
int lgit_remote_connected( lua_State *L );
int lgit_remote_stop( lua_State *L );
int lgit_remote_disconnect( lua_State *L );
int ligt_remote_free( lua_State *L );
int lgit_remote_check_cert( lua_State *L );
int lgit_remote_set_transport( lua_State *L );
int lgit_remote_set_callbacks( lua_State *L );
int lgit_remote_stats( lua_State *L );
int lgit_remote_autotag( lua_State *L );
int lgit_remote_set_autotag( lua_State *L );
int lgit_remote_rename( lua_State *L );
int lgit_remote_update_fetch_head( lua_State *L );
int lgit_remote_set_update_fetch_head( lua_State *L );
int lgit_remote_update_tips( lua_State *L );
int lgit_remote_is_valid_name( lua_State *L );
int lgit_remote_valid_url( lua_State *L );
int lgit_remote_supported_url( lua_State *L );

#define DIRECTION "direction"
#define STRING "representation"
#define SRC "src"
#define DEST "dest"
#define PUSH "push"
#define FETCH "fetch"
#define UNKNOWN "unknown"
#define FORCE "force"
#define ALL "all"
#define NONE "none"
#define AUTO "auto"

static const struct luaL_Reg lgit_remote_funcs [] = {
   { "save",               lgit_remote_save                    },
   { "owner",              lgit_remote_owner                   },
   { "name",               lgit_remote_name                    },
   { "url",                lgit_remote_url                     },
   { "pushurl",            lgit_remote_pushurl                 },
   { "set_url",            lgit_remote_set_url                 },
   { "set_pushurl",        lgit_remote_set_pushurl             },
   { "add_fetch",          lgit_remote_add_fetch               },
   { "fetch_refspecs",     lgit_remote_get_fetch_refspecs      },
   { "set_fetch_refspecs", lgit_remote_set_fetch_refspecs      },
   { "add_push",           lgit_remote_add_push                },
   { "get_push_refspecs",  lgit_remote_get_push_refspecs       },
   { "set_push_refspecs",  lgit_remote_set_push_refspecs       },
   { "clear_refspecs",     lgit_remote_clear_refspecs          },
   { "refspec_count",      lgit_remote_refspec_count           },
   { "get_refspec",        lgit_remote_get_refspec             },
   { "connect",            lgit_remote_connect                 },
   { "ls",                 lgit_remote_ls                      },
   { "download",           lgit_remote_download                },
   { "fetch",              lgit_remote_fetch                   },
   { "is_connected",       lgit_remote_connected               },
   { "stop",               lgit_remote_stop                    },
   { "disconnect",         lgit_remote_disconnect              },
   { "__gc",               ligt_remote_free                    },
   { "check_cert",         lgit_remote_check_cert              },
   { "set_transport",      lgit_remote_set_transport           },
   { "set_callbacks",      lgit_remote_set_callbacks           },
   { "stats",              lgit_remote_stats                   },
   { "autotag",            lgit_remote_autotag                 },
   { "set_autotag",        lgit_remote_set_autotag             },
   { "rename",             lgit_remote_rename                  },
   { "fetch_head",         lgit_remote_update_fetch_head       },
   { "set_fetch_head",     lgit_remote_set_update_fetch_head   },
   { "update_tips",        lgit_remote_update_tips             },
   { NULL,                 NULL                                },
};
#endif
