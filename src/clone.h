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
