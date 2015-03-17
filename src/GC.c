#include "Cello.h"

#if CELLO_GC == 1

enum {
  GCTAB_PRIMES_COUNT = 24
};

static const size_t GCTab_Primes[GCTAB_PRIMES_COUNT] = {
  0,       1,       5,       11,
  23,      53,      101,     197,
  389,     683,     1259,    2417,
  4733,    9371,    18617,   37097,
  74093,   148073,  296099,  592019,
  1100009, 2200013, 4400021, 8800019
};

struct GCEntry {
  var ptr;
  uint64_t hash;
  uint64_t flags;
};

struct GCTab {
  struct GCEntry* entries;
  size_t nslots;
  size_t nitems;
  size_t mitems;
  uintptr_t maxptr;
  uintptr_t minptr;
  var bottom;
};

static uint64_t GCTab_Probe(struct GCTab* t, uint64_t i, uint64_t h) {
  uint64_t v = i - (h-1);
  if (v < 0) {
    v = t->nslots + v;
  }
  return v;
}

static const double GCTab_Load_Factor = 0.9;

static size_t GCTab_Ideal_Size(size_t size) {
  size = (size_t)((double)(size+1) / GCTab_Load_Factor);
  for (size_t i = 0; i < GCTAB_PRIMES_COUNT; i++) {
    if (GCTab_Primes[i] >= size) { return GCTab_Primes[i]; }
  }
  size_t last = GCTab_Primes[GCTAB_PRIMES_COUNT-1];
  for (size_t i = 0;; i++) {
    if (last * i >= size) { return last * i; }
  }
}

static void GCTab_Set(struct GCTab* t, var ptr, uint64_t flags);

static void GCTab_Rehash(struct GCTab* t, size_t new_size) {

  struct GCEntry* old_entries = t->entries;
  size_t old_size = t->nslots;
  
  t->nslots = new_size;
  t->entries = calloc(t->nslots, sizeof(struct GCEntry));
  
#if CELLO_MEMORY_CHECK == 1
  if (t->entries is None) {
    throw(OutOfMemoryError, "Cannot allocate GC Pointer Table, out of memory!");
    return;
  }
#endif
  
  for (size_t i = 0; i < old_size; i++) {
    if (old_entries[i].hash isnt 0) {
      GCTab_Set(t, old_entries[i].ptr, old_entries[i].flags);
    }
  }
  
  free(old_entries);

}

static void GCTab_Resize_More(struct GCTab* t) {
  size_t new_size = GCTab_Ideal_Size(t->nitems);  
  size_t old_size = t->nslots;
  if (new_size > old_size) { GCTab_Rehash(t, new_size); }
}

static void GCTab_Resize_Less(struct GCTab* t) {
  size_t new_size = GCTab_Ideal_Size(t->nitems);  
  size_t old_size = t->nslots;
  if (new_size < old_size) { GCTab_Rehash(t, new_size); }
}

static uint64_t GCTab_Hash(var ptr) {
  return ((uintptr_t)ptr) >> 3;
}

static void GCTab_Set(struct GCTab* t, var ptr, uint64_t flags) {
  
  uint64_t i = GCTab_Hash(ptr) % t->nslots;
  uint64_t j = 0;
  uint64_t ihash = i+1;
  struct GCEntry entry = { ptr, ihash, flags };
  
  while (True) {
    
    uint64_t h = t->entries[i].hash;
    if (h is 0) {
      t->entries[i] = entry;
      return;
    }
    
    if (t->entries[i].ptr == entry.ptr) {
      /* TODO: Error */
      fprintf(stderr, "GC ERROR: KEY %p ALREADY IN TABLE\n", entry.ptr);
      abort();
      return;
    }
    
    uint64_t p = GCTab_Probe(t, i, h);
    if (j >= p) {
      struct GCEntry tmp = t->entries[i];
      t->entries[i] = entry;
      entry = tmp;
      j = p;
    }
    
    i = (i+1) % t->nslots;
    j++;
  }
  
}

