#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <quickjs.h>

int main(void) {
  JSRuntime *rt = JS_NewRuntime();
  JSContext *ctx = JS_NewContext(rt);

  char *const fooCode = "function foo(a) { global.a = a; return a; }";
  if (JS_IsException(JS_Eval(ctx, fooCode, strlen(fooCode), "<input>", JS_EVAL_FLAG_STRICT))) {
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    return -1;
  }

  JSValue global = JS_GetGlobalObject(ctx);
  JSValue foo = JS_GetPropertyStr(ctx, global, "foo");
  JSValue obj = JS_NewObject(ctx);
  JSValue argv[] = { obj };
  JSValue jsResult = JS_Call(ctx, foo, global, sizeof(argv) / sizeof(JSValue), argv);

  JSValue used[] = { 
    jsResult, 
    // argv[0],
    foo,
    global
  };

  for (int i = 0; i < sizeof(used) / sizeof(JSValue); ++i) {
    JS_FreeValue(ctx, used[i]);
  }

  JS_FreeContext(ctx);
  JS_FreeRuntime(rt);
  return 0;
}
