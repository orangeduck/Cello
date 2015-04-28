#include "Cello.h" 

const char* Reserve_Name(void) {
  return "Reserve";
}

const char* Reserve_Brief(void) {
  return "Resources can be Preallocated";
}

const char* Reserve_Description(void) {
  return
    "The `Reserve` class can be implemented by objects which allow for some "
    "resource or other to be preallocated or reserved. For example this class "
    "is implemented by `Array` and `Table` to preallocate memory space if it "
    "is known that many items are going to be added at a later date.";
}

static struct DocMethod* Reserve_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "reserve", 
      "void reserve(var self, var amount);",
      "Reserve some `amount` of resource for object `self`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

static struct DocExample* Reserve_Examples(void) {
  
  static struct DocExample examples[] = {
    {
      "Usage",
      "var x = new(Array, Int);\n"
      "reserve(x, $I(10000)); /* Reserve space in Array */ \n"
      "for (size_t i = 0; i < 10000; i++) {\n"
      "  push(x, $I(i));\n"
      "}\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

var Reserve = Cello(Reserve, 
  Instance(Doc,
    Reserve_Name, Reserve_Brief, Reserve_Description, 
    Reserve_Examples, Reserve_Methods));
  
void reserve(var self, var amount) {
  method(self, Reserve, reserve, amount);
}

