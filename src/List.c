#include "Cello.h"

static const char* List_Name(void) {
  return "List";
}

static const char* List_Brief(void) {
  return "Linked List";
}

/* TODO: Make better */
static const char* List_Description(void) {
  return
    "The `List` type is a linked list data structure providing all of the "
    "expected operations.";
}

/* TODO */
static const char* List_Examples(void) {
  return "";
}

/* TODO */
static const char* List_Methods(void) {
  return "";
};

struct List {
  var type;
  var head;
  var tail;
  size_t tsize;
  size_t nitems;
};

static var List_Alloc(struct List* l) {
  var item = calloc(1, 2 * sizeof(var) + sizeof(struct CelloHeader) + l->tsize);
  
#if CELLO_MEMORY_CHECK == 1
  if (item is NULL) {
    throw(OutOfMemoryError, "Cannot allocate List entry, out of memory!");
  }
#endif
  
  return header_init((struct CelloHeader*)(
    (char*)item + 2 * sizeof(var)), l->type, AllocData);
}

static void List_Free(struct List* l, var self) {
  free((char*)self - sizeof(struct CelloHeader) - 2 * sizeof(var));
}

static var* List_Next(struct List* l, var self) {
  return (var*)((char*)self - sizeof(struct CelloHeader) - 1 * sizeof(var));
}

static var* List_Prev(struct List* l, var self) {
  return (var*)((char*)self - sizeof(struct CelloHeader) - 2 * sizeof(var));
}

