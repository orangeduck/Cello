#include "Cello.h"

struct Header* header(var self) {
  return (struct Header*)((char*)self - sizeof(struct Header));
}

var header_init(var head, var type, int alloc) {
  
  struct Header* self = head;
  
  self->type = type;
  
#if CELLO_ALLOC_CHECK == 1
  self->alloc = (var)(intptr_t)alloc;
#endif
  
#if CELLO_MAGIC_CHECK == 1
  self->magic = (var)CELLO_MAGIC_NUM;
#endif

  return ((char*)self) + sizeof(struct Header);
}

static const char* Alloc_Name(void) {
  return "Alloc";
}

static const char* Alloc_Brief(void) {
  return "Memory Allocation";
}

static const char* Alloc_Description(void) {
  return
    "The `Alloc` class can be used to override how memory is allocated for a "
    "given data type. By default memory is allocated using `calloc` along with "
    "the `Size` class to determine the amount of memory to allocate."
    "\n\n"
    "A custom allocator should be careful to also initialise the header for "
    "the allocated memory using the function `header_init`. Cello objects "
    "without a header wont be recognised as such as so will throw errors when "
    "used with Cello functions."
    "\n\n"
    "Allocated memory is automatically registered with the garbage collector "
    "unless the functions `alloc_raw` and `dealloc_raw` are used."
  ;
}

static const char* Alloc_Definition(void) {
  return
    "struct Alloc {\n"
    "  var (*alloc)(void);\n"
    "  void (*dealloc)(var);\n"
    "};";
}

static struct Example* Alloc_Examples(void) {
  
  static struct Example examples[] = {
    {
     "Usage",
     "/* Allocation deallocated by Garbage Collector */\n"
     "var x = alloc(Int);\n"
     "construct(x, $I(10));\n",
    }, {
     "Avoid Garbage Collection",
     "/* Allocation must be manually deallocated */\n"
     "var x = alloc_raw(Int);\n"
     "construct(x, $I(10));\n"
     "destruct(x);\n"
     "dealloc_raw(x);\n",
    }, {NULL, NULL}
  };
  
  return examples;
}

static struct Method* Alloc_Methods(void) {
  
