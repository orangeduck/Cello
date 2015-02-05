#include "Cello.h"

#if CELLO_GC == 1

static var Cello_Heap_Root = NULL;
static var Cello_Stack_Root = NULL;

static var CelloObject_Prev_Get(var self) {
  struct CelloHeader* head = self - sizeof(struct CelloHeader);
  return head->prev;
}

static var CelloObject_Next_Get(var self) {
  struct CelloHeader* head = self - sizeof(struct CelloHeader);
  return head->next;
}

static void CelloObject_Prev_Set(var self, var obj) {
  struct CelloHeader* head = self - sizeof(struct CelloHeader);
  head->prev = obj;
}

static void CelloObject_Next_Set(var self, var obj) {
  struct CelloHeader* head = self - sizeof(struct CelloHeader);
  head->next = obj;
}

static void CelloObject_GCNext_Set(var self, var obj) {
  struct CelloHeader* head = self - sizeof(struct CelloHeader);
  head->gcnext = obj;
}

static var* CelloObject_GCNext_Ptr(var self) {
  struct CelloHeader* head = self - sizeof(struct CelloHeader);
  return &head->gcnext;
}

static void Cello_Stack_Filter(var sp) {
  var self = Cello_Stack_Root;
  while (self and self > sp) {
    self = CelloObject_Next_Get(self);
  }
  Cello_Stack_Root = self;
}

static void Cello_Stack_Push(var self) {
  
  //assert(sp > self);
  
  if (Cello_Stack_Root is None) {
    Cello_Stack_Root = self;
    return;
  }
  
  var object = Cello_Stack_Root;
  while (object and object > self) {
    object = CelloObject_Next_Get(object);
  }
  
  var prev = CelloObject_Prev_Get(object);
  CelloObject_Next_Set(prev, self);
  CelloObject_Prev_Set(self, prev);
  CelloObject_Next_Set(self, object);
  CelloObject_Prev_Set(object, self);
}

static void Cello_Heap_Push(var self) {
  CelloObject_Prev_Set(self, None);
  CelloObject_Next_Set(self, Cello_Heap_Root);
  CelloObject_Prev_Set(Cello_Heap_Root, self);
  Cello_Heap_Root = self;
}

static void Cello_Static_Push(var self) {
  CelloObject_Prev_Set(self, None);
  CelloObject_Next_Set(self, None);
}

#endif

var CelloHeader_Init(struct CelloHeader* head, var type, int flags) {
  
  var self = (var)head + sizeof(struct CelloHeader);
  head->type = type;
  head->flags = (var)(intptr_t)flags;
  
#if CELLO_GC == 1
  //Cello_Stack_Filter(sp); 
  //if (flags & CelloStackAlloc)  { Cello_Stack_Push(self); }
  //if (flags & CelloHeapAlloc)   { Cello_Heap_Push(self); }
  //if (flags & CelloStaticAlloc) { Cello_Static_Push(self); }
#endif

#if CELLO_MAGIC == 1
  head->magic = (var)0xCe110;
#endif

  return ((var)head) + sizeof(struct CelloHeader);
}

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

var New = Cello(New,
  Member(Doc, New_Name, New_Brief, New_Description, New_Examples, New_Methods));

var alloc_stk(var type, var head, var data, size_t size) {
  CelloHeader_Init(head, type, CelloStackAlloc);
  memcpy(head + sizeof(struct CelloHeader), data, size);
  return head + sizeof(struct CelloHeader);
}

size_t size(var type) {
  return type_method(type, New, size);
}

var alloc(var type) {
  
  size_t mem = size(type);
  if (mem is 0) { return None; }
  
  struct CelloHeader* head = calloc(1,
    sizeof(struct CelloHeader) + mem);
  
#if CELLO_MEMORY_CHECK == 1
  if (head is None) {
    throw(OutOfMemoryError, "Cannot create new '%s', out of memory!", type);
  }
#endif
  
  return CelloHeader_Init(head, type, CelloHeapAlloc);
}