static void GCTab_Rem(struct GCTab* t, var ptr) {
  
  if (t->nslots is 0) { return; }
  
  uint64_t i = GCTab_Hash(ptr) % t->nslots;
  uint64_t j = 0;
  
  while (True) {
    
    uint64_t h = t->entries[i].hash;
    if (h is 0 or j > GCTab_Probe(t, i, h)) { return; }
    if (t->entries[i].ptr == ptr) {
      
      memset(&t->entries[i], 0, sizeof(struct GCEntry));
      
      while (True) {
        
        uint64_t ni = (i+1) % t->nslots;
        uint64_t nh = t->entries[ni].hash;
        if (nh isnt 0 and GCTab_Probe(t, ni, nh) > 0) {
          memcpy(&t->entries[i], &t->entries[ni], sizeof(struct GCEntry));
          memset(&t->entries[ni], 0, sizeof(struct GCEntry));
          i = ni;
        } else {
          break;
        }
        
      }
      
      t->nitems--;
      return;
    }
    
    i = (i+1) % t->nslots; j++;
  }
  
}

static var Cello_GC_Mark_Item(var ptr);

static void Cello_GC_Recurse(var ptr) {
  
  var type = type_of(ptr);
  
  if (type is Bool    or type is Int 
  or  type is Float   or type is String
  or  type is Type    or type is File 
  or  type is Process or type is Function) { return; }
  
  if (type_implements(type, Traverse)) {
    traverse(ptr, $(Function, Cello_GC_Mark_Item));
    return;
  }
  
  if (type_implements(type, Size)) {
    for (size_t s = 0; s < size(type); s += sizeof(var)) {
      var p = ((char*)ptr) + s;
      Cello_GC_Mark_Item(*((var*)p));
    }
    return;
  }
  
}

static void Cello_GC_Print(void);

static var Cello_GC_Mark_Item(var ptr) {
  
  struct GCTab* t = gc_get_table();
  
  uintptr_t pval = (uintptr_t)ptr;
  if (pval % sizeof(var) isnt 0
  or  pval < t->minptr
  or  pval > t->maxptr) { return False; }
  
  uint64_t i = GCTab_Hash(ptr) % t->nslots;
  uint64_t j = 0;
  
  while (True) {
    
    uint64_t h = t->entries[i].hash;
    
    if (h is 0 or j > GCTab_Probe(t, i, h)) { return False; }
    
    if ((t->entries[i].ptr == ptr) and
    not (t->entries[i].flags & CelloMarked)) {
      t->entries[i].flags |= CelloMarked;
      Cello_GC_Recurse(t->entries[i].ptr);
      return True;
    }
    
    i = (i+1) % t->nslots; j++;
  }
  
  return False;
  
}

static void Cello_GC_Mark_Stack(void) {
  
  struct GCTab* t = gc_get_table();
  
  var stk = None;
  var bot = t->bottom;
  var top = &stk;
  
  if (bot == top) { return; }
  
  if (bot < top) {
    for (var p = top; p >= bot; p = ((char*)p) - sizeof(var)) {
      Cello_GC_Mark_Item(*((var*)p));
    }
  }
  
  if (bot > top) {
    for (var p = top; p <= bot; p = ((char*)p) + sizeof(var)) {
      Cello_GC_Mark_Item(*((var*)p));
    }
  }
  
}

static void Cello_GC_Mark_Stack_Fake(void) { }

