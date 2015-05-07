#include "Cello.h"

static const char* Len_Name(void) {
  return "Len";
}

static const char* Len_Brief(void) {
  return "Has a length";
}

static const char* Len_Description(void) {
  return
    "The `Len` class can be implemented by any type that has a length "
    "associated with it. It is typically implemented by collections "
    "and is often used in conjunction with `Iter` or `Get`.";
}

static const char* Len_Definition(void) {
  return
    "struct Len {\n"
    "  size_t (*len)(var);\n"
    "};\n";
}

static struct Example* Len_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var x = new(Array, Int, $I(1), $I(2), $I(5));\n"
      "show($I(len(x))); /* 3 */\n"
      "var y = $S(\"Test\");\n"
      "show($I(len(y))); /* 4 */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* Len_Methods(void) {
  
  static struct Method methods[] = {
    {
      "len", 
      "size_t len(var self);",
      "Returns the length of object `self`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Len = Cello(Len,
  Instance(Doc,
    Len_Name,       Len_Brief,    Len_Description, 
    Len_Definition, Len_Examples, Len_Methods));

size_t len(var self) {
  return method(self, Len, len);
}

bool empty(var self) {
  return len(self) is 0;
}
