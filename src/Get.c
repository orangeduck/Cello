#include "Cello.h"

static const char* Get_Name(void) {
  return "Get";
}

static const char* Get_Brief(void) {
  return "Gettable or Settable";
}

static const char* Get_Description(void) {
  return
    "The `Get` class provides a method to _get_ or _set_ certain properties "
    "of an object using keys and value. Typically it is implemented by "
    "data lookup structures such as `Table` or `Map` but it is also used "
    "more generally such as using indices to look up items in `Array`, or "
    "as thread local storage for the `Thread` object.";
}

static const char* Get_Definition(void) {
  return
    "struct Get {\n"
    "  var  (*get)(var, var);\n"
    "  void (*set)(var, var, var);\n"
    "  bool (*mem)(var, var);\n"
    "  void (*rem)(var, var);\n"
    "  var (*key_type)(var);\n"
    "  var (*val_type)(var);\n"
    "};\n";
}

static struct Example* Get_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage 1",
      "var x = new(Array, String, \n"
      "  $S(\"Hello\"), $S(\"There\"));\n"
      "\n"
      "show(get(x, $I(0))); /* Hello */\n"
      "show(get(x, $I(1))); /* There */\n"
      "set(x, $I(1), $S(\"Blah\"));\n"
      "show(get(x, $I(1))); /* Blah */\n"
    }, {
      "Usage 2",
      "var prices = new(Table, String, Int, \n"
      "  $S(\"Apple\"),  $I(12),\n"
      "  $S(\"Banana\"), $I( 6),\n"
      "  $S(\"Pear\"),   $I(55));\n"
      "\n"
      "var pear_price   = get(prices, $S(\"Pear\"));\n"
      "var banana_price = get(prices, $S(\"Banana\"));\n"
      "var apple_price  = get(prices, $S(\"Apple\"));\n"
      "\n"
      "show(pear_price);   /* 55 */\n"
      "show(banana_price); /*  6 */\n"
      "show(apple_price);  /* 12 */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* Get_Methods(void) {
  
  static struct Method methods[] = {
    {
      "get", 
      "var get(var self, var key);",
      "Get the value at a given `key` for object `self`."
    }, {
      "set", 
      "void set(var self, var key, var val);",
      "Set the value at a given `key` for object `self`."
    }, {
      "mem", 
      "bool mem(var self, var key);",
      "Returns true if `key` is a member of the object `self`."
    }, {
      "rem", 
      "void rem(var self, var key);",
      "Removes the `key` from object `self`."
    }, {
      "key_type", 
      "var key_type(var self);",
      "Returns the key type for the object `self`."
    }, {
      "val_type", 
      "var val_type(var self);",
      "Returns the value type for the object `self`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Get = Cello(Get,
  Instance(Doc,
    Get_Name,       Get_Brief,    Get_Description, 
    Get_Definition, Get_Examples, Get_Methods));

var get(var self, var key) {
  return method(self, Get, get, key);
}

void set(var self, var key, var val) {
  method(self, Get, set, key, val);
}

bool mem(var self, var key) {
  return method(self, Get, mem, key);
}

void rem(var self, var key) {
  method(self, Get, rem, key);
}

var key_type(var self) {
  return method(self, Get, key_type);
}

var val_type(var self) {
  return method(self, Get, val_type);  
}
