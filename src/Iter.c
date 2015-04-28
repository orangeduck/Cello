#include "Cello.h"

static const char* Iter_Name(void) {
  return "Iter";
}

static const char* Iter_Brief(void) {
  return "Can be looped over";
}

static const char* Iter_Description(void) {
  return
    "The `Iter` class is implemented by types which can be looped over. This "
    "allows them to be used in conjunction with the `foreach` macro as well "
    "as various other components of Cello.";
}

static struct DocExample* Iter_Examples(void) {
  
  static struct DocExample examples[] = {
    {
      "Usage",
      "var x = new(Array, Int, $I(1), $I(2), $I(5));\n"
      "\n"
      "foreach(o in x) {\n"
      "  show(o); /* 1, 2, 5 */\n"
      "}\n"
    }, {
      "Table",
      "var prices = new(Table, String, Int);\n"
      "set(prices, $S(\"Apple\"),  $I(12));\n"
      "set(prices, $S(\"Banana\"), $I( 6));\n"
      "set(prices, $S(\"Pear\"),   $I(55));\n"
      "\n"
      "foreach(key in prices) {\n"
      "  var price = get(prices, key);\n"
      "  print(\"Price of %$ is %$\\n\", key, price);\n"
      "}\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct DocMethod* Iter_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "iter_init", 
      "var iter_init(var self);\n    "
      "var iter_last(var self);",
      "Return the initial item (or final item) in the iteration over `self`."
    }, {
      "iter_next", 
      "var iter_next(var self, var curr);\n    "
      "var iter_prev(var self, var curr);",
      "Given the current item `curr`, return the next (or previous) item in "
      "the iteration over `self`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Iter = Cello(Iter,
  Instance(Doc,
    Iter_Name, Iter_Brief, Iter_Description, Iter_Examples, Iter_Methods));

var iter_init(var self) {
  return method(self, Iter, iter_init);
}

var iter_next(var self, var curr) {
  return method(self, Iter, iter_next, curr);
}

var iter_last(var self) {
  return method(self, Iter, iter_last);
}

var iter_prev(var self, var curr) {
  return method(self, Iter, iter_prev, curr);
}

