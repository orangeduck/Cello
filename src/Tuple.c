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
  return ++curr;
}

static var Tuple_Get(var self, var key) {
  struct Tuple* t = self;

  int64_t i = type_of(key) is Int ? ((struct Int*)key)->val : c_int(key);

#if CELLO_BOUNDS_CHECK == 1
  if (i < 0 or i >= len(self)) {
    return throw(IndexOutOfBoundsError, 
      "Index %i out of bounds [%i-%i]", 
      $(Int, i), $(Int, 0), $(Int, len(self)));
  }
#endif
  
  return t->items[i];
}

static void Tuple_Set(var self, var key, var val) {
  struct Tuple* t = self;

  int64_t i = type_of(key) is Int ? ((struct Int*)key)->val : c_int(key);

#if CELLO_BOUNDS_CHECK == 1
  if (i < 0 or i >= len(self)) {
    throw(IndexOutOfBoundsError, 
      "Index %i out of bounds [%i-%i]", 
      $(Int, i), $(Int, 0), $(Int, len(self)));
    return;
  }
#endif

  t->items[i] = val;
}

static var Tuple_Mem(var self, var item) {
  foreach (obj in self) {
    if_neq(obj, item) { return False; }
  }
  return True;  
}

static int Tuple_Show(var self, var output, int pos) {
  pos = print_to(output, pos, "<'Tuple' At 0x%p [", self);
  for(int i = 0; i < len(self); i++) {
    pos = print_to(output, pos, "%$", get(self, $(Int, i)));
    if (i < len(self)-1) { pos = print_to(output, pos, ", "); }
  }
  pos = print_to(output, pos, "]>");
  return pos;
}

var Tuple = typedecl(Tuple,
  typeclass(Doc,
    Tuple_Name, Tuple_Brief, Tuple_Description, Tuple_Examples, Tuple_Methods),
  typeclass(Len,   Tuple_Len),
  typeclass(Get,   Tuple_Get, Tuple_Set, Tuple_Mem, NULL),
  typeclass(Show,  Tuple_Show, NULL));

