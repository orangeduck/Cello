#include "Cello.h"

static const char* Iter_Name(void) {
  return "Iter";
}

static const char* Iter_Brief(void) {
  return "Can be looped over";
}

/* TODO */
static const char* Iter_Description(void) {
  return
    "The `Iter` class is implemented by types which can be looped over. This "
    "allows them to be used in conjunction with the `foreach` macro as well "
    "as various other components of Cello.";
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

/* TODO: Examples */

var Iter = Cello(Iter,
  Instance(Doc,
    Iter_Name, Iter_Brief, Iter_Description, NULL, Iter_Methods));

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

