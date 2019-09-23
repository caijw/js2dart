/*
 * @Author: kingweicai 
 * @Date: 2019-09-22 23:20:57 
 * @Last Modified by: kingweicai
 * @Last Modified time: 2019-09-23 14:39:17
 */

#include <quickjs/quickjs.h>
#include <quickjs/quickjs-libc.h>
#include "jscore.h"
#include <string.h>

enum {
    FALSE = 0,
    TRUE = 1,
};

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


const char filename[] = "test.js";
const char greeting_code[] = "console.log('hello world', 1);";



static int eval_buf_1(JSContext *ctx, const void *buf, int buf_len,
                    const char *filename, int eval_flags)
{
    JSValue val;
    int ret;

    if ((eval_flags & JS_EVAL_TYPE_MASK) == JS_EVAL_TYPE_MODULE) {
        /* for the modules, we compile then run to be able to set
           import.meta */
        val = JS_Eval(ctx, buf, buf_len, filename,
                      eval_flags | JS_EVAL_FLAG_COMPILE_ONLY);
        if (!JS_IsException(val)) {
            js_module_set_import_meta(ctx, val, TRUE, TRUE);
            val = JS_EvalFunction(ctx, val);
        }
    } else {
        val = JS_Eval(ctx, buf, buf_len, filename, eval_flags);
    }
    if (JS_IsException(val)) {
        js_std_dump_error(ctx);
        ret = -1;
    } else {
        ret = 0;
    }
    JS_FreeValue(ctx, val);
    return ret;
}

/**
 * 翻转字符串，返回翻转后的结果
 */
char *reverse(char *str, int length){
    char *reversed_str = (char *)malloc((length + 1) * sizeof(char));
    for (int i = 0; i < length; i++)
    {
        reversed_str[length - i - 1] = str[i];
    }
    reversed_str[length] = '\0';
    return reversed_str;
}

void hello_world() {
    int argc = 0;
    char *argv[1] = {NULL};
    JSRuntime *rt;
    JSContext *ctx;

    //int eval_flags = JS_EVAL_TYPE_GLOBAL;
    int ret;
    rt = JS_NewRuntime();
    ctx = JS_NewContext(rt);
    /* loader for ES6 modules */
    // JS_SetModuleLoaderFunc(rt, NULL, js_module_loader, NULL);
    js_std_add_helpers(ctx, argc, argv);

    /* system modules */
    // js_init_module_std(ctx, "std");
    // js_init_module_os(ctx, "os");
    /* make 'std' and 'os' visible to non module code */
    // if (1) {
    //     const char *str = "import * as std from 'std';\n"
    //         "import * as os from 'os';\n"
    //         "globalThis.std = std;\n"
    //         "globalThis.os = os;\n";
    //     eval_buf_1(ctx, str, strlen(str), "<input>", JS_EVAL_TYPE_MODULE);
    // }

    // eval script
    ret = eval_buf_1(ctx, greeting_code, strlen(greeting_code), "test.js", JS_EVAL_TYPE_GLOBAL);

    js_std_loop(ctx);
    js_std_free_handlers(rt);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
}
