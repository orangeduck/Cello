#include "Cello.h"

static const char* Concat_Name(void) {
  return "Concat";
}

static const char* Concat_Brief(void) {
  return "Concatenate Objects";
}

static const char* Concat_Description(void) {
  return
    "The `Concat` class is implemented by objects that can have other objects "
    "either _appended_ to their, on _concatenated_ to them. For example "
    "collections or strings.";
}

static const char* Concat_Definition(void) {
  return
    "struct Concat {\n"
    "  void (*concat)(var, var);\n"
    "  void (*append)(var, var);\n"
    "};\n";
}

static struct Example* Concat_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var x = new(Array, Float, $F(9.9), $F(2.8));\n"
      "var y = new(Array, Float, $F(1.1), $F(6.5));\n"
      "\n"
      "show(x); /* <'Array' At 0x00414603 [9.9, 2.8]> */\n"
      "show(y); /* <'Array' At 0x00414603 [1.1, 6.5]> */\n"
      "append(x, $F(2.5));\n"
      "show(x); /* <'Array' At 0x00414603 [9.9, 2.8, 2.5]> */\n"
      "concat(x, y);\n"
      "show(x); /* <'Array' At 0x00414603 [9.9, 2.8, 2.5, 1.1, 6.5]> */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* Concat_Methods(void) {
  
  static struct Method methods[] = {
    {
      "append", 
      "void append(var self, var obj);",
      "Append the object `obj` to the object `self`."
    }, {
      "concat", 
      "void concat(var self, var obj);",
      "Concatenate the object `obj` to the object `self`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Concat = Cello(Concat,
  Instance(Doc,
    Concat_Name,       Concat_Brief,    Concat_Description,
    Concat_Definition, Concat_Examples, Concat_Methods));

void append(var self, var obj) {
  method(self, Concat, append, obj);
}

void concat(var self, var obj) {
  method(self, Concat, concat, obj);
}