static var List_At(struct List* l, int64_t i) {

#if CELLO_BOUND_CHECK == 1
  if (i < 0 or i >= (int64_t)l->nitems) {
    return throw(IndexOutOfBoundsError,
      "Index '%i' out of bounds for List of size %i.", 
       $(Int, i), $(Int, l->nitems));
  }
#endif
  
  var item;
  
  if (i <= (int64_t)(l->nitems / 2)) {
    item = *List_Next(l, l->head);
    while (i) { item = *List_Next(l, item); i--; }
  } else {
    i = l->nitems-i-1;
    item = *List_Prev(l, l->tail);
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
  l->head = List_Alloc(l);
  l->tail = List_Alloc(l);
  
  *List_Next(l, l->head) = l->tail;
  *List_Prev(l, l->head) = NULL;
  *List_Next(l, l->tail) = NULL;
  *List_Prev(l, l->tail) = l->head;
  
  size_t nargs = len(args);
  for(size_t i = 0; i < nargs-1; i++) {
    List_Push(self, get(args, $I(i+1)));
  }
  
}

static var List_Subtype(var self) {
  struct List* l = self;
  return l->type;
}

static void List_Clear(var self) {
  struct List* l = self;
  var item = *List_Next(l, l->head);
  while (item isnt l->tail) {
    var next = *List_Next(l, item);
	  destruct(item);
    List_Free(l, item);
    item = next;
  }
  *List_Next(l, l->head) = l->tail;
  *List_Prev(l, l->tail) = l->head;
  l->nitems = 0;
}

static void List_Del(var self) {
  struct List* l = self;
  List_Clear(self);
  List_Free(l, l->head);
  List_Free(l, l->tail);
}

static void List_Assign(var self, var obj) {
  struct List* l = self;

  List_Clear(self);
  if (l->head isnt NULL) { List_Free(l, l->head); }
  if (l->head isnt NULL) { List_Free(l, l->tail); }
  
  l->type = subtype(obj);
  l->tsize = size(l->type);
  l->head = List_Alloc(l);
  l->tail = List_Alloc(l);
  
  *List_Next(l, l->head) = l->tail;
  *List_Prev(l, l->head) = NULL;
  *List_Next(l, l->tail) = NULL;
  *List_Prev(l, l->tail) = l->head;
  
  size_t nargs = len(obj);
  for(size_t i = 0; i < nargs; i++) {
    List_Push(self, get(obj, $I(i)));
  }
  
}

static void List_Concat(var self, var obj) {
  foreach (item in obj) {
    List_Push(self, item);
  }
}

static var List_Copy(var self) { 
  struct List* a = self;
  var b = new(List, a->type);
  List_Concat(b, self);
  return b;
}

static bool List_Eq(var self, var obj) {
  
  struct List* l = self;
  if (l->nitems isnt len(obj)) { return false; }
  
  var item = *List_Next(l, l->head);
  foreach (oitem in obj) {
    if (neq(item, oitem)) { return false; }
    item = *List_Next(l, item);
  }
  
  return true;
}

static size_t List_Len(var self) {
  struct List* l = self;
  return l->nitems;
}

static bool List_Mem(var self, var obj) {
  struct List* l = self;
  var item = *List_Next(l, l->head);
  while (item isnt l->tail) {
    if (eq(item, obj)) { return true; }
    item = *List_Next(l, item);
  }
  return false;
}

static void List_Pop_At(var self, var key) {

  struct List* l = self;
  int64_t i = c_int(key);
  
  var item = List_At(l, i);
  var prev = *List_Prev(l, item);
  var next = *List_Next(l, item);
  *List_Next(l, prev) = next;
  *List_Prev(l, next) = prev;
  destruct(item);
  List_Free(l, item);
  l->nitems--;
}

static void List_Rem(var self, var obj) {
  struct List* l = self;
  var item = *List_Next(l, l->head);
  while (item isnt l->tail) {
    if (eq(item, obj)) {
      var prev = *List_Prev(l, item);
      var next = *List_Next(l, item);
      *List_Next(l, prev) = next;
      *List_Prev(l, next) = prev;
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
  
  var last = *List_Prev(l, l->tail);
  *List_Next(l, last) = item;
  *List_Prev(l, item) = last;
  *List_Next(l, item) = l->tail;
  *List_Prev(l, l->tail) = item;
  l->nitems++;
  
}

static void List_Push_At(var self, var obj, var key) {
  struct List* l = self;
  
  var item = List_Alloc(l);
  assign(item, obj);
  
  int64_t i = c_int(key);
  var next = List_At(l, i);
  var prev = *List_Prev(l, next);
  *List_Prev(l, next) = item;
  *List_Next(l, prev) = item;
  *List_Next(l, item) = next;
  *List_Prev(l, item) = prev;
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
  
  var last = *List_Prev(l, l->tail);
  var prev = *List_Prev(l, last);
  destruct(last);
  List_Free(l, last);
  *List_Next(l, prev) = l->tail;
  *List_Prev(l, l->tail) = prev;
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
  if (l->nitems is 0) { return NULL; }
  return *List_Next(l, l->head);
}

static var List_Iter_Next(var self, var curr) {
  struct List* l = self;
  var next = *List_Next(l, curr);
  return next is l->tail ? NULL : next;
}

static void List_Swap(struct List* l, var iitem, var jitem) {
  var inext = *List_Next(l, iitem); var iprev = *List_Prev(l, iitem);
  var jnext = *List_Next(l, jitem); var jprev = *List_Prev(l, jitem); 
  *List_Next(l, iprev) = jitem; *List_Prev(l, inext) = jitem;
  *List_Next(l, jitem) = inext; *List_Prev(l, jitem) = iprev;
  *List_Next(l, jprev) = iitem; *List_Prev(l, jnext) = iitem;
  *List_Next(l, iitem) = jnext; *List_Prev(l, iitem) = jprev;
}

static void List_Reverse(var self) {
  struct List* l = self;
  var item0 = *List_Next(l, l->head);
  var item1 = *List_Prev(l, l->tail);
  
  if (item0 is l->tail
  or  item1 is l->head
  or  item0 is item1) { return; }
  
  while (true) {
    var item0_next = *List_Next(l, item0);
    var item1_prev = *List_Prev(l, item1);
    List_Swap(l, item0, item1);
    if (item0_next is item1
    or  item0_next is item1_prev) { break; }
    item0 = item0_next;
    item1 = item1_prev;
  }
}

/* TODO: Linked list sort */

static int List_Show(var self, var output, int pos) {
  struct List* l = self;
  pos = print_to(output, pos, "<'List' At 0x%p [", self);
  var item = *List_Next(l, l->head);
  while (item isnt l->tail) {
    pos = print_to(output, pos, "%$", item);
    item = *List_Next(l, item);
    if (item isnt l->tail) { pos = print_to(output, pos, ", "); }
  }
  pos = print_to(output, pos, "]>");
  return pos;
}

static void List_Reserve(var self, var amount) {
  struct List* l = self;
  int64_t nslots = c_int(amount);
  
#if CELLO_BOUND_CHECK == 1
  if (nslots < (int64_t)l->nitems) {
    throw(IndexOutOfBoundsError, 
      "List already has %li items, cannot reserve %li", $I(l->nitems), amount);
  }
#endif
  
  while (nslots - l->nitems) {
    var item = List_Alloc(l);
    var last = *List_Prev(l, l->tail);
    *List_Next(l, last) = item;
    *List_Prev(l, l->tail) = item;
    nslots--;
  }
  
}

static void List_Traverse(var self, var func) {
  struct List* l = self;
  var item = *List_Next(l, l->head);
  while (item isnt l->tail) {
    call_with(func, item);
    item = *List_Next(l, item);
  }
}

var List = Cello(List,
  Instance(Doc,
    List_Name,        List_Brief,
    List_Description, List_Examples,
    List_Methods),
  Instance(New,      List_New, List_Del),
  Instance(Subtype,  List_Subtype, NULL, NULL),
  Instance(Assign,   List_Assign),
  Instance(Copy,     List_Copy),
  Instance(Traverse, List_Traverse),
  Instance(Eq,       List_Eq),
  Instance(Clear,    List_Clear),
  Instance(Push,
    List_Push,       List_Pop,
    List_Push_At,    List_Pop_At),
  Instance(Len,      List_Len),
  Instance(Get,      List_Get, List_Set, List_Mem, List_Rem),
  Instance(Iter,     List_Iter_Init, List_Iter_Next),
  Instance(Reverse,  List_Reverse),
  //Member(Sort,     List_Sort_With),
  Instance(Show,     List_Show, NULL),
  Instance(Reserve,  List_Reserve));
  
