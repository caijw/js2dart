/*
 * @Author: kingweicai 
 * @Date: 2019-09-22 23:20:57 
 * @Last Modified by: kingweicai
 * @Last Modified time: 2019-09-23 15:34:41
 */

#include <quickjs/quickjs.h>
#include <quickjs/quickjs-libc.h>
#include "jscore.h"
#include <string.h>

enum {
    FALSE = 0,
    TRUE = 1,
};

const char greeting_code[] = "console.log('hello world');";

static int eval_buf_1(JSContext *ctx, const void *buf, int buf_len, const char *filename, int eval_flags, JSValue *val) {
    int ret = 0;
    if ((eval_flags & JS_EVAL_TYPE_MASK) == JS_EVAL_TYPE_MODULE) {
        /* for the modules, we compile then run to be able to set
           import.meta */
        *val = JS_Eval(ctx, buf, buf_len, filename, eval_flags | JS_EVAL_FLAG_COMPILE_ONLY);
        if (!JS_IsException(*val)) {
            js_module_set_import_meta(ctx, *val, TRUE, TRUE);
            *val = JS_EvalFunction(ctx, *val);
        }
    } else {
        *val = JS_Eval(ctx, buf, buf_len, filename, eval_flags);
    }
    return ret;
}

/**
 * 运行一段 hello world 的 js 代码
 */
void hello_world() {
    int argc = 0;
    char *argv[1] = {NULL};
    JSRuntime *rt;
    JSContext *ctx;
    JSValue val;
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
    ret = eval_buf_1(ctx, greeting_code, strlen(greeting_code), "test.js", JS_EVAL_TYPE_GLOBAL, &val);
    if (JS_IsException(val)) {
        js_std_dump_error(ctx);
    }
    JS_FreeValue(ctx, val);
    js_std_loop(ctx);
    js_std_free_handlers(rt);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
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

/**
 * 运行 dart 传过来的 js 代码，同时返回一个 char * 的运行结果
 */
char *runJs(char *str, int length) {
    int argc = 0;
    char *argv[1] = {NULL};
    int ret;
    char *ret_str = "";
    JSRuntime *rt; // 可以放到全局，不需要每次都新建一个 runtime
    JSContext *ctx;
    JSValue val;
    rt = JS_NewRuntime();
    ctx = JS_NewContext(rt);
    js_std_add_helpers(ctx, argc, argv);
    ret = eval_buf_1(ctx, str, length, "qxs.js", JS_EVAL_TYPE_GLOBAL, &val); // 需要处理 ret 非0
    if (JS_IsException(val)) {
        js_std_dump_error(ctx);
        return ret_str;
    }
    const char *valStr = JS_ToCString(ctx, val);
    int valStrLen = strlen(valStr);
    ret_str = (char *)malloc((valStrLen + 1) * sizeof(char));
    strcpy(ret_str, valStr);
    JS_FreeValue(ctx, val);
    js_std_loop(ctx);
    js_std_free_handlers(rt);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    return ret_str;
}
