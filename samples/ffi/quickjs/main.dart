/*
 * @Author: kingweicai 
 * @Date: 2019-09-22 23:04:01 
 * @Last Modified by: kingweicai
 * @Last Modified time: 2019-09-23 11:06:40
 */


import 'dart:ffi' as ffi;

// FFI signature of the hello_world C function
// c 函数签名定义
typedef hello_world_func = ffi.Void Function();
typedef runQxs = ffi.String Function(ffi.String, );
// Dart type definition for calling the C foreign function
// Dart 的 函数类型定义
typedef HelloWorld = void Function();

main() {
  // Open the dynamic library
  final dylib = ffi.DynamicLibrary.open('hello_world.dylib');
  // Look up the C function 'hello_world'
  final HelloWorld hello = dylib
      .lookup<ffi.NativeFunction<hello_world_func>>('hello_world')
      .asFunction();
  // Call the function

  final String qxsCode = "function test(a, b){ retrun a + b }";
  final List<int> qxsCodeList = qxsCode.codeUnits;
  

  hello();
}
