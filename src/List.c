#include "Cello.h"

static const char* List_Name(void) {
  return "List";
}

static const char* List_Brief(void) {
  return "Linked List";
}

static const char* List_Description(void) {
  return
    "The `List` type is a linked list data structure. Elements can be added "
    "and removed from the list and their memory is allocated and deallocated "
    "by the structure. Additionally destructors will be called on objects "
    "once removed."
    "\n\n"
    "Elements are copied into the List using `assign` and will initially have "
    "zero'd memory."
    "\n\n"
    "Lists can provide fast insertion and removal at arbitrary locations "
    "although most other operations will be slow due to having to traverse "
    "the linked list data structure."
    "\n\n"
    "This is largely equivalent to the C++ construct "
    "[std::list](http://www.cplusplus.com/reference/list/list/)";
}

static struct Example* List_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Construction & Deletion",
      "var x = new(List, Int);\n"
      "push(x, $I(32));\n"
      "push(x, $I(6));\n"
      "\n"
      "/* <'List' At 0x0000000000414603 [32, 6]> */\n"
      "show(x);\n",
    }, {
      "Element Access",
      "var x = new(List, Float, $F(0.01), $F(5.12));\n"
      "\n"
      "show(get(x, $I(0))); /* 0.01 */\n"
      "show(get(x, $I(1))); /* 5.12 */\n"
      "\n"
      "set(x, $I(0), $F(500.1));\n"
      "show(get(x, $I(0))); /* 500.1 */\n",
    }, {
      "Membership",
      "var x = new(List, Int, $I(1), $I(2), $I(3), $I(4));\n"
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
      "var greetings = new(List, String, \n"
      "  $S(\"Hello\"), $S(\"Bonjour\"), $S(\"Hej\"));\n"
      "\n"
      "foreach(greet in greetings) {\n"
      "  show(greet);\n"
      "}\n",
    }, {NULL, NULL}
  };
  
  return examples;
}


struct List {
  var type;
  var head;
  var tail;
  size_t tsize;
  size_t nitems;
};

static var List_Alloc(struct List* l) {
  var item = calloc(1, 2 * sizeof(var) + sizeof(struct Header) + l->tsize);
  
#if CELLO_MEMORY_CHECK == 1
  if (item is NULL) {
    throw(OutOfMemoryError, "Cannot allocate List entry, out of memory!");
  }
#endif
  
  return header_init((struct Header*)(
    (char*)item + 2 * sizeof(var)), l->type, AllocData);
}

static void List_Free(struct List* l, var self) {
  free((char*)self - sizeof(struct Header) - 2 * sizeof(var));
}

static var* List_Next(struct List* l, var self) {
  return (var*)((char*)self - sizeof(struct Header) - 1 * sizeof(var));
}

static var* List_Prev(struct List* l, var self) {
  return (var*)((char*)self - sizeof(struct Header) - 2 * sizeof(var));
}

static var List_At(struct List* l, int64_t i) {

  i = i < 0 ? l->nitems+i : i;

#if CELLO_BOUND_CHECK == 1
  if (i < 0 or i >= (int64_t)l->nitems) {
    return throw(IndexOutOfBoundsError,
      "Index '%i' out of bounds for List of size %i.", 
       $(Int, i), $(Int, l->nitems));
  }
#endif
  
  var item;
  
  if (i <= (int64_t)(l->nitems / 2)) {
    item = l->head;
    while (i) { item = *List_Next(l, item); i--; }
  } else {
    i = l->nitems-i-1;
    item = l->tail;
    while (i) { item = *List_Prev(l, item); i--; }
  }
  
  return item;
}

static void List_Push(var self, var obj);
static void List_Rem(var self, var obj);

