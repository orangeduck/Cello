#include "Cello.h"

static const char* Table_Name(void) {
  return "Table";
}

/* TODO */
static const char* Table_Brief(void) {
  return "";
}

/* TODO */
static const char* Table_Description(void) {
  return "";
}

/* TODO */
static const char* Table_Examples(void) {
  return "";
}

/* TODO */
static const char* Table_Methods(void) {
  return "";
}

struct Table {
  var data;
  var ktype;
  var vtype;
  size_t nslots;
  size_t nitems;
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

static const float Table_Load_Factor = 0.9;

static size_t Table_Ideal_Size(size_t size) {
  size = (size+1) / Table_Load_Factor;
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
    sizeof(struct CelloHeader) + size(t->ktype) + 
    sizeof(struct CelloHeader) + size(t->vtype);
}

static uint64_t Table_Key_Hash(struct Table* t, size_t i) {
  return *(uint64_t*)(t->data + i * Table_Step(t));
}

static var Table_Key(struct Table* t, size_t i) {
  return t->data + i * Table_Step(t) + 
    sizeof(uint64_t) + 
    sizeof(struct CelloHeader);  
}

static var Table_Val(struct Table* t, size_t i) {
  return t->data + i * Table_Step(t) + 
    sizeof(uint64_t) + 
    sizeof(struct CelloHeader) + 
    size(t->ktype) + 
    sizeof(struct CelloHeader);  
}

static uint64_t Table_Probe(struct Table* t, uint64_t i, uint64_t h) {
  uint64_t v = i - (h-1);
  if (v < 0) {
    v = t->nslots + v;
  }
  return v;
}

static void Table_Set(var self, var key, var val);
static void Table_Set_Move(var self, var key, var val, var move);

static var Table_New(var self, var args) {
  
  struct Table* t = self;
  t->ktype  = cast(get(args, $(Int, 0)), Type);
  t->vtype  = cast(get(args, $(Int, 1)), Type);
  
  size_t nargs = len(args);
  if (nargs % 2 isnt 0) {
    return throw(FormatError, 
      "Received non multiple of two argument count to Table constructor.");
  }
  
  t->nslots = Table_Ideal_Size((nargs-2)/2);
  t->nitems = 0;
  
  if (t->nslots is 0) {
    t->data = None;
    return self;
  }
  
  t->data = calloc(t->nslots, Table_Step(t));
  
  if (t->data is None) {
    throw(OutOfMemoryError, "Cannot allocate Table, out of memory!");
  }
  
  for(size_t i = 0; i < (nargs-2)/2; i++) {
    var key = get(args, $(Int, 2+(i*2)+0));
    var val = get(args, $(Int, 2+(i*2)+1));
    Table_Set_Move(t, key, val, False);
  }
  
  return self;
}

static var Table_Del(var self) {
  struct Table* t = self;  
  
  for (size_t i = 0; i < t->nslots; i++) {
    if (Table_Key_Hash(t, i) isnt 0) {
      destruct(Table_Key(t, i));
      destruct(Table_Val(t, i));
    }
  }
  
  free(t->data);
  return self;
}

static size_t Table_Size(void) {
  return sizeof(struct Table);
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
  t->data = None;
  
}

static var Table_Assign(var self, var obj) {
  struct Table* t = self;  
  Table_Clear(t);
  
  t->nitems = 0;
  t->nslots = Table_Ideal_Size(len(obj));
  
  if (t->nslots is 0) {
    t->data = None;
    return self;
  }
  
  t->data = calloc(t->nslots, Table_Step(t));
  
  if (t->data is None) {
    throw(OutOfMemoryError, "Cannot allocate Table, out of memory!");
  }
  
  foreach(key in obj) {
    Table_Set_Move(t, key, get(obj, key), False);
  }
  
  return self;
  
}

static var Table_Copy(var self) {
  struct Table* t = self;
  
  var r = new(Table, t->ktype, t->vtype);
  
  for (size_t i = 0; i < t->nslots; i++) {
    if (Table_Key_Hash(t, i) isnt 0) {
      Table_Set(r, Table_Key(t, i), Table_Val(t, i));
    }
  }
  
  return r;
}

static var Table_Eq(var self, var obj) {
  
  foreach (key in obj) {
    if (not mem(self, key)) { return False; }
    if_neq(get(obj, key), get(self, key)) { return False; }
  }
	
  foreach (key in self) {
    if (not mem(obj, key)) { return False; }
    if_neq(get(obj, key), get(self, key)) { return False; }
  }
	
  return True;
}

static size_t Table_Len(var self) {
  struct Table* t = self;
  return t->nitems;
}

static uint64_t Table_Swapspace_Hash(struct Table* t, var space) {
  return *((uint64_t*)space);
}

static var Table_Swapspace_Key(struct Table* t, var space) {
  return space + sizeof(uint64_t) + sizeof(struct CelloHeader);
}

static var Table_Swapspace_Val(struct Table* t, var space) {
  return space + sizeof(uint64_t) + sizeof(struct CelloHeader) +
    size(t->ktype) + sizeof(struct CelloHeader); 
}

static void Table_Debug(struct Table* t) {
  
  for (size_t i = 0; i < t->nslots; i++) {
    
    uint64_t h = Table_Key_Hash(t, i);
    uint64_t p = Table_Probe(t, i, h);
    if (h is 0) {
      print("| %i \n", $(Int, i));  
    } else {
      print("| %i [%i (%i)] %$\n", 
        $(Int, i), $(Int, h-1), $(Int, p), Table_Key(t, i));
    }
    
  } 
  
}

static void Table_Set_Move(var self, var key, var val, var move) {
  
  struct Table* t = self;
  key = cast(key, t->ktype);
  val = cast(val, t->vtype);
  
  uint64_t i = hash(key) % t->nslots;
  uint64_t j = 0;
  
  char sspace0[Table_Step(t)];
  char sspace1[Table_Step(t)];
  
  memset(sspace0, 0, Table_Step(t));
  memset(sspace1, 0, Table_Step(t));
  
  if (move) {
      
    uint64_t ihash = i+1;
    memcpy(sspace0, &ihash, sizeof(uint64_t));
    memcpy(sspace0 + sizeof(uint64_t), 
      key - sizeof(struct CelloHeader), 
      size(t->ktype) + sizeof(struct CelloHeader));
    memcpy(sspace0 + sizeof(uint64_t) + 
      sizeof(struct CelloHeader) + size(t->ktype), 
      val - sizeof(struct CelloHeader), 
      size(t->vtype) + sizeof(struct CelloHeader));
  
  } else {
        
    struct CelloHeader* khead = (struct CelloHeader*)
      (sspace0 + sizeof(uint64_t));
    struct CelloHeader* vhead = (struct CelloHeader*)
      (sspace0 + sizeof(uint64_t) 
      + sizeof(struct CelloHeader) + size(t->ktype));
      
    khead->type = t->ktype;
    vhead->type = t->vtype;
    khead->flags = (var)CelloDataAlloc;
    vhead->flags = (var)CelloDataAlloc;
    
    uint64_t ihash = i+1;
    memcpy(sspace0, &ihash, sizeof(uint64_t)); 
    assign(sspace0 + sizeof(uint64_t) + sizeof(struct CelloHeader), key);
    assign(sspace0 + sizeof(uint64_t) + sizeof(struct CelloHeader)
      + size(t->ktype) + sizeof(struct CelloHeader), val);
  }
  
  while (True) {
    
    uint64_t h = Table_Key_Hash(t, i);
    if (h is 0) {
      memcpy(t->data + i * Table_Step(t), sspace0, Table_Step(t));
      t->nitems++;
      return;
    }
    
    if_eq(Table_Key(t, i), Table_Swapspace_Key(t, sspace0)) {
      destruct(Table_Key(t, i));
      destruct(Table_Val(t, i));
      memcpy(t->data + i * Table_Step(t), sspace0, Table_Step(t));
      return;
    }
    
    uint64_t p = Table_Probe(t, i, h);
    if (j >= p) {
      memcpy(sspace1, t->data + i * Table_Step(t), Table_Step(t));
      memcpy(t->data + i * Table_Step(t), sspace0, Table_Step(t));
      memcpy(sspace0, sspace1, Table_Step(t));
      j = p;
    }
    
    i = (i+1) % t->nslots;
    j++;
  }
  
}

static void Table_Rehash(struct Table* t) {
  
  size_t new_size = Table_Ideal_Size(t->nitems);  
  size_t old_size = t->nslots;
  var    old_data = t->data;
  
  if (new_size == old_size) { return; }
  
  t->nslots = new_size;
  t->nitems = 0;
  t->data = calloc(t->nslots, Table_Step(t));
  
  for (size_t i = 0; i < old_size; i++) {
    
    uint64_t h = *(uint64_t*)(old_data + i * Table_Step(t));
    
    if (h isnt 0) {
      var key = old_data + i * Table_Step(t) + 
        sizeof(uint64_t) + sizeof(struct CelloHeader);
      var val = old_data + i * Table_Step(t) + 
        sizeof(uint64_t) + sizeof(struct CelloHeader) + 
        size(t->ktype) + sizeof(struct CelloHeader);
      Table_Set_Move(t, key, val, True);
    }
    
  }
  
  free(old_data);
}

static var Table_Mem(var self, var key) {
  struct Table* t = self;
  
  if (t->nslots is 0) { return False; }
  
  uint64_t i = hash(key) % t->nslots;
  uint64_t j = 0;
  
  while (True) {
    
    uint64_t h = Table_Key_Hash(t, i);
    if (h is 0 or j > Table_Probe(t, i, h)) {
      return False;
    }
    
    if_eq(Table_Key(t, i), key) {
      return True;
    }
    
    i = (i+1) % t->nslots; j++;
  }
  
  return False;
}

static void Table_Rem(var self, var key) {
  struct Table* t = self;
  key = cast(key, t->ktype);
  
  if (t->nslots is 0) {
    throw(KeyError, "Key %$ not in Table!", key);
  }
  
  uint64_t i = hash(key) % t->nslots;
  uint64_t j = 0;
  
  while (True) {
    
    uint64_t h = Table_Key_Hash(t, i);
    if (h is 0 or j > Table_Probe(t, i, h)) {
      throw(KeyError, "Key %$ not in Table!", key);
    }
    
    if_eq(Table_Key(t, i), key) {
      
      destruct(Table_Key(t, i));
      destruct(Table_Val(t, i));
      memset(t->data + i * Table_Step(t), 0, Table_Step(t));
      
      while (True) {
        
        uint64_t ni = (i+1) % t->nslots;
        uint64_t nh = Table_Key_Hash(t, ni);
        if (nh isnt 0 and Table_Probe(t, ni, nh) > 0) {
          memcpy(
            t->data +  i * Table_Step(t),
            t->data + ni * Table_Step(t),
            Table_Step(t));
          memset(t->data + ni * Table_Step(t), 0, Table_Step(t));
          i = ni;
        } else {
          break;
        }
        
      }
      
      t->nitems--;
      Table_Rehash(t);
      return;
    }
    
    i = (i+1) % t->nslots; j++;
  }
  
}

static var Table_Get(var self, var key) {
  struct Table* t = self;
  key = cast(key, t->ktype);
  
  if (t->nslots is 0) {
    throw(KeyError, "Key %$ not in Table!", key);
  }
  
  uint64_t i = hash(key) % t->nslots;
  uint64_t j = 0;
  
  while (True) {

    uint64_t h = Table_Key_Hash(t, i);
    if (h is 0 or j > Table_Probe(t, i, h)) {
      throw(KeyError, "Key %$ not in Table!", key);
    }
    
    if_eq(Table_Key(t, i), key) {
      return Table_Val(t, i);
    }
    
    i = (i+1) % t->nslots; j++;
  }
  
  return Undefined;
}

static void Table_Set(var self, var key, var val) {
  Table_Set_Move(self, key, val, False);
  Table_Rehash(self);
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
  
  curr += Table_Step(t);
  
  while (True) {

    if (curr > Table_Key(t, t->nslots-1)) {
      return Terminal;
    }

    uint64_t h = *(uint64_t*)(curr - sizeof(struct CelloHeader) - sizeof(uint64_t));
    if (h isnt 0) { return curr; }
    
    curr += Table_Step(t);
  }
  
  return Terminal;
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
  
  pos = print_to(output, pos, "}>");
  
  return pos;
}

var Table = typedecl(Table,
  typeclass(Doc,
    Table_Name, Table_Brief, Table_Description,
    Table_Examples, Table_Methods),
  typeclass(New, Table_New, Table_Del, Table_Size),
  typeclass(Assign, Table_Assign),
  typeclass(Copy, Table_Copy),
  typeclass(Eq, Table_Eq),
  typeclass(Len, Table_Len),
  typeclass(Get, Table_Get, Table_Set, Table_Mem, Table_Rem),
  typeclass(Clear, Table_Clear),
  typeclass(Iter, Table_Iter_Init, Table_Iter_Next),
  typeclass(Show, Table_Show, NULL));

