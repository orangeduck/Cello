#include "Cello.h"

static const char* Array_Name(void) {
  return "Array";
}

static const char* Array_Brief(void) {
  return "Sequential Container";
}

static const char* Array_Description(void) {
  return ""
    "Array is data structure containing a sequence of a single type of object. "
    "It can dynamically grow and shrink in size depending on how many elements "
    "it contains. It allocates storage for the type specified. It also "
    "deallocates and destroys the objects inside upon destruction."
    "\n\n"
    "Elements are copied into an Array using `assign` and will initially have "
    "zero'd memory."
    "\n\n"
    "Elements are ordered linearly. Elements are accessed by their position in "
    "this sequence directly. Addition and removal of elements at the end of "
    "the sequence is fast, with memory movement required for elements in the "
    "middle of the sequence." 
    "\n\n"
    "This is largely equivalent to the C++ construct "
    "[std::vector](http://www.cplusplus.com/reference/vector/vector/)";
}

/* TODO: Examples */

struct Array {
  var type;
  var data;
  size_t tsize;
  size_t nitems;
  size_t nslots;
  var sspace0;
  var sspace1;
};

static size_t Array_Step(struct Array* a) {
  return a->tsize + sizeof(struct Header);
}

static var Array_Item(struct Array* a, size_t i) {
  return (char*)a->data + Array_Step(a) * i + sizeof(struct Header);
}

static void Array_Alloc(struct Array* a, size_t i) {
  memset((char*)a->data + Array_Step(a) * i, 0, Array_Step(a));
  struct Header* head = (struct Header*)((char*)a->data + Array_Step(a) * i);
  header_init(head, a->type, AllocData);
}

static size_t Array_Size_Round(size_t s) {
  return ((s + sizeof(var) - 1) / sizeof(var)) * sizeof(var);
}

static void Array_New(var self, var args) {
  
  struct Array* a = self;
  a->type   = cast(get(args, $I(0)), Type);
  a->tsize  = Array_Size_Round(size(a->type));
  a->nitems = len(args)-1;
  a->nslots = a->nitems;
  
  if (a->nslots is 0) {
    a->data = NULL;
    return;
  }
  
  a->data = malloc(a->nslots * Array_Step(a));
  a->sspace0 = malloc(Array_Step(a));
  a->sspace1 = malloc(Array_Step(a));
  
#if CELLO_MEMORY_CHECK == 1
  if (a->data is NULL or a->sspace0 is NULL or a->sspace1 is NULL) {
    throw(OutOfMemoryError, "Cannot allocate Array, out of memory!");
  }
#endif
  
  for(size_t i = 0; i < a->nitems; i++) {
    Array_Alloc(a, i);
    assign(Array_Item(a, i), get(args, $I(i+1)));  
  }
  
}

static var Array_Subtype(var self) {
  struct Array* a = self;
  return a->type;
}

static void Array_Del(var self) {
  
  struct Array* a = self;
  
  for(size_t i = 0; i < a->nitems; i++) {
    destruct(Array_Item(a, i));
  }
  
  free(a->data);
  free(a->sspace0);
  free(a->sspace1);
  
}

static void Array_Clear(var self) {
  struct Array* a = self;
  
  for(size_t i = 0; i < a->nitems; i++) {
    destruct(Array_Item(a, i));
  }
  
  free(a->data);
  a->data  = NULL;
  a->nitems = 0;
  a->nslots = 0;
}

static void Array_Assign(var self, var obj) {
  struct Array* a = self;

  Array_Clear(self);
  
  a->type = subtype(obj);
  a->tsize = Array_Size_Round(size(a->type));
  a->nitems = len(obj);
  a->nslots = a->nitems;
  
  if (a->nslots is 0) {
    a->data = NULL;
    return;
  }
  
  a->data = malloc(a->nslots * Array_Step(a));
  a->sspace0 = realloc(a->sspace0, Array_Step(a));
  a->sspace1 = realloc(a->sspace1, Array_Step(a));
  
#if CELLO_MEMORY_CHECK == 1
  if (a->data is NULL or a->sspace0 is NULL or a->sspace1 is NULL) {
    throw(OutOfMemoryError, "Cannot allocate Array, out of memory!");
  }
#endif
  
  for(size_t i = 0; i < a->nitems; i++) {
    Array_Alloc(a, i);
    assign(Array_Item(a, i), get(obj, $I(i)));  
  }
  
}

static void Array_Reserve_More(struct Array* a) {
  
  if (a->nitems > a->nslots) {
    a->nslots = a->nitems + a->nitems / 2;
    a->data = realloc(a->data, Array_Step(a) * a->nslots);
#if CELLO_MEMORY_CHECK == 1
    if (a->data is NULL) {
      throw(OutOfMemoryError, "Cannot grow Array, out of memory!");
    }
#endif
  }

}

