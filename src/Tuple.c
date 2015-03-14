#include "Cello.h"

static const char* Tuple_Name(void) {
  return "Tuple";
}

/* TODO */
static const char* Tuple_Brief(void) {
  return "";
}

/* TODO */
static const char* Tuple_Description(void) {
  return "";
}

/* TODO */
static const char* Tuple_Examples(void) {
  return "";
}

/* TODO */
static const char* Tuple_Methods(void) {
  return "";
}

static var Tuple_New(var self, var args) {
  struct Tuple* t = self;
  size_t nargs = len(args);
  
  t->items = malloc(sizeof(var) * (nargs+1));
  
#if CELLO_MEMORY_CHECK == 1
  if (t->items is None) {
    throw(OutOfMemoryError, "Cannot create Tuple, out of memory!");
  }
#endif
  
  for (size_t i = 0; i < nargs; i++) {
    t->items[i] = get(args, $I(i));
  }
  t->items[nargs] = Terminal;
  
  return t;
}

static var Tuple_Del(var self) {
  struct Tuple* t = self;
  
#if CELLO_ALLOC_CHECK == 1
  if (CelloHeader_GetFlag(Cello_GetHeader(self), CelloStackAlloc)
  or  CelloHeader_GetFlag(Cello_GetHeader(self), CelloStaticAlloc)) {
    throw(ValueError, "Cannot free Tuple, not on heap!");
  }
#endif
  
  free(t->items);
  return self;
}

static var Tuple_Assign(var self, var obj) {
  struct Tuple* t = self;
  size_t nargs = len(obj);
  
#if CELLO_ALLOC_CHECK == 1
  if (CelloHeader_GetFlag(Cello_GetHeader(self), CelloStackAlloc)
  or  CelloHeader_GetFlag(Cello_GetHeader(self), CelloStaticAlloc)) {
    throw(ValueError, "Cannot reallocate Tuple, not on heap!");
  }
#endif
  
  t->items = realloc(t->items, sizeof(var) * (nargs+1));
  
#if CELLO_MEMORY_CHECK == 1
  if (t->items is None) {
    throw(OutOfMemoryError, "Cannot allocate Tuple, out of memory!");
  }
#endif
  
  for (size_t i = 0; i < nargs; i++) {
    t->items[i] = get(obj, $I(i));
  }
  t->items[nargs] = Terminal;
  
  return t;
}

static var Tuple_Copy(var self) {
  return new_with(Tuple, self);
}

static size_t Tuple_Len(var self) {
  struct Tuple* t = self;
  size_t x = 0;
  size_t i = 0;
  while (t->items[i] isnt Terminal) { i++; }
  return i;
}

static var Tuple_Iter_Init(var self) {
  struct Tuple* t = self;
  return t->items[0];
}

static var Tuple_Iter_Next(var self, var curr) {
  struct Tuple* t = self;
  size_t i = 0;
  while (t->items[i] isnt Terminal) {
    if (t->items[i] is curr) { return t->items[i+1]; }
    i++;
  }
  return Terminal;
}

static var Tuple_Get(var self, var key) {
  struct Tuple* t = self;

  int64_t i = type_of(key) is Int ? ((struct Int*)key)->val : c_int(key);

#if CELLO_BOUND_CHECK == 1
  if (i < 0 or i >= (int64_t)len(self)) {
    return throw(IndexOutOfBoundsError, 
      "Index '%i' out of bounds for Tuple of size %i.", 
      key, $(Int, Tuple_Len(t)));
  }
#endif
  
  return t->items[i];
}

static void Tuple_Set(var self, var key, var val) {
  struct Tuple* t = self;

  int64_t i = type_of(key) is Int ? ((struct Int*)key)->val : c_int(key);

#if CELLO_BOUND_CHECK == 1
  if (i < 0 or i >= (int64_t)len(self)) {
    throw(IndexOutOfBoundsError, 
      "Index '%i' out of bounds for Tuple of size %i.", 
      key, $(Int, Tuple_Len(t)));
    return;
  }
#endif

  t->items[i] = val;
}

static var Tuple_Mem(var self, var item) {
  foreach (obj in self) {
    if_neq (obj, item) { return False; }
  }
  return True;  
}

static void Tuple_Pop_At(var self, var key);

static void Tuple_Rem(var self, var item) {
  size_t i = 0;
  foreach (obj in self) {
    if_eq (obj, item) {
      Tuple_Pop_At(self, $I(i)); break;
    }
    i++;
  }
}

static int Tuple_Show(var self, var output, int pos) {
  pos = print_to(output, pos, "<'Tuple' At 0x%p (", self);
  for(size_t i = 0; i < len(self); i++) {
    pos = print_to(output, pos, "%$", get(self, $I(i)));
    if (i < len(self)-1) { pos = print_to(output, pos, ", "); }
  }
  pos = print_to(output, pos, ")>");
  return pos;
}

static void Tuple_Push(var self, var obj) {
  
  struct Tuple* t = self;
  size_t nitems = Tuple_Len(t);
  
#if CELLO_ALLOC_CHECK == 1
  if (CelloHeader_GetFlag(Cello_GetHeader(self), CelloStackAlloc)
  or  CelloHeader_GetFlag(Cello_GetHeader(self), CelloStaticAlloc)) {
    throw(ValueError, "Cannot reallocate Tuple, not on heap!");
  }
#endif
  
  t->items = realloc(t->items, sizeof(var) * (nitems+2));
  
#if CELLO_MEMORY_CHECK == 1
  if (t->items is None) {
    throw(OutOfMemoryError, "Cannot grow Tuple, out of memory!");
  }
#endif
  
  t->items[nitems+0] = obj;
  t->items[nitems+1] = Terminal;
  
}

