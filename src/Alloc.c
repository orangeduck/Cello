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

static const char* Size_Name(void) {
  return "Size";
}

/* TODO */
static const char* Size_Brief(void) {
  return "";
}

/* TODO */
static const char* Size_Description(void) {
  return "";
}

/* TODO */
static const char* Size_Examples(void) {
  return "";
}

/* TODO */
static const char* Size_Methods(void) {
  return "";
}

var Size = Cello(Size, Member(Doc, 
  Size_Name, Size_Brief, Size_Description, Size_Examples, Size_Methods));

size_t size(var type) {
  return type_method(type, Size, size);
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

var Alloc = Cello(Alloc, Member(Doc, 
  Alloc_Name, Alloc_Brief, Alloc_Description, Alloc_Examples, Alloc_Methods));


var alloc_stk(var type, var head, var data, size_t size) {
  CelloHeader_Init(head, type, CelloStackAlloc);
  memcpy((char*)head + sizeof(struct CelloHeader), data, size);
  return (char*)head + sizeof(struct CelloHeader);
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
  Member(Doc, New_Name, New_Brief, New_Description, New_Examples, New_Methods));

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
  
  return self;
}

void del(var self) {
  dealloc(destruct(self));
}
