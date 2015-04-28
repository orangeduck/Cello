#include "Cello.h"

static const char* Subtype_Name(void) {
  return "Subtype";
}

static const char* Subtype_Brief(void) {
  return "Contains elements of some Subtype";
}

static const char* Subtype_Description(void) {
  return
    "The `Subtype` class can be used to find the type of elements contained "
    "within another class. For example if an `Array` of a particular type is "
    "created, this class can be used to find what type that was.";
}

static const char* Subtype_Definition(void) {
  return
    "struct Subtype {\n"
    "  var (*subtype)(var);\n"
    "  var (*key_subtype)(var);\n"
    "  var (*val_subtype)(var);\n"
    "};\n";
}

static struct Example* Subtype_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var x = new(Array, Int);\n"
      "show(subtype(x)); /* Int */\n"
      "var y = new(Table, String, Int);\n"
      "show(key_subtype(y)); /* String */\n"
      "show(val_subtype(y)); /* Int */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* Subtype_Methods(void) {
  
  static struct Method methods[] = {
    {
      "subtype", 
      "var subtype(var self);",
      "Returns the subtype of object `self`."
    }, {
      "key_subtype", 
      "var key_subtype(var self);",
      "Returns the key subtype of object `self`."
    }, {
      "val_subtype", 
      "var val_subtype(var self);",
      "Returns the value subtype of object `self`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Subtype = Cello(Subtype,
  Instance(Doc,
    Subtype_Name,       Subtype_Brief,    Subtype_Description, 
    Subtype_Definition, Subtype_Examples, Subtype_Methods));

var subtype(var self) {
  return method(self, Subtype, subtype);
}

var key_subtype(var self) {
  return method(self, Subtype, key_subtype);
}

var val_subtype(var self) {
  return method(self, Subtype, val_subtype);
}