static void Tuple_Pop(var self) {
  
  struct Tuple* t = self;
  size_t nitems = Tuple_Len(t);
  
#if CELLO_ALLOC_CHECK == 1
  if (CelloHeader_GetFlag(Cello_GetHeader(self), CelloStackAlloc)
  or  CelloHeader_GetFlag(Cello_GetHeader(self), CelloStaticAlloc)) {
    throw(ValueError, "Cannot reallocate Tuple, not on heap!");
  }
#endif
  
  t->items = realloc(t->items, sizeof(var) * nitems);
  t->items[nitems-1] = Terminal;
  
}

static void Tuple_Push_At(var self, var key, var obj) {
  
  struct Tuple* t = self;

  size_t nitems = Tuple_Len(t);
  
#if CELLO_ALLOC_CHECK == 1
  if (CelloHeader_GetFlag(Cello_GetHeader(self), CelloStackAlloc)
  or  CelloHeader_GetFlag(Cello_GetHeader(self), CelloStaticAlloc)) {
    throw(ValueError, "Cannot reallocate Tuple, not on heap!");
  }
#endif
  
  t->items = realloc(t->items, sizeof(var) * (nitems+2));
  
#if CELLO_MEMORY_CHECK == 1
  if (t->items is None) {
    throw(OutOfMemoryError, "Cannot grow Tuple, out of memory!");
  }
#endif
  
  int64_t i = type_of(key) is Int ? ((struct Int*)key)->val : c_int(key);

#if CELLO_BOUND_CHECK == 1
  if (i < 0 or i >= (int64_t)nitems) {
    throw(IndexOutOfBoundsError,
      "Index '%i' out of bounds for Tuple of size %i.", key, $(Int, nitems));
  }
#endif

  memmove(t->items + sizeof(var) * (i+1), 
          t->items + sizeof(var) * (i+0), 
          sizeof(var) * (nitems - (size_t)i));
  
  t->items[i] = obj;
  
}

static void Tuple_Pop_At(var self, var key) {

  struct Tuple* t = self;
  size_t nitems = Tuple_Len(t);

  int64_t i = type_of(key) is Int ? ((struct Int*)key)->val : c_int(key);
  
#if CELLO_BOUND_CHECK == 1
  if (i < 0 or i >= (int64_t)nitems) {
    throw(IndexOutOfBoundsError,
      "Index '%i' out of bounds for Tuple of size %i.", key, $(Int, nitems));
  }
#endif
  
  memmove(t->items + sizeof(var) * (i+0), 
          t->items + sizeof(var) * (i+1), 
          sizeof(var) * (nitems - (size_t)i));
  
#if CELLO_ALLOC_CHECK == 1
  if (CelloHeader_GetFlag(Cello_GetHeader(self), CelloStackAlloc)
  or  CelloHeader_GetFlag(Cello_GetHeader(self), CelloStaticAlloc)) {
    throw(ValueError, "Cannot reallocate Tuple, not on heap!");
  }
#endif
  
  t->items = realloc(t->items, sizeof(var) * nitems);
  
}

static void Tuple_Concat(var self, var obj) {

  struct Tuple* t = self;
  size_t nitems = Tuple_Len(t);
  size_t objlen = len(obj);
  
#if CELLO_ALLOC_CHECK == 1
  if (CelloHeader_GetFlag(Cello_GetHeader(self), CelloStackAlloc)
  or  CelloHeader_GetFlag(Cello_GetHeader(self), CelloStaticAlloc)) {
    throw(ValueError, "Cannot reallocate Tuple, not on heap!");
  }
#endif
  
  t->items = realloc(t->items, sizeof(var) * (nitems+1+objlen));
  
#if CELLO_MEMORY_CHECK == 1
  if (t->items is None) {
    throw(OutOfMemoryError, "Cannot grow Tuple, out of memory!");
  }
#endif
  
  size_t i = nitems;
  foreach (item in obj) {
    t->items[i] = item;
    i++;
  }
  
  t->items[nitems+objlen] = Terminal;
  
}

static void Tuple_Clear(var self) {
  struct Tuple* t = self;
  
#if CELLO_ALLOC_CHECK == 1
  if (CelloHeader_GetFlag(Cello_GetHeader(self), CelloStackAlloc)
  or  CelloHeader_GetFlag(Cello_GetHeader(self), CelloStaticAlloc)) {
    throw(ValueError, "Cannot reallocate Tuple, not on heap!");
  }
#endif
  
  t->items = realloc(t->items, sizeof(var));
  t->items[0] = Terminal;
}

static void Tuple_Traverse(var self, var func) {
  struct Tuple* t = self;
  size_t i = 0;
  while (t->items[i] isnt Terminal) {
    call_with(func, t->items[i]); i++;
  }
}

var Tuple = Cello(Tuple,
  Instance(Doc,
    Tuple_Name, Tuple_Brief, Tuple_Description, Tuple_Examples, Tuple_Methods),
  Instance(New,      Tuple_New, Tuple_Del),
  Instance(Assign,   Tuple_Assign),
  Instance(Copy,     Tuple_Copy),
  Instance(Len,      Tuple_Len),
  Instance(Get,      Tuple_Get, Tuple_Set, Tuple_Mem, Tuple_Rem),
  Instance(Push,     Tuple_Push, Tuple_Pop, Tuple_Push_At, Tuple_Pop_At),
  Instance(Concat,   Tuple_Concat, Tuple_Push),
  Instance(Clear,    Tuple_Clear),
  Instance(Iter,     Tuple_Iter_Init, Tuple_Iter_Next),
  Instance(Traverse, Tuple_Traverse),
  Instance(Show,     Tuple_Show, NULL));

