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
  for (size_t i = 0; i < nargs; i++) {
    t->items[i] = get(args, $I(i));
  }
  t->items[nargs] = Terminal;
  
  return t;
}

static var Tuple_Del(var self) {
  struct Tuple* t = self;
  free(t->items);
  return self;
}

static size_t Tuple_Size(void) {
  return sizeof(struct Tuple);
}

static var Tuple_Assign(var self, var obj) {
  struct Tuple* t = self;
  size_t nargs = len(obj);
  t->items = realloc(t->items, sizeof(var) * (nargs+1));
  for (size_t i = 0; i < nargs; i++) {
    t->items[i] = get(obj, $I(i));
  }
  t->items[nargs-1] = Terminal;
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
  if (i < 0 or i >= len(self)) {
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
  if (i < 0 or i >= len(self)) {
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

static int Tuple_Show(var self, var output, int pos) {
  pos = print_to(output, pos, "<'Tuple' At 0x%p (", self);
  for(int i = 0; i < len(self); i++) {
    pos = print_to(output, pos, "%$", get(self, $(Int, i)));
    if (i < len(self)-1) { pos = print_to(output, pos, ", "); }
  }
  pos = print_to(output, pos, ")>");
  return pos;
}

var Tuple = Cello(Tuple,
  Member(Doc,
    Tuple_Name, Tuple_Brief, Tuple_Description, Tuple_Examples, Tuple_Methods),
  Member(New,    Tuple_New, Tuple_Del, Tuple_Size),
  Member(Assign, Tuple_Assign),
  Member(Copy,   Tuple_Copy),
  Member(Len,    Tuple_Len),
  Member(Get,    Tuple_Get, Tuple_Set, Tuple_Mem, NULL),
  Member(Iter,   Tuple_Iter_Init, Tuple_Iter_Next),
  Member(Show,   Tuple_Show, NULL));

