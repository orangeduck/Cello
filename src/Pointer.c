#include "Cello.h"

static const char* Pointer_Name(void) {
  return "Pointer";
}

static const char* Pointer_Brief(void) {
  return "Reference to other object";
}

static const char* Pointer_Description(void) {
  return
    "The `Pointer` class is implemented by types which act as references to "
    "other objects. Primarily this class is implemented by `Ref` and `Box` "
    "which provide the two main pointer types in Cello.";
}

static const char* Pointer_Definition(void) {
  return
    "struct Pointer {\n"
    "  void (*ref)(var, var);\n"
    "  var (*deref)(var);\n"
    "};\n";
}

static struct Example* Pointer_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var obj0 = $F(1.0), obj1 = $F(2.0);\n"
      "var r = $(Ref, obj0);\n"
      "show(r);\n"
      "show(deref(r)); /* 1.0 */\n"
      "ref(r, obj1);\n"
      "show(deref(r)); /* 2.0 */\n"
      "assign(r, obj0);\n"
      "show(deref(r)); /* 1.0 */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* Pointer_Methods(void) {
  
  static struct Method methods[] = {
    {
      "ref", 
      "void ref(var self, var item);",
      "Set the object `self` to reference the object `item`."
    }, {
      "deref", 
      "var deref(var self);",
      "Get the object referenced by `self`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Pointer = Cello(Pointer,
  Instance(Doc,
    Pointer_Name,       Pointer_Brief,    Pointer_Description, 
    Pointer_Definition, Pointer_Examples, Pointer_Methods));

void ref(var self, var item) {
  method(self, Pointer, ref, item);
}

var deref(var self) {
  return method(self, Pointer, deref);
}

static const char* Ref_Name(void) {
  return "Ref";
}

static const char* Ref_Brief(void) {
  return "Shared Pointer";
}

static const char* Ref_Description(void) {
  return
    "The `Ref` type is a basic wrapper around a C pointer. It can be used "
    "as a type argument to collections to allow them to store generic types. "
    "It may also be useful in various circumstances where another level of "
    "indirection or mutability is required.";
}

static const char* Ref_Definition(void) {
  return
    "struct Ref {\n"
    "  var val;\n"
    "};\n";
}

static struct Example* Ref_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var obj0 = $F(1.0), obj1 = $F(2.0);\n"
      "var r = $(Ref, obj0);\n"
      "show(r);\n"
      "show(deref(r)); /* 1.0 */\n"
      "ref(r, obj1);\n"
      "show(deref(r)); /* 2.0 */\n"
      "assign(r, obj0);\n"
      "show(deref(r)); /* 1.0 */\n"
    }, {
      "Collections",
      "var i0 = new(Int, $I(100));\n"
      "var i1 = new(Int, $I(200));\n"
      "var x = new(Array, Ref, i0, i1);\n"
      "\n"
      "print(deref(get(x, $I(0)))); /* 100 */"
      "\n"
      "del(x); /* Contents of `x` still alive */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static void Ref_Ref(var self, var val);
static var Ref_Deref(var self);
static void Ref_Assign(var self, var obj);

static void Ref_Assign(var self, var obj) {
  struct Pointer* p = instance(obj, Pointer);
  if (p and p->deref) {
    Ref_Ref(self, p->deref(obj));
  } else {
    Ref_Ref(self, obj);
  }
}

static void Ref_Ref(var self, var val) {
  struct Ref* r = self;
  r->val = val;
}

static var Ref_Deref(var self) {
  struct Ref* r = self;
  return r->val;
}

var Ref = Cello(Ref,
  Instance(Doc,
    Ref_Name, Ref_Brief, Ref_Description, Ref_Definition, Ref_Examples, NULL),
  Instance(Assign,   Ref_Assign),
  Instance(Pointer,  Ref_Ref, Ref_Deref));

  
static const char* Box_Name(void) {
  return "Box";
}

static const char* Box_Brief(void) {
  return "Unique Pointer";
}

static const char* Box_Description(void) {
  return
    "The `Box` type is another wrapper around a C pointer with one additional "
    "behaviour as compared to `Ref`. When a `Box` object is deleted it will "
    "also call `del` on the object it points to. The means a `Box` is "
    "considered a pointer type that _owns_ the object it points to, and so is "
    "responsible for it's destruction. Due to this `Box`s must point to valid "
    "Cello objects and so can't be initalised with `NULL` or anything else "
    "invalid. "
    "\n\n"
    "While this might not seem that useful when there is Garbage Collection "
    "this can be very useful when Garbage Collection is turned off, and when "
    "used in conjunction with collections.";
}

static const char* Box_Definition(void) {
  return
    "struct Box {\n"
    "  var val;\n"
    "};\n";
}

static struct Example* Box_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var obj0 = $F(1.0), obj1 = $F(2.0);\n"
      "var r = $(Box, obj0);\n"
      "show(r);\n"
      "show(deref(r)); /* 1.0 */\n"
      "ref(r, obj1);\n"
      "show(deref(r)); /* 2.0 */\n"
      "assign(r, obj0);\n"
      "show(deref(r)); /* 1.0 */\n"
    }, {
      "Lifetimes",
      "var quote = $S(\"Life is long\");\n"
      "\n"
      "with (r in $B(new(String, quote))) {\n"
      "  println(\"This reference is: %$\", r);\n"
      "  println(\"This string is alive: '%s'\", deref(r));\n"
      "}\n"
      "\n"
      "print(\"Now it has been cleared up!\\n\");\n"
    }, {
      "Collection",
      "/* Multiple Types in one Collection */\n"
      "var x = new(Array, Box, \n"
      "  new(String, $S(\"Hello\")), \n"
      "  new(String, $S(\"There\")), \n"
      "  new(Int, $I(10)));\n"
      "\n"
      "print(deref(get(x, $I(0)))); /* Hello */ \n"
      "\n"
      "del(x); /* Contents of `x` deleted with it */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static void Box_Ref(var self, var val);
static var Box_Deref(var self);
static void Box_Assign(var self, var obj);

static void Box_New(var self, var args) {
  Box_Assign(self, get(args, $I(0)));
}

static void Box_Del(var self) {
  var obj = Box_Deref(self);
  if (obj) { del(obj); }
  Box_Ref(self, NULL);
}

static void Box_Assign(var self, var obj) {
  struct Pointer* p = instance(obj, Pointer);
  if (p and p->deref) {
    Box_Ref(self, p->deref(obj));
  } else {
    Box_Ref(self, obj);
  }
}

static int Box_Show(var self, var output, int pos) {
  return print_to(output, pos, "<'Box' at 0x%p (%$)>", self, Box_Deref(self));
}

static void Box_Ref(var self, var val) {
  struct Box* b = self;
  b->val = val;
}

static var Box_Deref(var self) {
  struct Box* b = self;
  return b->val;
}

var Box = Cello(Box,
  Instance(Doc,
    Box_Name, Box_Brief, Box_Description, Box_Definition, Box_Examples, NULL),
  Instance(New,      Box_New, Box_Del),
  Instance(Assign,   Box_Assign),
  Instance(Show,     Box_Show, NULL),
  Instance(Pointer,  Box_Ref, Box_Deref));
  