static void Array_Concat(var self, var obj) {
  
  struct Array* a = self;
  
  size_t i = 0;
  size_t olen = len(obj);
  
  a->nitems += olen;
  Array_Reserve_More(a);
  
  foreach (item in obj) {
    Array_Alloc(a, a->nitems-olen+i);
    assign(Array_Item(a, a->nitems-olen+i), item);
    i++;
  }
  
}

static var Array_Copy(var self) { 
  struct Array* a = self;
  var b = new(Array, a->type);
  Array_Concat(b, self);
  return b;
}

static bool Array_Eq(var self, var obj) {
  
  struct Array* a = self;
  if (a->nitems isnt len(obj)) { return false; }
  
  for(size_t i = 0; i < a->nitems; i++) {
    if (neq(Array_Item(a, i), get(obj, $I(i)))) {
      return false;
    }
  }
  
  return true;
}

static size_t Array_Len(var self) {
  struct Array* a = self;
  return a->nitems;
}

static bool Array_Mem(var self, var obj) {
  struct Array* a = self;
  for(size_t i = 0; i < a->nitems; i++) {
    if (eq(Array_Item(a, i), obj)) {
      return true;
    }
  }
  return false;
}

static void Array_Reserve_Less(struct Array* a) {
  if (a->nslots > a->nitems + a->nitems / 2) {
    a->nslots = a->nitems;
    a->data = realloc(a->data, Array_Step(a) * a->nslots);
  }
}

static void Array_Pop_At(var self, var key) {

  struct Array* a = self;
  int64_t i = c_int(key);
  
#if CELLO_BOUND_CHECK == 1
  if (i < 0 or i >= (int64_t)a->nitems) {
    throw(IndexOutOfBoundsError,
      "Index '%i' out of bounds for Array of size %i.", key, $I(a->nitems));
    return;
  }
#endif
  
  destruct(Array_Item(a, i));
  
  memmove((char*)a->data + Array_Step(a) * (i+0), 
          (char*)a->data + Array_Step(a) * (i+1), 
          Array_Step(a) * ((a->nitems-1) - i));
  
  a->nitems--;
  Array_Reserve_Less(a);
}

static void Array_Rem(var self, var obj) {
  struct Array* a = self;
  for(size_t i = 0; i < a->nitems; i++) {
    if (eq(Array_Item(a, i), obj)) {
      Array_Pop_At(a, $I(i));
      return;
    }
  }
  throw(ValueError, "Object %$ not in Array!", obj);
}

static void Array_Push(var self, var obj) {
  struct Array* a = self;
  a->nitems++;
  Array_Reserve_More(a);
  Array_Alloc(a, a->nitems-1);
  assign(Array_Item(a, a->nitems-1), obj);
}

static void Array_Push_At(var self, var obj, var key) {
  struct Array* a = self;
  a->nitems++;
  Array_Reserve_More(a);
  
  int64_t i = c_int(key);
  
#if CELLO_BOUND_CHECK == 1
  if (i < 0 or i >= (int64_t)a->nitems) {
    throw(IndexOutOfBoundsError,
      "Index '%i' out of bounds for Array of size %i.", key, $I(a->nitems));
    return;
  }
#endif
  
  memmove((char*)a->data + Array_Step(a) * (i+1),
          (char*)a->data + Array_Step(a) * (i+0), 
          Array_Step(a) * ((a->nitems-1) - i));
  
  Array_Alloc(self, i);
  assign(Array_Item(a, i), obj);
}

static void Array_Pop(var self) {

  struct Array* a = self;
  
#if CELLO_BOUND_CHECK == 1
  if (a->nitems is 0) {
    throw(IndexOutOfBoundsError, "Cannot pop. Array is empty!");
    return;
  }
#endif
  
  destruct(Array_Item(a, a->nitems-1));
  
  a->nitems--;
  Array_Reserve_Less(a);
}

static var Array_Get(var self, var key) {

  struct Array* a = self;
  int64_t i = c_int(key);
  
#if CELLO_BOUND_CHECK == 1
  if (i < 0 or i >= (int64_t)a->nitems) {
    return throw(IndexOutOfBoundsError,
      "Index '%i' out of bounds for Array of size %i.", key, $I(a->nitems));
  }
#endif
  
  return Array_Item(a, i);
}

static void Array_Set(var self, var key, var val) {

  struct Array* a = self;
  int64_t i = c_int(key);
  
#if CELLO_BOUND_CHECK == 1
  if (i < 0 or i >= (int64_t)a->nitems) {
    throw(IndexOutOfBoundsError, 
      "Index '%i' out of bounds for Array of size %i.", key, $I(a->nitems));
    return;
  }
#endif
  
  assign(Array_Item(a, i), val);
}

