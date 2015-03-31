#include "Cello.h"

static const char* Mark_Name(void) {
  return "Mark";
}

/* TODO */
static const char* Mark_Brief(void) {
  return "";
}

/* TODO */
static const char* Mark_Description(void) {
  return "";
}

/* TODO */
static const char* Mark_Examples(void) {
  return "";
}

/* TODO */
static const char* Mark_Methods(void) {
  return "";
}

var Mark = Cello(Mark, Instance(Doc, 
  Mark_Name, Mark_Brief, Mark_Description, 
  Mark_Examples, Mark_Methods));
  
#if CELLO_GC == 1

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

/* TODO */
static const char* GC_Description(void) {
  return "";
}

/* TODO */
static const char* GC_Examples(void) {
  return "";
}

/* TODO */
static const char* GC_Methods(void) {
  return "";
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
};

static uint64_t GC_Probe(struct GC* gc, uint64_t i, uint64_t h) {
  uint64_t v = i - (h-1);
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

static void GC_Set(struct GC* gc, var ptr, bool root);

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
      GC_Set(gc, old_entries[i].ptr, old_entries[i].root);
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

static void GC_Set(struct GC* gc, var ptr, bool root) {
  
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

static void GC_Rem(struct GC* gc, var ptr) {
  
  if (gc->nslots is 0) { return; }
  
  uint64_t i = GC_Hash(ptr) % gc->nslots;
  uint64_t j = 0;
  
  while (true) {
    
    uint64_t h = gc->entries[i].hash;
    if (h is 0 or j > GC_Probe(gc, i, h)) { return; }
    if (gc->entries[i].ptr == ptr) {
      
      memset(&gc->entries[i], 0, sizeof(struct GCEntry));
      
      while (true) {
        
        uint64_t ni = (i+1) % gc->nslots;
        uint64_t nh = gc->entries[ni].hash;
        if (nh isnt 0 and GC_Probe(gc, ni, nh) > 0) {
          memcpy(&gc->entries[i], &gc->entries[ni], sizeof(struct GCEntry));
          memset(&gc->entries[ni], 0, sizeof(struct GCEntry));
          i = ni;
        } else {
          break;
        }
        
      }
      
      gc->nitems--;
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
  
  struct Size* s = type_instance(type, Size);
  if (s and s->size) {
    for (size_t i = 0; i < s->size(); i += sizeof(var)) {
      var p = ((char*)ptr) + i;
      GC_Mark_Item(gc, *((var*)p));
    }
    return;
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
  
  /* TODO: Mark Thread Local Storage */
  
  if (gc is NULL or gc->nitems is 0) { return; }
  
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

  mark_stack(gc);
  
}

static void GC_Print(struct GC* gc) {
 
  printf("| GC TABLE\n");
  for (size_t i = 0; i < gc->nslots; i++) {
    if (gc->entries[i].hash is 0) { printf("| %i : ---\n", (int)i); continue; }
    printf("| %i : %p %i %i\n", 
      (int)i, gc->entries[i].ptr, 
      (int)gc->entries[i].root,
      (int)gc->entries[i].marked);
  }
  printf("|======\n");
  
}

void GC_Sweep(struct GC* gc) {
   
  var* freelist = malloc(sizeof(var) * gc->nitems);
  size_t freenum = 0;
  
  size_t i = 0;
  while (i < gc->nslots) {
    
    if (gc->entries[i].hash is 0) { i++; continue; }
    if (gc->entries[i].marked) { i++; continue; }
    
    if (not gc->entries[i].root and not gc->entries[i].marked) {
      
      freelist[freenum] = gc->entries[i].ptr;
      freenum++;
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
  
  for (size_t i = 0; i < freenum; i++) {
    dealloc(destruct(freelist[i]));
  }
  
  free(freelist);
  
}

static var GC_Current(void) {
  return get(current(Thread), $S(GC_TLS_KEY));
}

static void GC_New(var self, var args) {
  
}

static void GC_Del(var self) {
  
}

var GC = Cello(GC,
  Instance(Doc, GC_Name, GC_Brief, GC_Description, GC_Examples, GC_Methods),
  Instance(Current, GC_Current));

void gc_finish(void) {
  struct GC* gc = current(GC);
  GC_Sweep(gc);
  free(gc->entries);
  dealloc_raw(gc);
}

void gc_init(var bottom) {
  struct GC* gc = alloc_raw(GC);
  gc->bottom = bottom;
  gc->maxptr = 0;
  gc->minptr = UINTPTR_MAX;
  set(current(Thread), $S(GC_TLS_KEY), $R(gc));
}

void gc_add(var ptr, bool root) {
  struct GC* gc = current(GC);
  gc->nitems++;
  gc->maxptr = (uintptr_t)ptr > gc->maxptr ? (uintptr_t)ptr : gc->maxptr;
  gc->minptr = (uintptr_t)ptr < gc->minptr ? (uintptr_t)ptr : gc->minptr;
  GC_Resize_More(gc);
  GC_Set(gc, ptr, root);
  if (gc->nitems > gc->mitems) {
    GC_Mark(gc);
    GC_Sweep(gc);
  }
  //GC_Mark(gc);
  //GC_Sweep(gc);
}

void gc_rem(var ptr) {
  struct GC* gc = current(GC);
  GC_Rem(gc, ptr);
  GC_Resize_Less(gc);
  gc->mitems = gc->nitems + gc->nitems / 2 + 1;
}

void gc_run(void) {
  struct GC* gc = current(GC);
  GC_Mark(gc);
  GC_Sweep(gc);
}

#endif