void Cello_GC_Mark(struct GCTab* t) {
  
  if (t is None or t->nitems is 0) { return; }
  
  for (size_t i = 0; i < t->nslots; i++) {
    if (t->entries[i].hash is 0) { continue; }
    if (t->entries[i].flags & CelloMarked) { continue; }
    if (t->entries[i].flags & CelloHeapAlloc) {
      t->entries[i].flags |= CelloMarked;
      Cello_GC_Recurse(t->entries[i].ptr);
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
  void (*mark_stack)(void) = noinline
    ? Cello_GC_Mark_Stack
    : (void(*)(void))(None);

  mark_stack();
  
}

static void Cello_GC_Print(void) {
 
  struct GCTab* t = gc_get_table();
  printf("| GC TABLE\n");
  for (size_t i = 0; i < t->nslots; i++) {
    if (t->entries[i].hash is 0) { printf("| %i : ---\n", (int)i); continue; }
    printf("| %i : %p %i\n", (int)i, t->entries[i].ptr, (int)t->entries[i].flags);
  }
  printf("|======\n");
  
}

void Cello_GC_Sweep(struct GCTab* t) {
   
  var* freelist = malloc(sizeof(var) * t->nitems);
  size_t freenum = 0;
  
  size_t i = 0;
  while (i < t->nslots) {
    
    if (t->entries[i].hash is 0) { i++; continue; }
    if (t->entries[i].flags & CelloMarked) { i++; continue; }
    
    if ((t->entries[i].flags & CelloGCAlloc) and 
    not (t->entries[i].flags & CelloMarked)) {
      
      freelist[freenum] = t->entries[i].ptr;
      freenum++;
      memset(&t->entries[i], 0, sizeof(struct GCEntry));
      
      uint64_t j = i;
      while (True) { 
        uint64_t nj = (j+1) % t->nslots;
        uint64_t nh = t->entries[nj].hash;
        if (nh isnt 0 and GCTab_Probe(t, nj, nh) > 0) {
          memcpy(&t->entries[j], &t->entries[nj], sizeof(struct GCEntry));
          memset(&t->entries[nj], 0, sizeof(struct GCEntry));
          j = nj;
        } else {
          break;
        }  
      }
      
      t->nitems--;
      continue;
    }
    
    i++;
  }
  
  for (size_t i = 0; i < t->nslots; i++) {
    if (t->entries[i].hash is 0) { continue; }
    if (t->entries[i].flags & CelloMarked) {
      t->entries[i].flags &= ~CelloMarked;
      continue;
    }
  }
  
  GCTab_Resize_Less(t);
  t->mitems = t->nitems + t->nitems / 2 + 1;
  
  for (size_t i = 0; i < freenum; i++) {
    dealloc(destruct(freelist[i]));
  }
  
  free(freelist);
  
}

void gc_finish(void) {
  struct GCTab* t = gc_get_table();
  Cello_GC_Sweep(t);
  free(t->entries);
  free(t);
}

void gc_init(var bottom) {
  struct GCTab* t = calloc(sizeof(struct GCTab), 1);
  gc_set_table(t);
  t->bottom = bottom;
  t->maxptr = 0;
  t->minptr = UINTPTR_MAX;
}

void gc_add(var ptr, int flags) {
  struct GCTab* t = gc_get_table();
  t->nitems++;
  t->maxptr = (uintptr_t)ptr > t->maxptr ? (uintptr_t)ptr : t->maxptr;
  t->minptr = (uintptr_t)ptr < t->minptr ? (uintptr_t)ptr : t->minptr;
  GCTab_Resize_More(t);
  GCTab_Set(t, ptr, flags);
  if (t->nitems > t->mitems) {
    Cello_GC_Mark(t);
    Cello_GC_Sweep(t);
  }
  //Cello_GC_Mark(t);
  //Cello_GC_Sweep(t);
}

void gc_rem(var ptr) {
  struct GCTab* t = gc_get_table();
  GCTab_Rem(t, ptr);
  GCTab_Resize_Less(t);
  t->mitems = t->nitems + t->nitems / 2 + 1;
}

void gc_run(void) {
  struct GCTab* t = gc_get_table();
  Cello_GC_Mark(t);
  Cello_GC_Sweep(t);
}

#endif
