#include "Cello.h"

static const char* Concat_Name(void) {
  return "Concat";
}

static const char* Concat_Brief(void) {
  return "Join Object";
}

static const char* Concat_Description(void) {
  return
    "The `Concat` class is implemented by objects that can have other objects "
    "either _appended_ to their, on _concatenated_ to them. For example "
    "collections or strings.";
}

static struct DocMethod* Concat_Methods(void) {
  
  static struct DocMethod methods[] = {
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

/* TODO: Examples */

var Concat = Cello(Concat,
  Instance(Doc,
    Concat_Name, Concat_Brief, Concat_Description,
    NULL, Concat_Methods));

void append(var self, var obj) {
  method(self, Concat, append, obj);
}

void concat(var self, var obj) {
  method(self, Concat, concat, obj);
}