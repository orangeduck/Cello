#include "Cello.h"

static const char* Assign_Name(void) {
  return "Assign";
}

static const char* Assign_Brief(void) {
  return "Assignment";
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
    "the assumption that the target object may not have had it's constructor "
    "called and could be uninitialised with just zero'd memory. This is often "
    "the case when copying contents into containers.";
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

static const char* Swap_Name(void) {
  return "Swap";
}

static const char* Swap_Brief(void) {
  return "Swapable";
}

static const char* Swap_Description(void) {
  return
    "The `Swap` class can be used to override the behaviour of swapping two "
    "objects. By default the `Swap` class simply swaps the memory of the "
    "two objects passed in as parameters making use of the `Size` class. "
    "In almost all cases this default behaviour should be fine, even if the "
    "objects have custom assignment functions."
    "\n\n"
    "Swapping can be used internally by various collections and algorithms.";
}

static const char* Swap_Definition(void) {
  return
    "struct Swap {\n"
    "  void (*swap)(var, var);\n"
    "};\n";
}

static struct Example* Swap_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var x = $S(\"Hello\");\n"
      "var y = $S(\"World\");\n"
      "show(x); /* Hello */\n"
      "show(y); /* World */\n"
      "swap(x, y);\n"
      "show(x); /* World */\n"
      "show(y); /* Hello */\n"
    }, {NULL, NULL}
  };
  
  return examples;
}

static struct Method* Swap_Methods(void) {
  
  static struct Method methods[] = {
    {
      "swap", 
      "void swap(var self, var obj);",
      "Swap the object `self` for the object `obj`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Swap = Cello(Swap,
  Instance(Doc,
    Swap_Name,       Swap_Brief,    Swap_Description, 
    Swap_Definition, Swap_Examples, Swap_Methods));

static void memswap(void* p0, void* p1, size_t s) {
  if (p0 == p1) { return; }
  for (size_t i = 0; i < s; i++) {
    char t = ((char*)p0)[i];
    ((char*)p0)[i] = ((char*)p1)[i];
    ((char*)p1)[i] = t;
  }
}
    
void swap(var self, var obj) {
  
  struct Swap* s = instance(self, Swap);
  if (s and s->swap) {
    s->swap(self, obj);
    return;
  }
  
  size_t n = size(type_of(self));
  if (type_of(self) is type_of(obj) and n) {
    memswap(self, obj, n);
    return;
  }
  
  throw(TypeError,
    "Cannot swap type %s and type %s", type_of(obj), type_of(self));
  
}
