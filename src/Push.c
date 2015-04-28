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

static const char* Push_Definition(void) {
  return
    "struct Push {\n"
    "  void (*push)(var, var);\n"
    "  void (*pop)(var);\n"
    "  void (*push_at)(var, var, var);\n"
    "  void (*pop_at)(var, var);\n"
    "};\n";
}

static struct Example* Push_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var x = new(Array, Int);\n"
      "\n"
      "push(x, $I( 0));\n"
      "push(x, $I( 5));\n"
      "push(x, $I(10));\n"
      "\n"
      "show(get(x, $I(0))); /*  0 */\n"
      "show(get(x, $I(1))); /*  5 */\n"
      "show(get(x, $I(2))); /* 10 */\n"
      "\n"
      "pop_at(x, $I(1));\n"
      "\n"
      "show(get(x, $I(0))); /*  0 */\n"
      "show(get(x, $I(1))); /* 10 */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* Push_Methods(void) {
  
  static struct Method methods[] = {
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

var Push = Cello(Push,
  Instance(Doc,
    Push_Name,       Push_Brief,    Push_Description, 
    Push_Definition, Push_Examples, Push_Methods));

void push(var self, var val) { method(self, Push, push, val); }
void push_at(var self, var val, var i) { method(self, Push, push_at, val, i); }
void pop(var self) { method(self, Push, pop); }
void pop_at(var self, var i) { method(self, Push, pop_at, i); }
