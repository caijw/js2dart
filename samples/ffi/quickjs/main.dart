/*
 * @Author: kingweicai 
 * @Date: 2019-09-22 23:04:01 
 * @Last Modified by: kingweicai
 * @Last Modified time: 2019-09-23 15:38:34
 */


import 'dart:ffi' as ffi;
import 'dart:convert';

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

// FFI signature of the hello_world C function
typedef hello_world_func = ffi.Void Function();
// Dart type definition for calling the C foreign function
typedef HelloWorld = void Function();

// C string parameter pointer function - char *reverse(char *str, int length);
typedef reverse_func = ffi.Pointer<Utf8> Function(ffi.Pointer<Utf8> str, ffi.Int32 length);
typedef Reverse = ffi.Pointer<Utf8> Function(ffi.Pointer<Utf8> str, int length);

typedef runJs_func = ffi.Pointer<Utf8> Function(ffi.Pointer<Utf8> str, ffi.Int32 length);
typedef RunJs = ffi.Pointer<Utf8> Function(ffi.Pointer<Utf8> str, int length);

// qxs 的代码，需要构造出来
final qxsCode = '''
  (function test(a, b) {
    a = a || "1";
    b = b || "2";
    var res = a + b;
    for (var i = 0; i < 10; ++i) {
      res += i;
    }
    return res;
  })("x", "y");
''';

main() {

  // Open the dynamic library
  final jscore = ffi.DynamicLibrary.open('jscore.dylib');

  // dart 调用 jscore 运行一段 hello world 的 js 代码
  final HelloWorld hello = jscore.lookup<ffi.NativeFunction<hello_world_func>>('hello_world').asFunction();
  hello();

  // dart 传递字符串给 c，c 反转字符串后返回给 dart
  final reversePointer = jscore.lookup<ffi.NativeFunction<reverse_func>>('reverse');
  final reverse = reversePointer.asFunction<Reverse>();
  final reversedMessage = Utf8.fromUtf8(reverse(Utf8.toUtf8('backwards'), 9));
  print('reversedMessage $reversedMessage');

  // dart 调用 jscore 运行 qxs 的 js 代码
  final runJsPointer = jscore.lookup<ffi.NativeFunction<runJs_func>>('runJs');
  final runJs = runJsPointer.asFunction<RunJs>();
  final qxsRes = Utf8.fromUtf8(runJs(Utf8.toUtf8(qxsCode), qxsCode.length));
  print('qxsRes $qxsRes');
}
