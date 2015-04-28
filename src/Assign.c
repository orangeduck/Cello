#include "Cello.h"

static const char* Assign_Name(void) {
  return "Assign";
}

static const char* Assign_Brief(void) {
  return "Construction from Object";
}

static const char* Assign_Description(void) {
  return
    "`Assign` is potentially the most important class in Cello. It is used "
    "throughout Cello to initialise objects using other objects. In C++ this is "
    "called the _copy constructor_ and it is used to assign the value of one "
    "object to another."
    "\n\n"
    "By default the `Assign` class uses the `Size` class to copy the memory "
    "from one object to another. But for more complex objects which maintain "
    "their own behaviours and state this may need to be overridden."
    "\n\n"
    "The most important thing about the `Assign` class is that it must work on "
    "the assumption that the target object could be uninitialised with zero'd "
    "memory.";
}

static const char* Assign_Definition(void) {
  return
    "struct Assign {\n"
    "  void (*assign)(var, var);\n"
    "};\n";
}

static struct Example* Assign_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var x = new(Int, $I(10));\n"
      "var y = new(Int, $I(20));\n"
      "\n"
      "show(x); /* 10 */\n"
      "show(y); /* 20 */\n"
      "\n"
      "assign(x, y);\n"
      "\n"
      "show(x); /* 20 */\n"
      "show(y); /* 20 */\n"
    }, {NULL, NULL}
  };
  
  return examples;
}

static struct Method* Assign_Methods(void) {
  
  static struct Method methods[] = {
    {
      "assign", 
      "var assign(var self, var obj);",
      "Assign the object `obj` to the object `self`. The assigned object "
      "`self` is returned."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Assign = Cello(Assign,
  Instance(Doc,
    Assign_Name,       Assign_Brief,    Assign_Description,
    Assign_Definition, Assign_Examples, Assign_Methods));

var assign(var self, var obj) {
  
  struct Assign* a = instance(self, Assign);
  
  if (a and a->assign) {
    a->assign(self, obj);
    return self;
  }
  
  size_t s = size(type_of(self));
  if (type_of(self) is type_of(obj) and s) {
    return memcpy(self, obj, s);
  }
  
  return throw(TypeError,
    "Cannot assign type %s to type %s", type_of(obj), type_of(self));
  
}
