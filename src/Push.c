#include "Cello.h"

static const char* Push_Name(void) {
  return "Push";
}

static const char* Push_Brief(void) {
  return "Pushable and Popable object";
}

static const char* Push_Description(void) {
  return ""
    "The `Push` class provides an interface for the addition and removal of "
    "objects from another in a positional sense."
    "\n\n"
    "`push` can be used to add new objects to a collection and `pop` to remove "
    "them. Usage of `push` can require `assign` to be defined on the argument.";
}

static struct DocMethod* Push_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "push", 
      "void push(var self, var obj);",
      "Push the object `obj` onto the top of object `self`."
    }, {
      "pop", 
      "void pop(var self);",
      "Pop the top item from the object `self`."
    }, {
      "push_at", 
      "void push_at(var self, var obj, var key);",
      "Push the object `obj` onto the object `self` at a given `key`."
    }, {
      "pop_at", 
      "void pop_at(var self, var key);",
      "Pop the object from the object `self` at a given `key`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

/* TODO: Examples */

var Push = Cello(Push,
  Instance(Doc,
    Push_Name, Push_Brief, Push_Description, NULL, Push_Methods));

void push(var self, var val) { method(self, Push, push, val); }
void push_at(var self, var val, var i) { method(self, Push, push_at, val, i); }
void pop(var self) { method(self, Push, pop); }
void pop_at(var self, var i) { method(self, Push, pop_at, i); }
