#include "Cello.h"

static const char* Array_Name(void) {
  return "Array";
}

static const char* Array_Brief(void) {
  return "Sequential Container";
}

static const char* Array_Description(void) {
  return ""
    "The `Array` type is data structure containing a sequence of a single type "
    "of object. It can dynamically grow and shrink in size depending on how "
    "many elements it contains. It allocates storage for the type specified. "
    "It also deallocates and destroys the objects inside upon destruction."
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

static struct Example* Array_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Construction & Deletion",
      "var x = new(Array, Int);\n"
      "push(x, $I(32));\n"
      "push(x, $I(6));\n"
      "\n"
      "/* <'Array' At 0x0000000000414603 [32, 6]> */\n"
      "show(x);\n",
    }, {
      "Element Access",
      "var x = new(Array, Float, $F(0.01), $F(5.12));\n"
      "\n"
      "show(get(x, $I(0))); /* 0.01 */\n"
      "show(get(x, $I(1))); /* 5.12 */\n"
      "\n"
      "set(x, $I(0), $F(500.1));\n"
      "show(get(x, $I(0))); /* 500.1 */\n",
    }, {
      "Membership",
      "var x = new(Array, Int, $I(1), $I(2), $I(3), $I(4));\n"
      "\n"
      "show($I(mem(x, $I(1)))); /* 1 */\n"
      "show($I(len(x)));        /* 4 */\n"
      "\n"
      "rem(x, $I(3));\n"
      "\n"
      "show($I(mem(x, $I(3)))); /* 0 */\n"
      "show($I(len(x)));        /* 3 */\n"
      "show($I(empty(x)));      /* 0 */\n"
      "\n"
      "resize(x, 0);\n"
      "\n"
      "show($I(empty(x)));      /* 1 */\n",
    }, {
      "Iteration",
      "var greetings = new(Array, String, \n"
      "  $S(\"Hello\"), $S(\"Bonjour\"), $S(\"Hej\"));\n"
      "\n"
      "foreach(greet in greetings) {\n"
      "  show(greet);\n"
      "}\n",
    }, {NULL, NULL}
  };
  
  return examples;
}

struct Array {
  var type;
  var data;
  size_t tsize;
  size_t nitems;
  size_t nslots;
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
  
#if CELLO_MEMORY_CHECK == 1
  if (a->data is NULL) {
    throw(OutOfMemoryError, "Cannot allocate Array, out of memory!");
  }
#endif
  
  for(size_t i = 0; i < a->nitems; i++) {
    Array_Alloc(a, i);
    assign(Array_Item(a, i), get(args, $I(i+1)));  
  }
  
}

