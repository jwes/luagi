#ifndef IS_LUAGI_REMOTE
#define IS_LUAGI_REMOTE
#include <lua.h>
int luagi_remote_list( lua_State *L );
int luagi_remote_load( lua_State *L );
int luagi_remote_create_anonymous( lua_State *L );
int luagi_remote_create_with_fetchspec( lua_State *L );
int luagi_remote_create( lua_State *L );
int luagi_remote_save( lua_State *L );
int luagi_remote_owner( lua_State *L );
int luagi_remote_name( lua_State *L );
int luagi_remote_url( lua_State *L );
int luagi_remote_pushurl( lua_State *L );
int luagi_remote_set_url( lua_State *L );
int luagi_remote_set_pushurl( lua_State *L );
int luagi_remote_add_fetch( lua_State *L );
int luagi_remote_get_fetch_refspecs( lua_State *L );
int luagi_remote_set_fetch_refspecs( lua_State *L );
int luagi_remote_add_push( lua_State *L );
int luagi_remote_get_push_refspecs( lua_State *L );
int luagi_remote_set_push_refspecs( lua_State *L );
int luagi_remote_clear_refspecs( lua_State *L );
int luagi_remote_refspec_count( lua_State *L );
int luagi_remote_get_refspec( lua_State *L );
int luagi_remote_connect( lua_State *L );
int luagi_remote_ls( lua_State *L );
int luagi_remote_download( lua_State *L );
int luagi_remote_fetch( lua_State *L );
int luagi_remote_connected( lua_State *L );
int luagi_remote_stop( lua_State *L );
int luagi_remote_disconnect( lua_State *L );
int ligt_remote_free( lua_State *L );
int luagi_remote_check_cert( lua_State *L );
int luagi_remote_set_transport( lua_State *L );
int luagi_remote_set_callbacks( lua_State *L );
int luagi_remote_stats( lua_State *L );
int luagi_remote_autotag( lua_State *L );
int luagi_remote_set_autotag( lua_State *L );
int luagi_remote_rename( lua_State *L );
int luagi_remote_update_fetch_head( lua_State *L );
int luagi_remote_set_update_fetch_head( lua_State *L );
int luagi_remote_update_tips( lua_State *L );
int luagi_remote_is_valid_name( lua_State *L );
int luagi_remote_valid_url( lua_State *L );
int luagi_remote_supported_url( lua_State *L );

static const struct luaL_Reg luagi_remote_funcs [] = {
   { "save",               luagi_remote_save                    },
   { "owner",              luagi_remote_owner                   },
   { "name",               luagi_remote_name                    },
   { "url",                luagi_remote_url                     },
   { "pushurl",            luagi_remote_pushurl                 },
   { "set_url",            luagi_remote_set_url                 },
   { "set_pushurl",        luagi_remote_set_pushurl             },
   { "add_fetch",          luagi_remote_add_fetch               },
   { "fetch_refspecs",     luagi_remote_get_fetch_refspecs      },
   { "set_fetch_refspecs", luagi_remote_set_fetch_refspecs      },
   { "add_push",           luagi_remote_add_push                },
   { "get_push_refspecs",  luagi_remote_get_push_refspecs       },
   { "set_push_refspecs",  luagi_remote_set_push_refspecs       },
   { "clear_refspecs",     luagi_remote_clear_refspecs          },
   { "refspec_count",      luagi_remote_refspec_count           },
   { "get_refspec",        luagi_remote_get_refspec             },
   { "connect",            luagi_remote_connect                 },
   { "ls",                 luagi_remote_ls                      },
   { "download",           luagi_remote_download                },
   { "fetch",              luagi_remote_fetch                   },
   { "is_connected",       luagi_remote_connected               },
   { "stop",               luagi_remote_stop                    },
   { "disconnect",         luagi_remote_disconnect              },
   { "__gc",               ligt_remote_free                    },
   { "check_cert",         luagi_remote_check_cert              },
   { "set_transport",      luagi_remote_set_transport           },
   { "set_callbacks",      luagi_remote_set_callbacks           },
   { "stats",              luagi_remote_stats                   },
   { "autotag",            luagi_remote_autotag                 },
   { "set_autotag",        luagi_remote_set_autotag             },
   { "rename",             luagi_remote_rename                  },
   { "fetch_head",         luagi_remote_update_fetch_head       },
   { "set_fetch_head",     luagi_remote_set_update_fetch_head   },
   { "update_tips",        luagi_remote_update_tips             },
   { NULL,                 NULL                                },
};
#endif
