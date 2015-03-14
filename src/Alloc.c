#include "Cello.h"

struct CelloHeader* Cello_GetHeader(var self) {
  return (struct CelloHeader*)((char*)self - sizeof(struct CelloHeader));
}

void CelloHeader_SetFlag(struct CelloHeader* head, int flag) {
  head->flags = (var)(intptr_t)(((int)(intptr_t)head->flags) | flag);
}

void CelloHeader_RemFlag(struct CelloHeader* head, int flag) {
  head->flags = (var)(intptr_t)(((int)(intptr_t)head->flags) & (~flag));
}

var CelloHeader_GetFlag(struct CelloHeader* head, int flag) {
  return (var)(intptr_t)(((int)(intptr_t)head->flags) & flag);
}

static var CelloObject_Marked(var self) {
  return CelloHeader_GetFlag(Cello_GetHeader(self), CelloMarked);
}

static void CelloObject_Mark(var self) {
  CelloHeader_SetFlag(Cello_GetHeader(self), CelloMarked);
}

static void CellObject_Unmark(var self) {
  CelloHeader_RemFlag(Cello_GetHeader(self), CelloMarked);
}

var CelloHeader_Init(struct CelloHeader* head, var type, int flags) {
  
  head->type = type;
  head->flags = (var)(intptr_t)flags;

#if CELLO_MAGIC == 1
  head->magic = (var)0xCe110;
#endif

  return ((char*)head) + sizeof(struct CelloHeader);
}

static const char* Alloc_Name(void) {
  return "Alloc";
}

/* TODO */
static const char* Alloc_Brief(void) {
  return "";
}

/* TODO */
static const char* Alloc_Description(void) {
  return "";
}

/* TODO */
static const char* Alloc_Examples(void) {
  return "";
}

/* TODO */
static const char* Alloc_Methods(void) {
  return "";
}

var Alloc = Cello(Alloc, Instance(Doc, 
  Alloc_Name, Alloc_Brief, Alloc_Description, Alloc_Examples, Alloc_Methods));


var alloc_stk(var type, var head, var data, size_t size) {
  CelloHeader_Init(head, type, CelloStackAlloc);
  memcpy((char*)head + sizeof(struct CelloHeader), data, size);
  var self = (char*)head + sizeof(struct CelloHeader);  
  return self;
}

var alloc(var type) {
  
  if (type_implements_method(type, Alloc, alloc)) {
    return type_method(type, Alloc, alloc);
  }
  
  struct CelloHeader* head = calloc(1,
    sizeof(struct CelloHeader) + size(type));
  
#if CELLO_MEMORY_CHECK == 1
  if (head is None) {
    throw(OutOfMemoryError, "Cannot create new '%s', out of memory!", type);
  }
#endif
  
  return CelloHeader_Init(head, type, CelloHeapAlloc);
}

void dealloc(var self) {

  if (implements_method(self, Alloc, dealloc)) {
    method(self, Alloc, dealloc);
    return;
  }

  struct CelloHeader* head = (struct CelloHeader*)(
    (char*)self - sizeof(struct CelloHeader));

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

static const char* New_Name(void) {
  return "New";
}

/* TODO */
static const char* New_Brief(void) {
  return "";
}

/* TODO */
static const char* New_Description(void) {
  return "";
}

/* TODO */
static const char* New_Examples(void) {
  return "";
}

/* TODO */
static const char* New_Methods(void) {
  return "";
}

var New = Cello(New,
  Instance(Doc, New_Name, New_Brief, New_Description, New_Examples, New_Methods));

var construct_with(var self, var args) {
  return method(self, New, construct_with, args);
}

var destruct(var self) {
  if (implements_method(self, New, destruct)) {
    return method(self, New, destruct);
  }
  return self;
}

var new_with(var type, var args) { 
  
  var self = alloc(type);
  
  if (type_implements_method(type, New, construct_with)) {
    self = type_method(type, New, construct_with, self, args);
  }
  
#if CELLO_GC == 1
  gc_add(self, CelloGCAlloc);
#endif
  
  return self;
}

var new_root_with(var type, var args) { 
  
  var self = alloc(type);
  
  if (type_implements_method(type, New, construct_with)) {
    self = type_method(type, New, construct_with, self, args);
  }
  
#if CELLO_GC == 1
  gc_add(self, CelloHeapAlloc);
#endif
  
  return self;
}

void del(var self) {
  //dealloc(destruct(self));

#if CELLO_GC == 1
  //gc_rem(self);
#endif
}


/* TODO */
static const char* Copy_Name(void) {
  return "Copy";
}

/* TODO */
static const char* Copy_Brief(void) {
  return "";
}

/* TODO */
static const char* Copy_Description(void) {
  return "";
}

/* TODO */
static const char* Copy_Examples(void) {
  return "";
}

/* TODO */
static const char* Copy_Methods(void) {
  return "";
}

var Copy = Cello(Copy,
  Instance(Doc,
    Copy_Name, Copy_Brief, Copy_Description, 
    Copy_Examples, Copy_Methods));

var copy(var self) {
  
  if (not implements(self, Copy)) {
    var obj = assign(alloc(type_of(self)), self);
    
#if CELLO_GC == 1
    gc_add(obj, CelloGCAlloc);
#endif
    
    return obj;
  } else {
    return method(self, Copy, copy);
  }
  
}