static void List_New(var self, var args) {
  
  struct List* l = self;
  l->type   = cast(get(args, $I(0)), Type);
  l->tsize  = size(l->type);
  l->nitems = 0;
  l->head = NULL;
  l->tail = NULL;
  
  size_t nargs = len(args);
  for(size_t i = 0; i < nargs-1; i++) {
    List_Push(self, get(args, $I(i+1)));
  }
  
}

static void List_Clear(var self) {
  struct List* l = self;
  var item = l->head;
  while (item) {
    var next = *List_Next(l, item);
	  destruct(item);
    List_Free(l, item);
    item = next;
  }
  l->tail = NULL;
  l->head = NULL;
  l->nitems = 0;
}

static void List_Del(var self) {
  struct List* l = self;
  List_Clear(self);
}

static void List_Assign(var self, var obj) {
  struct List* l = self;
  
  List_Clear(self);
  
  l->type = implements_method(obj, Iter, iter_type) ? iter_type(obj) : Ref;
  l->tsize = size(l->type);
  
  size_t nargs = len(obj);
  for (size_t i = 0; i < nargs; i++) {
    List_Push(self, get(obj, $I(i)));
  }
  
}

static void List_Concat(var self, var obj) {
  foreach (item in obj) {
    List_Push(self, item);
  }
}

static var List_Iter_Init(var self);
static var List_Iter_Next(var self, var curr);

static int List_Cmp(var self, var obj) {
  
  var item0 = List_Iter_Init(self);
  var item1 = iter_init(obj);
  
  while (true) {
    if (item0 is Terminal and item1 is Terminal) { return 0; }
    if (item0 is Terminal) { return -1; }
    if (item1 is Terminal) { return  1; }
    int c = cmp(item0, item1);
    if (c < 0) { return -1; }
    if (c > 0) { return  1; }
    item0 = List_Iter_Next(self, item0);
    item1 = iter_next(obj, item1);
  }
  
  return 0;
}

static uint64_t List_Hash(var self) {
  struct List* l = self;
  uint64_t h = 0;
  
  var item = l->head;
  for (size_t i = 0; i < l->nitems; i++) {
    h ^= hash(item);
    item = *List_Next(l, item);
  }
  
  return h;
}

static size_t List_Len(var self) {
  struct List* l = self;
  return l->nitems;
}

static bool List_Mem(var self, var obj) {
  struct List* l = self;
  var item = l->head;
  while (item) {
    if (eq(item, obj)) { return true; }
    item = *List_Next(l, item);
  }
  return false;
}

static void List_Unlink(struct List* l, var item) {
  
  var next = *List_Next(l, item);
  var prev = *List_Prev(l, item);

  if (item is l->head and item is l->tail) {
    l->head = NULL; l->tail = NULL;
  } else if (item is l->head) {
    l->head = next;
    *List_Prev(l, next) = NULL;
  } else if (item is l->tail) {
    l->tail = prev;
    *List_Next(l, prev) = NULL;
  } else {
    *List_Next(l, prev) = next;
    *List_Prev(l, next) = prev;
  }
  
}

static void List_Link(struct List* l, var item, var prev, var next) {
  if (prev is NULL) { l->head = item; } else { *List_Next(l, prev) = item; }
  if (next is NULL) { l->tail = item; } else { *List_Prev(l, next) = item; }
  *List_Next(l, item) = next;
  *List_Prev(l, item) = prev;
}

static void List_Pop_At(var self, var key) {

  struct List* l = self;
  int64_t i = c_int(key);
  
  var item = List_At(l, i);
  List_Unlink(l, item);
  destruct(item);
  List_Free(l, item);
  l->nitems--;
}

static void List_Rem(var self, var obj) {
  struct List* l = self;
  var item = l->head;
  while (item) {
    if (eq(item, obj)) {
      List_Unlink(l, item);
      destruct(item);
      List_Free(l, item);
      l->nitems--;
      return;
    }
    item = *List_Next(l, item);
  }
  
  throw(ValueError, "Object %$ not in List!", obj);
}