static var Array_Iter_Init(var self) {
  struct Array* a = self;
  if (a->nitems is 0) { return NULL; }
  return Array_Item(a, 0);
}

static var Array_Iter_Next(var self, var curr) {
  struct Array* a = self;
  if (curr >= Array_Item(a, a->nitems-1)) {
    return NULL;
  } else {
    return (char*)curr + Array_Step(a);
  }
}

static void Array_Swap(struct Array* a, size_t i, size_t j) {
  if (i == j) { return; }
  memcpy((char*)a->sspace0, (char*)a->data + i * Array_Step(a), Array_Step(a));
  memcpy((char*)a->data + i * Array_Step(a),
         (char*)a->data + j * Array_Step(a), Array_Step(a));
  memcpy((char*)a->data + j * Array_Step(a), (char*)a->sspace0, Array_Step(a));
}

static void Array_Reverse(var self) {
  struct Array* a = self;
  for(size_t i = 0; i < a->nitems / 2; i++) {
    Array_Swap(a, i, a->nitems-1-i);
  }
}

static size_t Array_Sort_Partition(
  struct Array* a, int64_t l, int64_t r, var f) {
  
  int64_t p = l + (r - l) / 2;
  memcpy((char*)a->sspace1, (char*)a->data + p * Array_Step(a), Array_Step(a));
  
  Array_Swap(a, p, r);
  
  int64_t s = l;
  for (int64_t i = l; i < r; i++) {
    if (call(f, 
      Array_Get(a, $I(i)), 
      (char*)a->sspace1 + sizeof(struct Header))) {
      Array_Swap(a, i, s);
      s++;
    }
  }
  
  Array_Swap(a, s, r);
  
  return s;
}

static void Array_Sort_Part(struct Array* a, int64_t l, int64_t r, var f) {
  if (l < r) {
    int64_t s = Array_Sort_Partition(a, l, r, f);
    Array_Sort_Part(a, l, s-1, f);
    Array_Sort_Part(a, s+1, r, f);
  }
}

static void Array_Sort_With(var self, var f) {
  Array_Sort_Part(self, 0, Array_Len(self)-1, f);
}

static int Array_Show(var self, var output, int pos) {
  struct Array* a = self;
  int ipos = pos;
  pos += print_to(output, pos, "<'Array' At 0x%p [", self);
  for(size_t i = 0; i < a->nitems; i++) {
    pos += print_to(output, pos, "%$", Array_Item(a, i));
    if (i < a->nitems-1) { pos += print_to(output, pos, ", "); }
  }
  pos += print_to(output, pos, "]>");
  return pos - ipos;
}

static void Array_Reserve(var self, var amount) {
  struct Array* a = self;
  int64_t nnslots = c_int(amount);
  
#if CELLO_BOUND_CHECK == 1
  if (nnslots < (int64_t)a->nitems) {
    throw(IndexOutOfBoundsError, 
      "Array already has %li items, cannot reserve %li", $I(a->nitems), amount);
  }
#endif
  
  a->nslots = nnslots;
  a->data = realloc(a->data, Array_Step(a) * a->nslots);

#if CELLO_MEMORY_CHECK == 1
  if (a->data is NULL) {
    throw(OutOfMemoryError, "Cannot grow Array, out of memory!");
  }
#endif

}

static void Array_Mark(var self, var gc, void(*f)(var,void*)) {
  struct Array* a = self;
  for (size_t i = 0; i < a->nitems; i++) {
    f(gc, Array_Item(a, i));
  }
}

var Array = Cello(Array,
  Instance(Doc,
    Array_Name,        Array_Brief,
    Array_Description, NULL,
    NULL),
  Instance(New,     Array_New, Array_Del),
  Instance(Subtype, Array_Subtype, NULL, NULL),
  Instance(Assign,  Array_Assign),
  Instance(Copy,    Array_Copy),
  Instance(Mark,    Array_Mark),
  Instance(Eq,      Array_Eq),
  Instance(Clear,   Array_Clear),
  Instance(Push,
    Array_Push,     Array_Pop,
    Array_Push_At,  Array_Pop_At),
  Instance(Len,     Array_Len),
  Instance(Get,     Array_Get, Array_Set, Array_Mem, Array_Rem),
  Instance(Iter,    Array_Iter_Init, Array_Iter_Next),
  Instance(Reverse, Array_Reverse),
  Instance(Sort,    Array_Sort_With),
  Instance(Show,    Array_Show, NULL),
  Instance(Reserve, Array_Reserve));

  
