# c_module

```makefile
# examples/c_module.js -> c_module.c
# TODO 这里的 examples/fib.so 应该就是 .obj/fib.o，还是说文件名不重要，只有有符号定义即可
./qjsc -e -M examples/fib.so,fib -m -o c_module.c examples/c_module.js

# c_module.c -> .obj/c_module.o
clang -g -Wall -MMD -MF .obj/c_module.o.d -Wextra -Wno-sign-compare -Wno-missing-field-initializers -Wundef -Wuninitialized -Wunused -Wno-unused-parameter -Wwrite-strings -Wchar-subscripts -funsigned-char -MMD -MF .obj/c_module.o.d -D_GNU_SOURCE -DCONFIG_VERSION=\"2019-09-18\" -O2 -flto -c -o .obj/c_module.o c_module.c

# examples/fib.c -> .obj/fib.o
clang -g -Wall -MMD -MF .obj/fib.o.d -Wextra -Wno-sign-compare -Wno-missing-field-initializers -Wundef -Wuninitialized -Wunused -Wno-unused-parameter -Wwrite-strings -Wchar-subscripts -funsigned-char -MMD -MF .obj/fib.o.d -D_GNU_SOURCE -DCONFIG_VERSION=\"2019-09-18\" -O2 -flto -c -o .obj/fib.o examples/fib.c

# TODO
ar rcs libquickjs.lto.a .obj/quickjs.o .obj/libregexp.o .obj/libunicode.o .obj/cutils.o .obj/quickjs-libc.o

# .obj/c_module.o .obj/fib.o libquickjs.lto.a -> examples/c_module
# -lm math库
# -ldl dl库
clang -g -flto -o examples/c_module .obj/c_module.o .obj/fib.o libquickjs.lto.a -lm -ldl
```