  static struct Method methods[] = {
    {
      "$",
      "#define $(T, ...)\n"
      "#define $I(X)\n"
      "#define $F(X)\n"
      "#define $S(X)\n"
      "#define $R(X)\n"
      "#define $B(X)",
      "Allocate memory for the given type `T` on the stack and copy in the "
      "given arguments `...` as struct members. Shorthand constructors exist "
      "for native types:\n\n* `$I -> Int` `$F -> Float` `$S -> String`\n*"
      " `$R -> Ref` `$B -> Box`\n\n"
    }, {
      "alloc", 
      "#define alloc_stack(T)\n"
      "var alloc(var type);\n"
      "var alloc_raw(var type);\n"
      "var alloc_root(var type);",
      "Allocate memory for a given `type`. To avoid the Garbage Collector "
      "completely use `alloc_raw`, to register the allocation as a root use "
      "`alloc_root`. In the case of raw or root allocations the corresponding "
      "`dealloc` function should be used when done. Memory allocated with "
      "`alloc_stack` is not managed by the Garbage Collector."
    }, {
      "dealloc",
      "void dealloc(var self);\n"
      "void dealloc_raw(var self);\n"
      "void dealloc_root(var self);",
      "Deallocate memory for object `self` manually. If registered with the "
      "Garbage Collector then entry will be removed. If the `raw` variation is "
      "used memory will be deallocated without going via the Garbage Collector." 
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Alloc = Cello(Alloc, Instance(Doc, 
  Alloc_Name,       Alloc_Brief,    Alloc_Description, 
  Alloc_Definition, Alloc_Examples, Alloc_Methods));

enum {
  ALLOC_STANDARD,
  ALLOC_RAW,
  ALLOC_ROOT
};
  
static var alloc_by(var type, int method) {
  
  struct Alloc* a = type_instance(type, Alloc);
  var self;
  if (a and a->alloc) {
    self = a->alloc();
  } else {
    struct Header* head = calloc(1, sizeof(struct Header) + size(type));  

#if CELLO_MEMORY_CHECK == 1
    if (head is NULL) {
      throw(OutOfMemoryError, "Cannot create new '%s', out of memory!", type);
    }
#endif
    
    self = header_init(head, type, AllocHeap);
  }

  switch (method) {
    case ALLOC_STANDARD:
#ifndef CELLO_NGC
    set(current(GC), self, $I(0));
#endif
    break;
    case ALLOC_RAW: break;
    case ALLOC_ROOT:
#ifndef CELLO_NGC
    set(current(GC), self, $I(1));
#endif
    break;
  }
  
  return self;
}

var alloc(var type)      { return alloc_by(type, ALLOC_STANDARD); }
var alloc_raw(var type)  { return alloc_by(type, ALLOC_RAW); }
var alloc_root(var type) { return alloc_by(type, ALLOC_ROOT); }

void dealloc(var self) {

  struct Alloc* a = instance(self, Alloc);
  if (a and a->dealloc) {
    a->dealloc(self);
    return;
  }

#if CELLO_ALLOC_CHECK == 1
  if (self is NULL) {
    throw(ResourceError, "Attempt to deallocate NULL!"); 
  }

  if (header(self)->alloc is (var)AllocStatic) {
    throw(ResourceError,
      "Attempt to deallocate %$ "
      "which was allocated statically!", self); 
  }
  
  if (header(self)->alloc is (var)AllocStack) {
    throw(ResourceError,
      "Attempt to deallocate %$ "
      "which was allocated on the stack!", self); 
  }
  
  if (header(self)->alloc is (var)AllocData) {
    throw(ResourceError,
      "Attempt to deallocate %$ "
      "which was allocated inside a data structure!", self); 
  }
#endif
  
#if CELLO_ALLOC_CHECK == 1
  size_t s = size(type_of(self));
  for (size_t i = 0; i < (sizeof(struct Header) + s) / sizeof(var); i++) {
    ((var*)header(self))[i] = (var)0xDeadCe110;
  }
#endif
  
  free(((char*)self) - sizeof(struct Header));
  
}

void dealloc_raw(var self)  { dealloc(self); }
void dealloc_root(var self) { dealloc(self); }

static const char* New_Name(void) {
  return "New";
}

static const char* New_Brief(void) {
  return "Construction and Destruction";
}

static const char* New_Description(void) {
  return
    "The `New` class allows the user to define constructors and destructors "
    "for a type, accessible via `new` and `del`. Objects allocated with `new` "
    "are allocated on the heap and also registered with the Garbage Collector "
    "this means technically it isn't required to call `del` on them as they "
    "will be cleaned up at a later date."
    "\n\n"
    "The `new_root` function can be called to register a variable with the "
    "Garbage Collector but to indicate that it will be manually destructed "
    "with `del_root` by the user. This should be used for variables that wont "
    "be reachable by the Garbage Collector such as those in the data segment "
    "or only accessible via vanilla C structures."
    "\n\n"
    "The `new_raw` and `del_raw` functions can be called to construct and "
    "destruct objects without going via the Garbage Collector."
    "\n\n"
    "It is also possible to simply call the `construct` and `destruct` "
    "functions if you wish to construct an already allocated object."
    "\n\n"
    "Constructors should assume that memory is zero'd for an object but "
    "nothing else."
  ;
}

static const char* New_Definition(void) {
  return
    "struct New {\n"
    "  void (*construct_with)(var, var);\n"
    "  void (*destruct)(var);\n"
    "};\n";
}


static struct Example* New_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var x = new(Int, $I(1));\n"
      "show(x); /* 1 */\n"
      "show(type_of(x)); /* Int */\n"
      "\n"
      "var y = alloc(Float);\n"
      "construct(y, $F(1.0));\n"
      "show(y); /* 1.0 */\n"
      "destruct(y);\n"
    }, {NULL, NULL}
  };
  
  return examples;
}

static struct Method* New_Methods(void) {
  
