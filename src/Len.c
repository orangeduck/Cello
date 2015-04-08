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

static struct DocMethod* Len_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "len", 
      "size_t len(var self);",
      "Returns the length of object `self`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

/* TODO: Examples */

var Len = Cello(Len,
  Instance(Doc,
    Len_Name, Len_Brief, Len_Description, NULL, Len_Methods));

size_t len(var self) {
  return method(self, Len, len);
}

bool empty(var self) {
  return len(self) is 0;
}
