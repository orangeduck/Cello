#include "Cello.h" 

static const char* Resize_Name(void) {
  return "Reserve";
}

static const char* Resize_Brief(void) {
  return "Object can be resized";
}

static const char* Resize_Description(void) {
  return
    "The `Resize` class can be implemented by objects which can be resized in "
    "some way. Resizing to a larger size than the current may allow for some "
    "resource or other to be preallocated or reserved. For example this class "
    "is implemented by `Array` and `Table` to either remove a number of items "
    "quickly or to preallocate memory space if it is known that many items are "
    "going to be added at a later date.";
}

static const char* Resize_Definition(void) {
  return
    "struct Resize {\n"
    "  void (*resize)(var, size_t);\n"
    "};\n";
}

static struct Method* Resize_Methods(void) {
  
  static struct Method methods[] = {
    {
      "resize", 
      "void resize(var self, size_t n);",
      "Resize to some size `n`, perhaps reserving some resource for object "
      "`self`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

static struct Example* Resize_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var x = new(Array, Int);\n"
      "resize(x, 10000); /* Reserve space in Array */ \n"
      "for (size_t i = 0; i < 10000; i++) {\n"
      "  push(x, $I(i));\n"
      "}\n"
    }, {
      "Usage 2",
      "var x = new(Array, Int, $I(0), $I(1), $I(2));\n"
      "resize(x, 0); /* Clear Array of items */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

var Resize = Cello(Resize, 
  Instance(Doc,
    Resize_Name,       Resize_Brief,    Resize_Description, 
    Resize_Definition, Resize_Examples, Resize_Methods));
  
void resize(var self, size_t n) {
  method(self, Resize, resize, n);
}