  static struct Method methods[] = {
    {
      "new", 
      "#define new(T, ...)\n"
      "#define new_raw(T, ...)\n"
      "#define new_root(T, ...)\n"
      "var new_with(var type, var args);\n"
      "var new_raw_with(var type, var args);\n"
      "var new_root_with(var type, var args);",
      "Construct a new object of a given `type`. Use `new_raw` to avoid the "
      "Garbage Collector completely, and `new_root` to register the allocation "
      "as a Garbage Collection root. In the case of raw and root allocations "
      "they must be destructed with the corresponding deletion functions."
    }, {
      "del",
      "void del(var self);\n"
      "void del_raw(var self);\n"
      "void del_root(var self);",
      "Destruct the object `self` manually. If registered with the "
      "Garbage Collector then entry will be removed. If `del_raw` is used then"
      "the destruction will be done without going via the Garbage Collector."
    }, {
      "construct",
      "#define construct(self, ...)\n"
      "var construct_with(var self, var args);",
      "Call the constructor on object `self` which has already been allocated."
    }, {
      "destruct",
      "var destruct(var self);",
      "Call the destructor on object `self` without deallocating the memory "
      "for it."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var New = Cello(New,
  Instance(Doc,
    New_Name,       New_Brief,    New_Description, 
    New_Definition, New_Examples, New_Methods));

var construct_with(var self, var args) {
  struct New* n = instance(self, New);
  if (n and n->construct_with) {
    n->construct_with(self, args);
  } else if (len(args) == 1) {
    assign(self, get(args, $I(0)));
  }
  return self;
}

var destruct(var self) {
  struct New* n = instance(self, New);
  if (n and n->destruct) {
    n->destruct(self);
  }
  return self;
}

var new_with(var type, var args) {
  return construct_with(alloc(type), args);
}

var new_raw_with(var type, var args) { 
  return construct_with(alloc_raw(type), args);
}

var new_root_with(var type, var args) { 
  return construct_with(alloc_root(type), args);
}

static void del_by(var self, int method) {
  
  switch (method) {
    case ALLOC_STANDARD:
    case ALLOC_ROOT:
#ifndef CELLO_NGC
    rem(current(GC), self);
    return;
#endif
    break;
    case ALLOC_RAW: break;
  }
  
  dealloc(destruct(self));
  
}

void del(var self) { del_by(self, ALLOC_STANDARD); }
void del_raw(var self) { del_by(self, ALLOC_RAW); }
void del_root(var self) { del_by(self, ALLOC_ROOT); }

static const char* Copy_Name(void) {
  return "Copy";
}

static const char* Copy_Brief(void) {
  return "Copyable";
}

static const char* Copy_Description(void) {
  return
    "The `Copy` class can be used to override the behaviour of an object when "
    "a copy is made of it. By default the `Copy` class allocates a new empty "
    "object of the same type and uses the `Assign` class to set the "
    "contents. The copy is then registered with the Garbage Collector as if it "
    "had been constructed with `new`. This means when using manual memory "
    "management a copy must be deleted manually."
    "\n\n"
    "If the `copy` class is overridden then the implementer may manually have "
    "to register the object with the Garbage Collector if they wish for it to "
    "be tracked."
    "\n\n"
    "By convention `copy` follows the semantics of `Assign`, which typically "
    "means a _deep copy_ should be made, and that an object will create a "
    "copy of all of the sub-objects it references or contains - although this "
    "could vary depending on the type's overridden behaviours.";
}

static const char* Copy_Definition(void) {
  return
    "struct Copy {\n"
    "  var (*copy)(var);\n"
    "};\n";
}

static struct Example* Copy_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var x = new(String, $S(\"Hello\"));\n"
      "var y = copy(x);\n"
      "show(x); /* Hello */\n"
      "show(y); /* Hello */\n"
      "show($I(eq(x, y))); /* 1 */\n"
      "show($I(x is y)); /* 0 */\n"
    }, {NULL, NULL}
  };
  
  return examples;
}

static struct Method* Copy_Methods(void) {
  
  static struct Method methods[] = {
    {
      "copy", 
      "var copy(var self);",
      "Make a copy of the object `self`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Copy = Cello(Copy,
  Instance(Doc,
    Copy_Name,       Copy_Brief,    Copy_Description, 
    Copy_Definition, Copy_Examples, Copy_Methods));

var copy(var self) {
  
  struct Copy* c = instance(self, Copy);
  if (c and c->copy) {
    return c->copy(self); 
  }
  
  return assign(alloc(type_of(self)), self);
  
}