void dealloc(var self) {

  struct CelloHeader* head = self - sizeof(struct CelloHeader);

#if CELLO_ALLOC_CHECK == 1
  if (self is  True) {
    throw(ResourceError, "Attempt to deallocate \"True\"/\"Okay\"!"); 
  }
  
  if (self is False) {
    throw(ResourceError, "Attempt to deallocate \"False\"/\"None\"/\"NULL\"!"); 
  }

  if (((int)(intptr_t)head->flags) & CelloStaticAlloc) {
    throw(ResourceError,
      "Attempt to deallocate %$ "
      "which was allocated statically!", self); 
  }
  
  if (((int)(intptr_t)head->flags) & CelloStackAlloc) {
    throw(ResourceError,
      "Attempt to deallocate %$ "
      "which was allocated on the stack!", self); 
  }
  
  if (((int)(intptr_t)head->flags) & CelloDataAlloc) {
    throw(ResourceError,
      "Attempt to deallocate %$ "
      "which was allocated inside a data structure!", self); 
  }
#endif
  
  free(head);
  
}

var new_with(var type, var args) { 
  
  var self = alloc(type);
  
  if (type_implements_method(type, New, construct_with)) {
    self = type_method(type, New, construct_with, self, args);
  }
  
  return self;
}

void del(var self) {
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

#if CELLO_GC == 1

bool CelloObject_Marked(var self) {
  struct CelloHeader* head = self - sizeof(struct CelloHeader);
  return ((int)(intptr_t)head->flags) & CelloMarked;
}

void CelloObject_Mark(var self) {
  struct CelloHeader* head = self - sizeof(struct CelloHeader);
  head->flags = (var)(intptr_t)(((int)(intptr_t)head->flags) & CelloMarked);
}

void CellObject_Unmark(var self) {
  struct CelloHeader* head = self - sizeof(struct CelloHeader);
  head->flags = (var)(intptr_t)(((int)(intptr_t)head->flags) & (~CelloMarked));
}

enum {
  Cello_GC_Initial = 10
};

static uint64_t Cello_GC_MaxItems = Cello_GC_Initial;
static uint64_t Cello_GC_NumItems = 0;
static float Cello_GC_Threshold = 1.5;
static var Cello_GC_Root = NULL;

static var Cello_GC_Traverse(var self) {
  if (CelloObject_Marked(self)) {
    return False;
  } else {
    CelloObject_Mark(self);
    return True;
  }
}

static void Cello_GC_Mark(void) {
  var object;
  object = Cello_Heap_Root;
  while (object isnt None) {
    //traverse(object, $(Function, Cello_GC_Traverse));
    object = CelloObject_Next_Get(object);
  }
  
  object = Cello_Stack_Root;
  while (object isnt None) {
    //traverse(object, $(Function, Cello_GC_Traverse));
    object = CelloObject_Next_Get(object);
  }
}

static void Cello_GC_Sweep(void) {
  var* object = &Cello_GC_Root;
  while (*object) {
    if (!CelloObject_Marked(*object)) {
      var unreached = *object;
      *object = *CelloObject_GCNext_Ptr(unreached);
      del(unreached);
      Cello_GC_NumItems--;
    } else {
      CellObject_Unmark(*object);
      object = CelloObject_GCNext_Ptr(*object);
    }
  }
}

static void Cello_GC_Run(void) {
  Cello_GC_Mark();
  Cello_GC_Sweep();
  Cello_GC_MaxItems = Cello_GC_NumItems > 0
    ? Cello_GC_NumItems * Cello_GC_Threshold
    : Cello_GC_MaxItems + Cello_GC_Initial;
}

var gc_new_with(var type, var args) {
  
  if (Cello_GC_NumItems >= Cello_GC_MaxItems) {
    Cello_GC_Run();
  }
  
  var self = new_with(type, args);
  
  CelloObject_GCNext_Set(self, Cello_GC_Root);
  Cello_GC_Root = self;
  Cello_GC_NumItems++;
  
  return self;
}

#endif