static void List_Push(var self, var obj) {
  struct List* l = self;
  var item = List_Alloc(l);
  assign(item, obj);
  List_Link(l, item, l->tail, NULL);
  l->nitems++;
}

static void List_Push_At(var self, var obj, var key) {
  struct List* l = self;
  
  var item = List_Alloc(l);
  assign(item, obj);
  
  int64_t i = c_int(key);
  if (i is 0) {
    List_Link(l, item, NULL, l->head);
  } else {
    var curr = List_At(l, i);
    List_Link(l, item, *List_Prev(l, curr), curr);
  }
  l->nitems++;
}

static void List_Pop(var self) {

  struct List* l = self;
  
#if CELLO_BOUND_CHECK == 1
  if (l->nitems is 0) {
    throw(IndexOutOfBoundsError, "Cannot pop. List is empty!");
    return;
  }
#endif
  
  var item = l->tail;
  List_Unlink(l, item);
  destruct(item);
  List_Free(l, item);
  l->nitems--;
}

static var List_Get(var self, var key) {
  struct List* l = self;
  return List_At(l, c_int(key));
}

static void List_Set(var self, var key, var val) {
  struct List* l = self;
  assign(List_At(l, c_int(key)), val);
}

static var List_Iter_Init(var self) {
  struct List* l = self;
  if (l->nitems is 0) { return Terminal; }
  return l->head;
}

static var List_Iter_Next(var self, var curr) {
  struct List* l = self;
  curr = *List_Next(l, curr);
  return curr ? curr : Terminal;
}

static var List_Iter_Last(var self) {
  struct List* l = self;
  if (l->nitems is 0) { return Terminal; }
  return l->tail;
}

static var List_Iter_Prev(var self, var curr) {
  struct List* l = self;
  curr = *List_Prev(l, curr);
  return curr ? curr : Terminal;
}

static var List_Iter_Type(var self) {
  struct List* l = self;
  return l->type;
}

static int List_Show(var self, var output, int pos) {
  struct List* l = self;
  pos = print_to(output, pos, "<'List' At 0x%p [", self);
  var item = l->head;
  while (item) {
    pos = print_to(output, pos, "%$", item);
    item = *List_Next(l, item);
    if (item) { pos = print_to(output, pos, ", "); }
  }
  return print_to(output, pos, "]>");
}

static void List_Resize(var self, size_t n) {
  struct List* l = self;

  if (n is 0) {
    List_Clear(self);
    return;
  }
  
  while (n < l->nitems) {
    var item = l->tail;
    List_Unlink(l, item);
    destruct(item);
    List_Free(l, item);
    l->nitems--;
  }
  
  while (n > l->nitems) {
    var item = List_Alloc(l);
    List_Link(l, item, l->tail, NULL);
    l->nitems++;
  }
  
}

static void List_Mark(var self, var gc, void(*f)(var,void*)) {
  struct List* l = self;
  var item = l->head;
  while (item) {
    f(gc, item);
    item = *List_Next(l, item);
  }
}

var List = Cello(List,
  Instance(Doc,
    List_Name, List_Brief,    List_Description, 
    NULL,      List_Examples, NULL),
  Instance(New,     List_New, List_Del),
  Instance(Assign,  List_Assign),
  Instance(Mark,    List_Mark),
  Instance(Cmp,     List_Cmp),
  Instance(Hash,    List_Hash),
  Instance(Push,
    List_Push,      List_Pop,
    List_Push_At,   List_Pop_At),
  Instance(Concat,  List_Concat, List_Push),
  Instance(Len,     List_Len),
  Instance(Get,     List_Get, List_Set, List_Mem, List_Rem),
  Instance(Iter,
    List_Iter_Init, List_Iter_Next,
    List_Iter_Last, List_Iter_Prev, List_Iter_Type),
  Instance(Show,    List_Show, NULL),
  Instance(Resize,  List_Resize));
  
