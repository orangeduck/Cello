#include "Cello.h"

static const char* Mark_Name(void) {
  return "Mark";
}

static const char* Mark_Brief(void) {
  return "Markable by GC";
}

static const char* Mark_Description(void) {
  return
    "The `Mark` class can be overridden to customize the behaviour of the "
    "Cello Garbage Collector on encountering a given type. By default the "
    "allocated memory for a structure is scanned for pointers to other Cello "
    "objects, but if a type does its own memory allocation it may store "
    "pointers to Cello objects in other locations."
    "\n\n"
    "If this is the case the `Mark` class can be overridden and the callback "
    "function `f` must be called on all pointers which might be Cello objects "
    "which are managed by the class. Alternately the `mark` function can be "
    "called on any sub object to start a chain of recursive marking.";
}

static const char* Mark_Definition(void) {
  return
    "struct Mark {\n"
    "  void (*mark)(var, var, void(*)(var,void*));\n"
    "};\n";
}

static struct Method* Mark_Methods(void) {
  
  static struct Method methods[] = {
    {
      "mark", 
      "void mark(var self, var gc, void(*f)(var,void*));",
      "Mark the object `self` with the Garbage Collector `gc` and the callback "
      "function `f`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Mark = Cello(Mark, Instance(Doc, 
  Mark_Name,       Mark_Brief, Mark_Description, 
  Mark_Definition, NULL,       Mark_Methods));
  
void mark(var self, var gc, void(*f)(var,void*)) {
  if (self is NULL) { return; }
  struct Mark* m = instance(self, Mark);
  if (m and m->mark) { m->mark(self, gc, f); }
}

#ifndef CELLO_NGC
  
#define GC_TLS_KEY "__GC"

enum {
  GC_PRIMES_COUNT = 24
};

static const size_t GC_Primes[GC_PRIMES_COUNT] = {
  0,       1,       5,       11,
  23,      53,      101,     197,
  389,     683,     1259,    2417,
  4733,    9371,    18617,   37097,
  74093,   148073,  296099,  592019,
  1100009, 2200013, 4400021, 8800019
};

static const char* GC_Name(void) {
  return "GC";
}

static const char* GC_Brief(void) {
  return "Garbage Collector";
}

static const char* GC_Description(void) {
  return
    "The `GC` type provides an interface to the Cello Garbage Collector. One "
    "instance of this type is created for each thread and can be retrieved "
    "using the `current` function. The Garbage Collector can be stopped and "
    "started using `start` and `stop` and objects can be added or removed from "
    "the Garbage Collector using `set` and `rem`.";
}

static struct Example* GC_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Starting & Stopping",
      "var gc = current(GC);\n"
      "stop(gc);\n"
      "var x = new(Int, $I(10)); /* Not added to GC */\n"
      "show($I(running(gc))); /* 0 */\n"
      "del(x); /* Must be deleted when done */\n"
      "start(gc);\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

struct GCEntry {
  var ptr;
  uint64_t hash;
  bool root;
  bool marked;
};

struct GC {
  struct GCEntry* entries;
  size_t nslots;
  size_t nitems;
  size_t mitems;
  uintptr_t maxptr;
  uintptr_t minptr;
  var bottom;
  bool running;
  uintptr_t freenum;
  var* freelist;
};

static uint64_t GC_Probe(struct GC* gc, uint64_t i, uint64_t h) {
  int64_t v = i - (h-1);
  if (v < 0) {
    v = gc->nslots + v;
  }
  return v;
}

static const double GC_Load_Factor = 0.9;

static size_t GC_Ideal_Size(size_t size) {
  size = (size_t)((double)(size+1) / GC_Load_Factor);
  for (size_t i = 0; i < GC_PRIMES_COUNT; i++) {
    if (GC_Primes[i] >= size) { return GC_Primes[i]; }
  }
  size_t last = GC_Primes[GC_PRIMES_COUNT-1];
  for (size_t i = 0;; i++) {
    if (last * i >= size) { return last * i; }
  }
}

static void GC_Set_Ptr(struct GC* gc, var ptr, bool root);

static void GC_Rehash(struct GC* gc, size_t new_size) {

  struct GCEntry* old_entries = gc->entries;
  size_t old_size = gc->nslots;
  
  gc->nslots = new_size;
  gc->entries = calloc(gc->nslots, sizeof(struct GCEntry));
  
#if CELLO_MEMORY_CHECK == 1
  if (gc->entries is NULL) {
    throw(OutOfMemoryError, "Cannot allocate GC Pointer Table, out of memory!");
    return;
  }
#endif
  
  for (size_t i = 0; i < old_size; i++) {
    if (old_entries[i].hash isnt 0) {
      GC_Set_Ptr(gc, old_entries[i].ptr, old_entries[i].root);
    }
  }
  
  free(old_entries);

}

static void GC_Resize_More(struct GC* gc) {
  size_t new_size = GC_Ideal_Size(gc->nitems);  
  size_t old_size = gc->nslots;
  if (new_size > old_size) { GC_Rehash(gc, new_size); }
}

static void GC_Resize_Less(struct GC* gc) {
  size_t new_size = GC_Ideal_Size(gc->nitems);  
  size_t old_size = gc->nslots;
  if (new_size < old_size) { GC_Rehash(gc, new_size); }
}

static uint64_t GC_Hash(var ptr) {
  return ((uintptr_t)ptr) >> 3;
}

static void GC_Set_Ptr(struct GC* gc, var ptr, bool root) {
  
  uint64_t i = GC_Hash(ptr) % gc->nslots;
  uint64_t j = 0;
  uint64_t ihash = i+1;
  struct GCEntry entry = { ptr, ihash, root, 0 };
  
  while (true) {
    
    uint64_t h = gc->entries[i].hash;
    if (h is 0) { gc->entries[i] = entry; return; }
    if (gc->entries[i].ptr == entry.ptr) { return; }
    
    uint64_t p = GC_Probe(gc, i, h);
    if (j >= p) {
      struct GCEntry tmp = gc->entries[i];
      gc->entries[i] = entry;
      entry = tmp;
      j = p;
    }
    
    i = (i+1) % gc->nslots;
    j++;
  }
  
}

static bool GC_Mem_Ptr(struct GC* gc, var ptr) {

  if (gc->nslots is 0) { return false; }
  
  uint64_t i = GC_Hash(ptr) % gc->nslots;
  uint64_t j = 0;
  
  while (true) {
    uint64_t h = gc->entries[i].hash;
    if (h is 0 or j > GC_Probe(gc, i, h)) { return false; }
    if (gc->entries[i].ptr == ptr) { return true; }
    i = (i+1) % gc->nslots; j++;
  }

}

static void GC_Rem_Ptr(struct GC* gc, var ptr) {
  
  if (gc->nslots is 0) { return; }
  
  for (size_t i = 0; i < gc->freenum; i++) {
    if (gc->freelist[i] is ptr) { gc->freelist[i] = NULL; }
  }
  
  uint64_t i = GC_Hash(ptr) % gc->nslots;
  uint64_t j = 0;
  
  while (true) {
    
    uint64_t h = gc->entries[i].hash;
    if (h is 0 or j > GC_Probe(gc, i, h)) { return; }
    if (gc->entries[i].ptr is ptr) {
      
      var freeitem = gc->entries[i].ptr;
      memset(&gc->entries[i], 0, sizeof(struct GCEntry));
      
      j = i;
      while (true) { 
        uint64_t nj = (j+1) % gc->nslots;
        uint64_t nh = gc->entries[nj].hash;
        if (nh isnt 0 and GC_Probe(gc, nj, nh) > 0) {
          memcpy(&gc->entries[j], &gc->entries[nj], sizeof(struct GCEntry));
          memset(&gc->entries[nj], 0, sizeof(struct GCEntry));
          j = nj;
        } else {
          break;
        }  
      }
      
      gc->nitems--;
      
      dealloc(destruct(freeitem));
      return;
    }
    
    i = (i+1) % gc->nslots; j++;
  }
  
}

static void GC_Mark_Item(struct GC* gc, void* ptr);

static void GC_Recurse(struct GC* gc, var ptr) {
  
  var type = type_of(ptr);
  
  if (type is Int    or  type is Float   
  or  type is String or  type is Type
  or  type is File   or  type is Process
  or  type is Function) { return; }
    
  struct Mark* m = type_instance(type, Mark);
  if (m and m->mark) {
    m->mark(ptr, gc, (void(*)(var,void*))GC_Mark_Item);
    return;
  }
    
  for (size_t i = 0; i+sizeof(var) <= size(type); i += sizeof(var)) {
    var p = ((char*)ptr) + i;
    GC_Mark_Item(gc, *((var*)p));
  }
  
}

static void GC_Print(struct GC* gc);

static void GC_Mark_Item(struct GC* gc, void* ptr) {
  
  uintptr_t pval = (uintptr_t)ptr;
  if (pval % sizeof(var) isnt 0
  or  pval < gc->minptr
  or  pval > gc->maxptr) { return; }
  
  uint64_t i = GC_Hash(ptr) % gc->nslots;
  uint64_t j = 0;
  
  while (true) {
    
    uint64_t h = gc->entries[i].hash;
    
    if (h is 0 or j > GC_Probe(gc, i, h)) { return; }
    
    if (gc->entries[i].ptr is ptr and not gc->entries[i].marked) {
      gc->entries[i].marked = true;
      GC_Recurse(gc, gc->entries[i].ptr);
      return;
    }
    
    i = (i+1) % gc->nslots; j++;
  }
  
}

static void GC_Mark_Stack(struct GC* gc) {
  
  var stk = NULL;
  var bot = gc->bottom;
  var top = &stk;
  
  if (bot == top) { return; }
  
  if (bot < top) {
    for (var p = top; p >= bot; p = ((char*)p) - sizeof(var)) {
      GC_Mark_Item(gc, *((var*)p));
    }
  }
  
  if (bot > top) {
    for (var p = top; p <= bot; p = ((char*)p) + sizeof(var)) {
      GC_Mark_Item(gc, *((var*)p));
    }
  }
  
}

static void GC_Mark_Stack_Fake(struct GC* gc) { }

void GC_Mark(struct GC* gc) {
  
  if (gc is NULL or gc->nitems is 0) { return; }
  
  /* Mark Thread Local Storage */
  mark(current(Thread), gc, (void(*)(var,void*))GC_Mark_Item);
  
  /* Mark Roots */
  for (size_t i = 0; i < gc->nslots; i++) {
    if (gc->entries[i].hash is 0) { continue; }
    if (gc->entries[i].marked) { continue; }
    if (gc->entries[i].root) {
      gc->entries[i].marked = true;
      GC_Recurse(gc, gc->entries[i].ptr);
    }
  }
  
  volatile int noinline = 1;
  
  /* Flush Registers to Stack */
  if (noinline) {
    jmp_buf env;
    memset(&env, 0, sizeof(jmp_buf));
    setjmp(env);
  }
  
  /* Avoid Inlining function call */
  void (*mark_stack)(struct GC* gc) = noinline
    ? GC_Mark_Stack
    : (void(*)(struct GC* gc))(NULL);
  
  /* Mark Stack */
  mark_stack(gc);
  
}

static int GC_Show(var self, var out, int pos) {
  struct GC* gc = self;
 
  pos = print_to(out, pos, "<'GC' At 0x%p\n", self);
  for (size_t i = 0; i < gc->nslots; i++) {
    if (gc->entries[i].hash is 0) {
      pos = print_to(out, pos, "| %i : \n", $I(i));
      continue;
    }
    pos = print_to(out, pos, "| %i : %15s %p %s %s\n", 
      $I(i), type_of(gc->entries[i].ptr), 
      gc->entries[i].ptr, 
      gc->entries[i].root ? $S("root") : $S("auto"),
      gc->entries[i].marked ? $S("*") : $S(" "));
  }
  
  return print_to(out, pos, "+------------------->\n");
}

void GC_Sweep(struct GC* gc) {
   
  gc->freelist = realloc(gc->freelist, sizeof(var) * gc->nitems);
  gc->freenum = 0;
  
  size_t i = 0;
  while (i < gc->nslots) {
    
    if (gc->entries[i].hash is 0) { i++; continue; }
    if (gc->entries[i].marked) { i++; continue; }
    
    if (not gc->entries[i].root and not gc->entries[i].marked) {
      
      gc->freelist[gc->freenum] = gc->entries[i].ptr;
      gc->freenum++;
      memset(&gc->entries[i], 0, sizeof(struct GCEntry));
      
      uint64_t j = i;
      while (true) { 
        uint64_t nj = (j+1) % gc->nslots;
        uint64_t nh = gc->entries[nj].hash;
        if (nh isnt 0 and GC_Probe(gc, nj, nh) > 0) {
          memcpy(&gc->entries[j], &gc->entries[nj], sizeof(struct GCEntry));
          memset(&gc->entries[nj], 0, sizeof(struct GCEntry));
          j = nj;
        } else {
          break;
        }  
      }
      
      gc->nitems--;
      continue;
    }
    
    i++;
  }
  
  for (size_t i = 0; i < gc->nslots; i++) {
    if (gc->entries[i].hash is 0) { continue; }
    if (gc->entries[i].marked) {
      gc->entries[i].marked = false;
      continue;
    }
  }
  
  GC_Resize_Less(gc);
  gc->mitems = gc->nitems + gc->nitems / 2 + 1;
  
  for (size_t i = 0; i < gc->freenum; i++) {
    if (gc->freelist[i]) {
      dealloc(destruct(gc->freelist[i]));
    }
  }
  
  free(gc->freelist);
  gc->freelist = NULL;
  gc->freenum = 0;
  
}

static var GC_Current(void) {
  return get(current(Thread), $S(GC_TLS_KEY));
}

static void GC_New(var self, var args) {
  struct GC* gc = self;
  struct Ref* bt = cast(get(args, $I(0)), Ref);
  gc->bottom = bt->val;
  gc->maxptr = 0;
  gc->minptr = UINTPTR_MAX;
  gc->running = true;
  gc->freelist = NULL;
  gc->freenum = 0;
  set(current(Thread), $S(GC_TLS_KEY), gc);
}

static void GC_Del(var self) {
  struct GC* gc = self;
  GC_Sweep(gc);
  free(gc->entries);
  free(gc->freelist);
  rem(current(Thread), $S(GC_TLS_KEY));
}

static void GC_Set(var self, var key, var val) {
  struct GC* gc = self;
  if (not gc->running) { return; }
  gc->nitems++;
  gc->maxptr = (uintptr_t)key > gc->maxptr ? (uintptr_t)key : gc->maxptr;
  gc->minptr = (uintptr_t)key < gc->minptr ? (uintptr_t)key : gc->minptr;
  GC_Resize_More(gc);
  GC_Set_Ptr(gc, key, (bool)c_int(val));
  if (gc->nitems > gc->mitems) {
    GC_Mark(gc);
    GC_Sweep(gc);
  }
}

static void GC_Rem(var self, var key) {
  struct GC* gc = self;
  if (not gc->running) { return; }
  GC_Rem_Ptr(gc, key);
  GC_Resize_Less(gc);
  gc->mitems = gc->nitems + gc->nitems / 2 + 1;
}

static bool GC_Mem(var self, var key) {
  return GC_Mem_Ptr(self, key);
}

static void GC_Start(var self) {
  struct GC* gc = self;
  gc->running = true;
}

static void GC_Stop(var self) {
  struct GC* gc = self;
  gc->running = false;
}

static bool GC_Running(var self) {
  struct GC* gc = self;
  return gc->running;
}

var GC = Cello(GC,
  Instance(Doc,
    GC_Name, GC_Brief,    GC_Description, 
    NULL,    GC_Examples, NULL),
  Instance(New,     GC_New, GC_Del),
  Instance(Get,     NULL, GC_Set, GC_Mem, GC_Rem),
  Instance(Start,   GC_Start, GC_Stop, NULL, GC_Running),
  Instance(Show,    GC_Show, NULL),
  Instance(Current, GC_Current));

void Cello_Exit(void) {
  del_raw(current(GC));
}

#endif
