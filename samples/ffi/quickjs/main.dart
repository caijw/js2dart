/*
 * @Author: kingweicai 
 * @Date: 2019-09-22 23:04:01 
 * @Last Modified by: kingweicai
 * @Last Modified time: 2019-09-23 14:38:47
 */


import 'dart:ffi' as ffi;
import 'dart:convert';

// FFI signature of the hello_world C function
// c 函数签名定义
typedef hello_world_func = ffi.Void Function();

// Dart type definition for calling the C foreign function
// Dart 的 函数类型定义
typedef HelloWorld = void Function();

class Utf8 extends ffi.Struct<Utf8> {
  @ffi.Uint8()
  int char;

  static String fromUtf8(ffi.Pointer<Utf8> ptr) {
    final units = List<int>();
    var len = 0;
    while (true) {
      final char = ptr.elementAt(len++).load<Utf8>().char;
      if (char == 0) break;
      units.add(char);
    }
    return Utf8Decoder().convert(units);
  }

  static ffi.Pointer<Utf8> toUtf8(String s) {
    final units = Utf8Encoder().convert(s);
    final ptr = ffi.Pointer<Utf8>.allocate(count: units.length + 1);
    for (var i = 0; i < units.length; i++) {
      ptr.elementAt(i).load<Utf8>().char = units[i];
    }
    // Add the C string null terminator '\0'
    ptr.elementAt(units.length).load<Utf8>().char = 0;
    return ptr;
  }
}

// C string parameter pointer function - char *reverse(char *str, int length);
typedef reverse_func = ffi.Pointer<Utf8> Function(
    ffi.Pointer<Utf8> str, ffi.Int32 length);
typedef Reverse = ffi.Pointer<Utf8> Function(ffi.Pointer<Utf8> str, int length);


main() {
  // Open the dynamic library
  final jscore = ffi.DynamicLibrary.open('jscore.dylib');
  // Look up the C function 'hello_world'
  final HelloWorld hello = jscore
      .lookup<ffi.NativeFunction<hello_world_func>>('hello_world')
      .asFunction();
  // Call the function
  hello();

  final reversePointer =
      jscore.lookup<ffi.NativeFunction<reverse_func>>('reverse');
  final reverse = reversePointer.asFunction<Reverse>();
  final reversedMessage = Utf8.fromUtf8(reverse(Utf8.toUtf8('backwards'), 9));
  print('$reversedMessage');

}
