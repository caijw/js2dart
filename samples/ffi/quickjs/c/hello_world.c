/*
 * @Author: kingweicai 
 * @Date: 2019-09-22 23:20:57 
 * @Last Modified by: kingweicai
 * @Last Modified time: 2019-09-22 23:24:04
 */

#include <quickjs/quickjs.h>
#include <quickjs/quickjs-libc.h>
#include "hello_world.h"

const uint32_t qjsc_hello_size = 87;

const uint8_t qjsc_hello[87] = {
 0x01, 0x04, 0x0e, 0x63, 0x6f, 0x6e, 0x73, 0x6f,
 0x6c, 0x65, 0x06, 0x6c, 0x6f, 0x67, 0x16, 0x48,
 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72,
 0x6c, 0x64, 0x22, 0x65, 0x78, 0x61, 0x6d, 0x70,
 0x6c, 0x65, 0x73, 0x2f, 0x68, 0x65, 0x6c, 0x6c,
 0x6f, 0x2e, 0x6a, 0x73, 0x0d, 0x00, 0x06, 0x00,
 0x9e, 0x01, 0x00, 0x01, 0x00, 0x03, 0x00, 0x00,
 0x14, 0x01, 0xa0, 0x01, 0x00, 0x00, 0x00, 0x39,
 0xd0, 0x00, 0x00, 0x00, 0x43, 0xd1, 0x00, 0x00,
 0x00, 0x04, 0xd2, 0x00, 0x00, 0x00, 0x24, 0x01,
 0x00, 0xcc, 0x28, 0xa6, 0x03, 0x01, 0x00,
};

void hello_world() {
    int argc = 0;
    char *argv[1] = {NULL};
    JSRuntime *rt;
    JSContext *ctx;
    rt = JS_NewRuntime();
    ctx = JS_NewContextRaw(rt);
    JS_AddIntrinsicBaseObjects(ctx);
    js_std_add_helpers(ctx, argc, argv);
    js_std_eval_binary(ctx, qjsc_hello, qjsc_hello_size, 0);
    js_std_loop(ctx);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
}
