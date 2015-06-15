#include "Cello.h"

static const char* Table_Name(void) {
  return "Table";
}

static const char* Table_Brief(void) {
  return "Hash table";
}

static const char* Table_Description(void) {
  return
    "The `Table` type is a hash table data structure that maps keys to values. "
    "It uses an open-addressing robin-hood hashing scheme which requires "
    "`Hash` and `Cmp` to be defined on the key type. Keys and values are "
    "copied into the collection using the `Assign` class and intially have "
    "zero'd memory."
    "\n\n"
    "Hash tables provide `O(1)` lookup, insertion and removal can but require "
    "long pauses when the table must be _rehashed_ and all entries processed."
    "\n\n"
    "This is largely equivalent to the C++ construct "
    "[std::unordered_map](http://www.cplusplus.com/reference/unordered_map/unordered_map/)";
}

static struct Example* Table_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var prices = new(Table, String, Int);\n"
      "set(prices, $S(\"Apple\"),  $I(12));\n"
      "set(prices, $S(\"Banana\"), $I( 6));\n"
      "set(prices, $S(\"Pear\"),   $I(55));\n"
      "\n"
      "foreach (key in prices) {\n"
      "  var price = get(prices, key);\n"
      "  println(\"Price of %$ is %$\", key, price);\n"
      "}\n"
    }, {
      "Manipulation",
      "var t = new(Table, String, Int);\n"
      "set(t, $S(\"Hello\"), $I(2));\n"
      "set(t, $S(\"There\"), $I(5));\n"
      "\n"
      "show($I(len(t))); /* 2 */\n"
      "show($I(mem(t, $S(\"Hello\")))); /* 1 */\n"
      "\n"
      "rem(t, $S(\"Hello\"));\n"
      "\n"
      "show($I(len(t))); /* 1 */\n"
      "show($I(mem(t, $S(\"Hello\")))); /* 0 */\n"
      "show($I(mem(t, $S(\"There\")))); /* 1 */\n"
      "\n"
      "resize(t, 0);\n"
      "\n"
      "show($I(len(t))); /* 0 */\n"
      "show($I(mem(t, $S(\"Hello\")))); /* 0 */\n"
      "show($I(mem(t, $S(\"There\")))); /* 0 */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

struct Table {
  var data;
  var ktype;
  var vtype;
  size_t ksize;
  size_t vsize;
  size_t nslots;
  size_t nitems;
  var sspace0;
  var sspace1;
};

enum {
  TABLE_PRIMES_COUNT = 24
};

static const size_t Table_Primes[TABLE_PRIMES_COUNT] = {
  0,       1,       5,       11,
  23,      53,      101,     197,
  389,     683,     1259,    2417,
  4733,    9371,    18617,   37097,
  74093,   148073,  296099,  592019,
  1100009, 2200013, 4400021, 8800019
};

static const double Table_Load_Factor = 0.9;

static size_t Table_Ideal_Size(size_t size) {
  size = (size_t)((double)(size+1) / Table_Load_Factor);
  for (size_t i = 0; i < TABLE_PRIMES_COUNT; i++) {
    if (Table_Primes[i] >= size) { return Table_Primes[i]; }
  }
  size_t last = Table_Primes[TABLE_PRIMES_COUNT-1];
  for (size_t i = 0;; i++) {
    if (last * i >= size) { return last * i; }
  }
}

static size_t Table_Step(struct Table* t) {
  return
    sizeof(uint64_t) + 
    sizeof(struct Header) + t->ksize + 
    sizeof(struct Header) + t->vsize;
}

static uint64_t Table_Key_Hash(struct Table* t, uint64_t i) {
  return *(uint64_t*)((char*)t->data + i * Table_Step(t));
}

static var Table_Key(struct Table* t, uint64_t i) {
  return (char*)t->data + i * Table_Step(t) +
    sizeof(uint64_t) + 
    sizeof(struct Header);  
}

static var Table_Val(struct Table* t, uint64_t i) {
  return (char*)t->data + i * Table_Step(t) +
    sizeof(uint64_t) + 
    sizeof(struct Header) + 
    t->ksize + 
    sizeof(struct Header);  
}

static uint64_t Table_Probe(struct Table* t, uint64_t i, uint64_t h) {
  int64_t v = i - (h-1);
  if (v < 0) {
    v = t->nslots + v;
  }
  return v;
}

static void Table_Set(var self, var key, var val);
static void Table_Set_Move(var self, var key, var val, bool move);

static size_t Table_Size_Round(size_t s) {
  return ((s + sizeof(var) - 1) / sizeof(var)) * sizeof(var);
}

static void Table_New(var self, var args) {
  
  struct Table* t = self;
  t->ktype = cast(get(args, $(Int, 0)), Type);
  t->vtype = cast(get(args, $(Int, 1)), Type);
  t->ksize = Table_Size_Round(size(t->ktype));
  t->vsize = Table_Size_Round(size(t->vtype));
  
  size_t nargs = len(args);
  if (nargs % 2 isnt 0) {
    throw(FormatError, 
      "Received non multiple of two argument count to Table constructor.");
  }
  
  t->nslots = Table_Ideal_Size((nargs-2)/2);
  t->nitems = 0;
  
  if (t->nslots is 0) {
    t->data = NULL;
    return;
  }
  
  t->data = calloc(t->nslots, Table_Step(t));
  t->sspace0 = calloc(1, Table_Step(t));
  t->sspace1 = calloc(1, Table_Step(t));
  
#if CELLO_MEMORY_CHECK == 1
  if (t->data is NULL or t->sspace0 is NULL or t->sspace1 is NULL) {
    throw(OutOfMemoryError, "Cannot allocate Table, out of memory!");
  }
#endif
  
  for(size_t i = 0; i < (nargs-2)/2; i++) {
    var key = get(args, $(Int, 2+(i*2)+0));
    var val = get(args, $(Int, 2+(i*2)+1));
    Table_Set_Move(t, key, val, false);
  }
  
}

static void Table_Del(var self) {
  struct Table* t = self;  
  
  for (size_t i = 0; i < t->nslots; i++) {
    if (Table_Key_Hash(t, i) isnt 0) {
      destruct(Table_Key(t, i));
      destruct(Table_Val(t, i));
    }
  }
  
  free(t->data);
  free(t->sspace0);
  free(t->sspace1);
  
}

static var Table_Key_Type(var self) {
  struct Table* t = self;  
  return t->ktype;
}

static var Table_Val_Type(var self) {
  struct Table* t = self;  
  return t->vtype;
}

static void Table_Clear(var self) {
  struct Table* t = self;
  
  for (size_t i = 0; i < t->nslots; i++) {
    if (Table_Key_Hash(t, i) isnt 0) {
      destruct(Table_Key(t, i));
      destruct(Table_Val(t, i));
    }
  }
  
  free(t->data);
  
  t->nslots = 0;
  t->nitems = 0;
  t->data = NULL;
  
}

static void Table_Assign(var self, var obj) {
  struct Table* t = self;  
  Table_Clear(t);
  
  t->ktype = implements_method(obj, Get, key_type) ? key_type(obj) : Ref;
  t->vtype = implements_method(obj, Get, val_type) ? val_type(obj) : Ref;
  t->ksize = Table_Size_Round(size(t->ktype));
  t->vsize = Table_Size_Round(size(t->vtype));
  t->nitems = 0;
  t->nslots = Table_Ideal_Size(len(obj));
  
  if (t->nslots is 0) {
    t->data = NULL;
    return;
  }
  
  t->data = calloc(t->nslots, Table_Step(t));
  t->sspace0 = realloc(t->sspace0, Table_Step(t));
  t->sspace1 = realloc(t->sspace1, Table_Step(t));
  
#if CELLO_MEMORY_CHECK == 1
  if (t->data is NULL or t->sspace0 is NULL or t->sspace1 is NULL) {
    throw(OutOfMemoryError, "Cannot allocate Table, out of memory!");
  }
#endif

  memset(t->sspace0, 0, Table_Step(t));
  memset(t->sspace1, 0, Table_Step(t));
  
  foreach(key in obj) {
    Table_Set_Move(t, key, get(obj, key), false);
  }
  
}

static var Table_Iter_Init(var self);
static var Table_Iter_Next(var self, var curr);

static bool Table_Mem(var self, var key);
static var Table_Get(var self, var key);

static int Table_Cmp(var self, var obj) {
  
  int c;
  var item0 = Table_Iter_Init(self);
  var item1 = iter_init(obj);
  
  while (true) {
    if (item0 is Terminal and item1 is Terminal) { return 0; }
    if (item0 is Terminal) { return -1; }
    if (item1 is Terminal) { return  1; }
    c = cmp(item0, item1);
    if (c < 0) { return -1; }
    if (c > 0) { return  1; }
    c = cmp(Table_Get(self, item0), get(obj, item1));
    if (c < 0) { return -1; }
    if (c > 0) { return  1; }
    item0 = Table_Iter_Next(self, item0);
    item1 = iter_next(obj, item1);
  }
  
  return 0;
  
}

static uint64_t Table_Hash(var self) {
  struct Table* t = self;
  uint64_t h = 0;
  
  var curr = Table_Iter_Init(self);
  while (curr isnt Terminal) {
    var vurr = (char*)curr + t->ksize + sizeof(struct Header);
    h = h ^ hash(curr) ^ hash(vurr);
    curr = Table_Iter_Next(self, curr);
  }
  
  return h;
}

static size_t Table_Len(var self) {
  struct Table* t = self;
  return t->nitems;
}

static uint64_t Table_Swapspace_Hash(struct Table* t, var space) {
  return *((uint64_t*)space);
}

static var Table_Swapspace_Key(struct Table* t, var space) {
  return (char*)space + sizeof(uint64_t) + sizeof(struct Header);
}

static var Table_Swapspace_Val(struct Table* t, var space) {
  return (char*)space + sizeof(uint64_t) + sizeof(struct Header) +
    t->ksize + sizeof(struct Header); 
}

static void Table_Set_Move(var self, var key, var val, bool move) {
  
  struct Table* t = self;
  key = cast(key, t->ktype);
  val = cast(val, t->vtype);
  
  uint64_t i = hash(key) % t->nslots;
  uint64_t j = 0;
  
  memset(t->sspace0, 0, Table_Step(t));
  memset(t->sspace1, 0, Table_Step(t));
  
  if (move) {
      
    uint64_t ihash = i+1;
    memcpy((char*)t->sspace0, &ihash, sizeof(uint64_t));
    memcpy((char*)t->sspace0 + sizeof(uint64_t),
      (char*)key - sizeof(struct Header),
      t->ksize + sizeof(struct Header));
    memcpy((char*)t->sspace0 + sizeof(uint64_t) +
      sizeof(struct Header) + t->ksize, 
      (char*)val - sizeof(struct Header),
      t->vsize + sizeof(struct Header));
  
  } else {
        
    struct Header* khead = (struct Header*)
      ((char*)t->sspace0 + sizeof(uint64_t));
    struct Header* vhead = (struct Header*)
      ((char*)t->sspace0 + sizeof(uint64_t) 
      + sizeof(struct Header) + t->ksize);
    
    header_init(khead, t->ktype, AllocData);
    header_init(vhead, t->vtype, AllocData);
    
    uint64_t ihash = i+1;
    memcpy((char*)t->sspace0, &ihash, sizeof(uint64_t)); 
    assign((char*)t->sspace0 + sizeof(uint64_t) + sizeof(struct Header), key);
    assign((char*)t->sspace0 + sizeof(uint64_t) + sizeof(struct Header)
      + t->ksize + sizeof(struct Header), val);
  }
  
  while (true) {
    
    uint64_t h = Table_Key_Hash(t, i);
    if (h is 0) {
      memcpy((char*)t->data + i * Table_Step(t), t->sspace0, Table_Step(t));
      t->nitems++;
      return;
    }
    
    if (eq(Table_Key(t, i), Table_Swapspace_Key(t, t->sspace0))) {
      destruct(Table_Key(t, i));
      destruct(Table_Val(t, i));
      memcpy((char*)t->data + i * Table_Step(t), t->sspace0, Table_Step(t));
      return;
    }
    
    uint64_t p = Table_Probe(t, i, h);
    if (j >= p) {
      memcpy((char*)t->sspace1, (char*)t->data + i * Table_Step(t), Table_Step(t));
      memcpy((char*)t->data + i * Table_Step(t), (char*)t->sspace0, Table_Step(t));
      memcpy((char*)t->sspace0, (char*)t->sspace1, Table_Step(t));
      j = p;
    }
    
    i = (i+1) % t->nslots;
    j++;
  }
  
}

static void Table_Rehash(struct Table* t, size_t new_size) {
  
  var old_data = t->data;
  size_t old_size = t->nslots;
  
  t->nslots = new_size;
  t->nitems = 0;
  t->data = calloc(t->nslots, Table_Step(t));
  
#if CELLO_MEMORY_CHECK == 1
  if (t->data is NULL) {
    throw(OutOfMemoryError, "Cannot allocate Table, out of memory!");
  }
#endif
  
  for (size_t i = 0; i < old_size; i++) {
    
    uint64_t h = *(uint64_t*)((char*)old_data + i * Table_Step(t));
    
    if (h isnt 0) {
      var key = (char*)old_data + i * Table_Step(t) +
        sizeof(uint64_t) + sizeof(struct Header);
      var val = (char*)old_data + i * Table_Step(t) +
        sizeof(uint64_t) + sizeof(struct Header) + 
        t->ksize + sizeof(struct Header);
      Table_Set_Move(t, key, val, true);
    }
    
  }
  
  free(old_data);
}

static void Table_Resize_More(struct Table* t) {
  size_t new_size = Table_Ideal_Size(t->nitems);  
  size_t old_size = t->nslots;
  if (new_size > old_size) { Table_Rehash(t, new_size); }
}

static void Table_Resize_Less(struct Table* t) {
  size_t new_size = Table_Ideal_Size(t->nitems);  
  size_t old_size = t->nslots;
  if (new_size < old_size) { Table_Rehash(t, new_size); }
}

static bool Table_Mem(var self, var key) {
  struct Table* t = self;
  key = cast(key, t->ktype);
  
  if (t->nslots is 0) { return false; }
  
  uint64_t i = hash(key) % t->nslots;
  uint64_t j = 0;
  
  while (true) {
    
    uint64_t h = Table_Key_Hash(t, i);
    if (h is 0 or j > Table_Probe(t, i, h)) {
      return false;
    }
    
    if (eq(Table_Key(t, i), key)) {
      return true;
    }
    
    i = (i+1) % t->nslots; j++;
  }
  
  return false;
}

static void Table_Rem(var self, var key) {
  struct Table* t = self;
  key = cast(key, t->ktype);
  
  if (t->nslots is 0) {
    throw(KeyError, "Key %$ not in Table!", key);
  }
  
  uint64_t i = hash(key) % t->nslots;
  uint64_t j = 0;
  
  while (true) {
    
    uint64_t h = Table_Key_Hash(t, i);
    if (h is 0 or j > Table_Probe(t, i, h)) {
      throw(KeyError, "Key %$ not in Table!", key);
    }
    
    if (eq(Table_Key(t, i), key)) {
      
      destruct(Table_Key(t, i));
      destruct(Table_Val(t, i));
      memset((char*)t->data + i * Table_Step(t), 0, Table_Step(t));
      
      while (true) {
        
        uint64_t ni = (i+1) % t->nslots;
        uint64_t nh = Table_Key_Hash(t, ni);
        if (nh isnt 0 and Table_Probe(t, ni, nh) > 0) {
          memcpy(
            (char*)t->data + i * Table_Step(t),
            (char*)t->data + ni * Table_Step(t),
            Table_Step(t));
          memset((char*)t->data + ni * Table_Step(t), 0, Table_Step(t));
          i = ni;
        } else {
          break;
        }
        
      }
      
      t->nitems--;
      Table_Resize_Less(t);
      return;
    }
    
    i = (i+1) % t->nslots; j++;
  }
  
}

static var Table_Get(var self, var key) {
  struct Table* t = self;
  
  if (key >= t->data and ((char*)key) < ((char*)t->data) + t->nslots * Table_Step(self)) {
    return Table_Val(self, (((char*)key) - ((char*)t->data)) / Table_Step(self));
  }
  
  key = cast(key, t->ktype);
  
  if (t->nslots is 0) {
    throw(KeyError, "Key %$ not in Table!", key);
  }
  
  uint64_t i = hash(key) % t->nslots;
  uint64_t j = 0;
  
  while (true) {

    uint64_t h = Table_Key_Hash(t, i);
    if (h is 0 or j > Table_Probe(t, i, h)) {
      throw(KeyError, "Key %$ not in Table!", key);
    }
    
    if (eq(Table_Key(t, i), key)) {
      return Table_Val(t, i);
    }
    
    i = (i+1) % t->nslots; j++;
  }
  
  return NULL;
}

static void Table_Set(var self, var key, var val) {
  Table_Set_Move(self, key, val, false);
  Table_Resize_More(self);
}

static var Table_Iter_Init(var self) {
  struct Table* t = self;
  if (t->nitems is 0) { return Terminal; }
  
  for (size_t i = 0; i < t->nslots; i++) {
    if (Table_Key_Hash(t, i) isnt 0) {
      return Table_Key(t, i);
    }
  }
  
  return Terminal;
}

static var Table_Iter_Next(var self, var curr) {
  struct Table* t = self;
  
  curr = (char*)curr + Table_Step(t);
  
  while (true) {

    if (curr > Table_Key(t, t->nslots-1)) { return Terminal;}
    uint64_t h = *(uint64_t*)((char*)curr - 
      sizeof(struct Header) - sizeof(uint64_t));
    if (h isnt 0) { return curr; }
    curr = (char*)curr + Table_Step(t);
  }
  
  return Terminal;
}

static var Table_Iter_Last(var self) {
  
  struct Table* t = self;
  if (t->nitems is 0) { return Terminal; }
  
  size_t i = t->nslots-1;
  while (true) {
    if (Table_Key_Hash(t, i) isnt 0) {
      return Table_Key(t, i);
    }
    if (i == 0) { break; }
    i--;
  }
  
  return Terminal;
  
}

static var Table_Iter_Prev(var self, var curr) {
  struct Table* t = self;
  
  curr = (char*)curr - Table_Step(t);
  
  while (true) {

    if (curr < Table_Key(t, 0)) { return Terminal; }
    uint64_t h = *(uint64_t*)((char*)curr - 
      sizeof(struct Header) - sizeof(uint64_t));
    if (h isnt 0) { return curr; }
    curr = (char*)curr - Table_Step(t);
  }
  
  return Terminal;
}

static var Table_Iter_Type(var self) {
  struct Table* t = self;  
  return t->ktype;
}

static int Table_Show(var self, var output, int pos) {
  struct Table* t = self;
  
  pos = print_to(output, pos, "<'Table' At 0x%p {", self);
  
  size_t j =0;
  for(size_t i = 0; i < t->nslots; i++) {
    if (Table_Key_Hash(t, i) isnt 0) {
      pos = print_to(output, pos, "%$:%$",
        Table_Key(t, i), Table_Val(t, i));
      if (j < Table_Len(t)-1) { pos = print_to(output, pos, ", "); }
      j++;
    }
  }
  
  return print_to(output, pos, "}>");
}

static void Table_Resize(var self, size_t n) {
  struct Table* t = self;
  
  if (n is 0) {
    Table_Clear(t);
    return;
  }
  
#if CELLO_BOUND_CHECK == 1
  if (n < t->nitems) {
    throw(FormatError, 
      "Cannot resize Table to make it smaller than %li items", 
      $I(t->nitems));
  }
#endif
  
  Table_Rehash(t, Table_Ideal_Size(n));
}

static void Table_Mark(var self, var gc, void(*f)(var,void*)) {
  struct Table* t = self;
  for(size_t i = 0; i < t->nslots; i++) {
    if (Table_Key_Hash(t, i) isnt 0) {
      f(gc, Table_Key(t, i));
      f(gc, Table_Val(t, i));
    }
  }
}

var Table = Cello(Table,
  Instance(Doc,
    Table_Name, Table_Brief,    Table_Description,
    NULL,       Table_Examples, NULL),
  Instance(New,      Table_New, Table_Del),
  Instance(Assign,   Table_Assign),
  Instance(Mark,     Table_Mark),
  Instance(Cmp,      Table_Cmp),
  Instance(Hash,     Table_Hash),
  Instance(Len,      Table_Len),
  Instance(Get,
    Table_Get, Table_Set, Table_Mem, Table_Rem, 
    Table_Key_Type, Table_Val_Type),
  Instance(Iter, 
    Table_Iter_Init, Table_Iter_Next, 
    Table_Iter_Last, Table_Iter_Prev, Table_Iter_Type),
  Instance(Show,     Table_Show, NULL),
  Instance(Resize,   Table_Resize));

