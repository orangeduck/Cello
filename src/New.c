#include "Cello.h"

static const char* New_Name(void) {
  return "New";
}

static const char* New_Brief(void) {
  return "Constructable on the Heap";
}

static const char* New_Description(void) {
  return "";
    "The `New` class provides a method to implement dynamic (heap) memory "
    "allocation for certain object types as well as _constructor_ and "
    "_destructor_ functions to be called just after an object's memory space "
    "has been allocated and just before it's memory allocation is freed. "
    "To implement this class you must also give a function returning the "
    "memory size of the data object to allocated."
    "\n\n"
    "The `new` function takes a list of `var` type arguments to provide a "
    "method of passing arguments to an object's constructor. native C types "
    "should never be passed into this list and should instead be wrapped "
    "using `$`. See examples for details.";
}

static const char* New_Examples(void) {
  return ""
    "__Usage__\n"
    "    \n"
    "    var x = new(Int, $(Int, 1));\n"
    "    \n"
    "    show(x); /* 1 */\n"
    "    show(type_of(x)) /* Int */\n"
    "    \n"
    "    del(x);\n"
    "    \n"
    "    var y = $(Real, 0.0);\n"  
    "    \n"
    "    show(y); /* 0.0 */\n"
    "    construct(y, $(Int, 1.0));\n"
    "    show(y); /* 1.0 */\n"
    "    \n"
    "    var z = alloc(String);\n"
    "    construct(z, $(String, \"Hello\"));\n"
    "    \n"
    "    show(z); /* Hello */\n"
    "    z = destruct(z);\n"
    "    dealloc(z);\n";
}

/* TODO */
static const char* New_Methods(void) {
  return "";
}

var New = typedecl(New,
  typeclass(Doc,
    New_Name, New_Brief, New_Description, New_Examples, New_Methods));
  
var new_stk(var type, var mem, var data, size_t size) {
  struct CelloHeader* head = mem;
  head->type = type;
  head->flags = (var)CelloStackAlloc;
  memcpy(mem + sizeof(struct CelloHeader), data, size);
  return mem + sizeof(struct CelloHeader);
}

size_t size(var type) {
  return type_method(type, New, size);
}

var alloc(var type) {
  
  size_t mem = size(type);
  if (mem is 0) { return None; }
  
  struct CelloHeader* head = calloc(1,
    sizeof(struct CelloHeader) + mem);
  
  if (head is None) {
    throw(OutOfMemoryError, "Cannot create new '%s', out of memory!", type);
  }
  
  head->type = type;
  head->flags = (var)CelloHeapAlloc;
  
  return ((var)head) + sizeof(struct CelloHeader);
}

void dealloc(var self) {
  struct CelloHeader* head = self - sizeof(struct CelloHeader);
  if (((int)(intptr_t)head->flags) & CelloHeapAlloc) { free(head); }
}

var new_with(var type, var args) { 
  
  var self = alloc(type);
  
  if (type_implements_method(type, New, construct_with)) {
    self = type_method(type, New, construct_with, self, args);
  }
  
  return self;
}

void del(var self) {
  if (self is      True) { return; }
  if (self is     False) { return; }
  dealloc(destruct(self));
}

var construct_with(var self, var args) {
  return method(self, New, construct_with, args);
}

var destruct(var self) {
  if (implements_method(self, New, destruct)) {
    return method(self, New, destruct);
  }
  return self;
}
