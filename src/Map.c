#include "Cello.h"

static const char* Map_Name(void) {
  return "Map";
}

/* TODO */
static const char* Map_Brief(void) {
  return "";
}

/* TODO */
static const char* Map_Description(void) {
  return "";
}

/* TODO */
static const char* Map_Examples(void) {
  return "";
}

/* TODO */
static const char* Map_Methods(void) {
  return "";
}

/*
struct Map {
  var root;
  var ktype;
  var vtype;
  size_t nitems;
};

static var Map_Item_Ptr(struct Map* m, var item) {
  return item - sizeof(struct CelloHeader) - 3 * sizeof(var);
}

static size_t Map_Item_Size(struct Map* m) {
  return size(m->ktype) + size(m->vtype) + 
    sizeof(struct CelloHeader) + 3 * sizeof(var);
}

static var* Map_Left(struct Map* m, var item) {
  return item - sizeof(struct CelloHeader) - 3 * sizeof(var);
}

static var* Map_Right(struct Map* m, var item) {
  return item - sizeof(struct CelloHeader) - 2 * sizeof(var);
}

static var* Map_Parent(struct Map* m, var item) {
  return item - sizeof(struct CelloHeader) - 1 * sizeof(var);
}

static void Map_Set(var self, var key, var val);

static var Map_New(var self, var args) {
  struct Map* m = self;
  m->root = Terminal;
  m->ktype = get(args, $I(0));
  m->vtype = get(args, $I(1));
  m->nitems = 0;
  
  size_t nargs = len(args);
  if (nargs % 2 isnt 0) {
    return throw(FormatError, 
      "Received non multiple of two argument count to Map constructor.");
  }
  
  for(size_t i = 0; i < (nargs-2)/2; i++) {
    var key = get(args, $I(2+(i*2)+0));
    var val = get(args, $I(2+(i*2)+1));
    Map_Set(t, key, val);
  }
  
  return self;
}

static void Map_Clear(var self) {
  struct Map* m = self;
  var c = Map_Iter_Init(m);
  var n = Map_Iter_Next(m, c);
  do {
    destruct(c);
    free(Map_Item_Ptr(m, c));
    c = n;
    n = Map_Iter_Next(m, c);
  } while (n isnt Terminal);
  m->root = None;
}

static var Map_Del(var self) {
  Map_Clear(self);
  return self;
}

static size_t Map_Size(void) {
  return sizeof(struct Map);
}

static var Map_Assign(var self, var obj) {
  Map_Clear(self);
  foreach (key in obj) {
    Map_Set(self, key, get(obj, key));
  }
  return self;
}

*/


