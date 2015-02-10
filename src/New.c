#include "Cello.h"

#if CELLO_GC == 1

static var Cello_Heap_Root = NULL;
static var Cello_Stack_Root = NULL;

static var* CelloObject_Prev(var self) {
  struct CelloHeader* head = self - sizeof(struct CelloHeader);
  return &head->prev;
}

static var* CelloObject_Next(var self) {
  struct CelloHeader* head = self - sizeof(struct CelloHeader);
  return &head->next;
}

static void Cello_Stack_Filter(var sp) {
  var self = Cello_Stack_Root;
  while (self and self > sp) {
    self = *CelloObject_Next(self);
  }
  Cello_Stack_Root = self;
}

static void Cello_Stack_Push(var self) {
  
  if (Cello_Stack_Root is None) {
    Cello_Stack_Root = self;
    *CelloObject_Next(self) = Terminal;
    *CelloObject_Prev(self) = Terminal;
    return;
  }
  
  var item = Cello_Stack_Root;
  while (item and item > self) {
    item = *CelloObject_Next(item);
  }
  
  var prev = *CelloObject_Prev(item);
  *CelloObject_Next(prev) = self;
  *CelloObject_Prev(self) = prev;
  *CelloObject_Next(self) = item;
  *CelloObject_Prev(item) = self;
}

static void Cello_Heap_Push(var self) {
  *CelloObject_Prev(self) = Terminal;
  *CelloObject_Next(self) = Cello_Heap_Root;
  *CelloObject_Prev(Cello_Heap_Root) = self;
  Cello_Heap_Root = self;
}

#endif

void CelloHeader_SetFlag(struct CelloHeader* head, int flag) {
  head->flags = (var)(intptr_t)(((int)(intptr_t)head->flags) | flag);
}

void CelloHeader_RemFlag(struct CelloHeader* head, int flag) {
  head->flags = (var)(intptr_t)(((int)(intptr_t)head->flags) & (~flag));
}

var CelloHeader_GetFlag(struct CelloHeader* head, int flag) {
  return (var)(intptr_t)(((int)(intptr_t)head->flags) & flag);
}

var CelloHeader_Init(struct CelloHeader* head, var type, int flags) {
  
  var sp = None;
  var self = ((var)head) + sizeof(struct CelloHeader);
  head->type = type;
  head->flags = (var)(intptr_t)flags;
  
#if CELLO_GC == 1
  Cello_Stack_Filter(&sp); 
  if (flags & CelloStackAlloc) { Cello_Stack_Push(self); }
  if (flags & CelloHeapAlloc)  { Cello_Heap_Push(self); }
#endif

#if CELLO_MAGIC == 1
  head->magic = (var)0xCe110;
#endif

  return self;
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
  
  /* TODO: Remove from GC link lists (remove from GC main list if GC alloc) */
  
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
  return CelloHeader_GetFlag(self - sizeof(struct CelloHeader), CelloMarked);
}

void CelloObject_Mark(var self) {
  CelloHeader_SetFlag(self - sizeof(struct CelloHeader), CelloMarked);
}

void CellObject_Unmark(var self) {
  CelloHeader_RemFlag(self - sizeof(struct CelloHeader), CelloMarked);
}

enum {
  Cello_GC_Initial = 10
};

/* TODO: Make Thread Local */
static uint64_t Cello_GC_MaxItems = Cello_GC_Initial;
static uint64_t Cello_GC_NumItems = 0;
static uint64_t Cello_GC_NumSlots = 0;
static float Cello_GC_Threshold = 1.5;
static var* Cello_GC_Array = NULL;

static void Cello_GC_Reserve_More(void) {
  if (Cello_GC_NumItems > Cello_GC_NumSlots) {
    Cello_GC_NumSlots = Cello_GC_NumItems + Cello_GC_NumItems / 2;
    Cello_GC_Array = realloc(Cello_GC_Array, sizeof(var) * Cello_GC_NumSlots);
#if CELLO_MEMORY_CHECK == 1
    if (Cello_GC_Array is None) {
      throw(OutOfMemoryError, "Cannot allocate more objects, out of memory!");
    }
#endif
  }
}

static void Cello_GC_Reserve_Less(void) {
  if (Cello_GC_NumSlots > Cello_GC_NumItems + Cello_GC_NumItems / 2) {
    Cello_GC_NumSlots = Cello_GC_NumItems;
    Cello_GC_Array= realloc(Cello_GC_Array, sizeof(var) * Cello_GC_NumSlots);
  }
}

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
  while (object isnt Terminal) {
    //traverse(object, $(Function, Cello_GC_Traverse));
    object = *CelloObject_Next(object);
  }
  
  object = Cello_Stack_Root;
  while (object isnt Terminal) {
    //traverse(object, $(Function, Cello_GC_Traverse));
    object = *CelloObject_Next(object);
  }
}

static void Cello_GC_Sweep(void) {
  
  size_t i = 0;
  while (i < Cello_GC_NumItems) {
    
    if (CelloObject_Marked(Cello_GC_Array[i])) {
      CellObject_Unmark(Cello_GC_Array[i]);
      i++;
      continue;
    }
    
    if (!CelloObject_Marked(Cello_GC_Array[i])) {
      del(Cello_GC_Array[i]);
      Cello_GC_Array[i] = Cello_GC_Array[Cello_GC_NumItems-1];
      Cello_GC_NumItems--;
      Cello_GC_Reserve_Less();
      continue;
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

/*
var auto_with(var type, var args) {
  
  if (Cello_GC_NumItems >= Cello_GC_MaxItems) {
    Cello_GC_Run();
  }
  
  var self;
  size_t mem = size(type);
  
  if (mem is 0) {
    self = None;
  } else {
  
    struct CelloHeader* head = calloc(1,
      sizeof(struct CelloHeader) + mem);
    
#if CELLO_MEMORY_CHECK == 1
    if (head is None) {
      throw(OutOfMemoryError, "Cannot create new '%s', out of memory!", type);
    }
#endif
    
    self = CelloHeader_Init(head, type, CelloGCAlloc);
  }
  
  if (type_implements_method(type, New, construct_with)) {
    self = type_method(type, New, construct_with, self, args);
  }
  
  Cello_GC_NumItems++;
  Cello_GC_Reserve_More();
  Cello_GC_Array[Cello_GC_NumItems-1] = self;
  
  return self;
}
*/

#endif