static void Array_Del(var self) {
  
  struct Array* a = self;
  
  for(size_t i = 0; i < a->nitems; i++) {
    destruct(Array_Item(a, i));
  }
  
  free(a->data);
  
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

static void Array_Push(var self, var obj);

static void Array_Assign(var self, var obj) {
  struct Array* a = self;

  Array_Clear(self);
  
  a->type = implements_method(obj, Iter, iter_type) ? iter_type(obj) : Ref;
  a->tsize = Array_Size_Round(size(a->type));
  a->nitems = 0;
  a->nslots = 0;
  
  if (implements_method(obj, Len, len)
  and implements_method(obj, Get, get)) {
  
    a->nitems = len(obj);
    a->nslots = a->nitems;
    
    if (a->nslots is 0) {
      a->data = NULL;
      return;
    }
    
    a->data = malloc(a->nslots * Array_Step(a));
    
  #if CELLO_MEMORY_CHECK == 1
    if (a->data is NULL) {
      throw(OutOfMemoryError, "Cannot allocate Array, out of memory!");
    }
  #endif
    
    for(size_t i = 0; i < a->nitems; i++) {
      Array_Alloc(a, i);
      assign(Array_Item(a, i), get(obj, $I(i)));  
    }
  
  } else {
    
    foreach (item in obj) {
      Array_Push(self, item);
    }
    
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

static var Array_Iter_Init(var self);
static var Array_Iter_Next(var self, var curr);

static int Array_Cmp(var self, var obj) {
  
  var item0 = Array_Iter_Init(self);
  var item1 = iter_init(obj);
  
  while (true) {
    if (item0 is Terminal and item1 is Terminal) { return 0; }
    if (item0 is Terminal) { return -1; }
    if (item1 is Terminal) { return  1; }
    int c = cmp(item0, item1);
    if (c < 0) { return -1; }
    if (c > 0) { return  1; }
    item0 = Array_Iter_Next(self, item0);
    item1 = iter_next(obj, item1);
  }
  
  return 0;
}

static uint64_t Array_Hash(var self) {
  struct Array* a = self;
  uint64_t h = 0;
  
  for (size_t i = 0; i < a->nitems; i++) {
    h ^= hash(Array_Item(a, i));
  }
  
  return h;
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
  i = i < 0 ? a->nitems+i : i;
  
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
  i = i < 0 ? a->nitems+i : i;
  
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
  i = i < 0 ? a->nitems+i : i;
  
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
  i = i < 0 ? a->nitems+i : i;
  
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
  if (a->nitems is 0) { return Terminal; }
  return Array_Item(a, 0);
}

static var Array_Iter_Next(var self, var curr) {
  struct Array* a = self;
  if (curr >= Array_Item(a, a->nitems-1)) {
    return Terminal;
  } else {
    return (char*)curr + Array_Step(a);
  }
}

static var Array_Iter_Last(var self) {
  struct Array* a = self;
  if (a->nitems is 0) { return Terminal; }
  return Array_Item(a, a->nitems-1);
}

static var Array_Iter_Prev(var self, var curr) {
  struct Array* a = self;
  if (curr < Array_Item(a, 0)) {
    return Terminal;
  } else {
    return (char*)curr - Array_Step(a);
  }
}

static var Array_Iter_Type(var self) {
  struct Array* a = self;
  return a->type;
}

static size_t Array_Sort_Partition(
  struct Array* a, int64_t l, int64_t r, bool(*f)(var,var)) {
  
  int64_t p = l + (r - l) / 2;
  swap(Array_Item(a, p), Array_Item(a, r));
  
  int64_t s = l;
  for (int64_t i = l; i < r; i++) {
    if (f(Array_Get(a, $I(i)), Array_Item(a, r))) {
      swap(Array_Item(a, i), Array_Item(a, s));
      s++;
    }
  }
  
  swap(Array_Item(a, s), Array_Item(a, r));
  
  return s;
}

static void Array_Sort_Part(struct Array* a, int64_t l, int64_t r, bool(*f)(var,var)) {
  if (l < r) {
    int64_t s = Array_Sort_Partition(a, l, r, f);
    Array_Sort_Part(a, l, s-1, f);
    Array_Sort_Part(a, s+1, r, f);
  }
}

static void Array_Sort_By(var self, bool(*f)(var,var)) {
  Array_Sort_Part(self, 0, Array_Len(self)-1, f);
}

static int Array_Show(var self, var output, int pos) {
  struct Array* a = self;
  pos = print_to(output, pos, "<'Array' At 0x%p [", self);
  for (size_t i = 0; i < a->nitems; i++) {
    pos = print_to(output, pos, "%$", Array_Item(a, i));
    if (i < a->nitems-1) { pos = print_to(output, pos, ", "); }
  }
  return print_to(output, pos, "]>");
}

static void Array_Resize(var self, size_t n) {
  struct Array* a = self;
  
  if (n is 0) {
    Array_Clear(self);
    return;
  }
  
  while (n < a->nitems) {
    destruct(Array_Item(a, a->nitems-1));
    a->nitems--;
  }
  
  a->nslots = n;
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
    Array_Name, Array_Brief,    Array_Description, 
    NULL,       Array_Examples, NULL),
  Instance(New,     Array_New, Array_Del),
  Instance(Assign,  Array_Assign),
  Instance(Mark,    Array_Mark),
  Instance(Cmp,     Array_Cmp),
  Instance(Hash,    Array_Hash),
  Instance(Push,
    Array_Push,     Array_Pop,
    Array_Push_At,  Array_Pop_At),
  Instance(Concat,  Array_Concat, Array_Push),
  Instance(Len,     Array_Len),
  Instance(Get,     Array_Get, Array_Set, Array_Mem, Array_Rem),
  Instance(Iter,   
    Array_Iter_Init, Array_Iter_Next, 
    Array_Iter_Last, Array_Iter_Prev, Array_Iter_Type),
  Instance(Sort,    Array_Sort_By),
  Instance(Show,    Array_Show, NULL),
  Instance(Resize,  Array_Resize));

  
