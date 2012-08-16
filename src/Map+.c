#include "List+.h"
#include "Bool+.h"
#include "None+.h"

#include "Map+.h"

var Map = methods {
  methods_begin(Map),
  method(Map, New),
  method(Map, Assign),
  method(Map, Copy),
  method(Map, Collection),
  method(Map, Dict),
  method(Map, Iter),
  methods_end(Map),
};

var Map_New(var self, va_list* args) {
  MapData* md = cast(self, Map);
  md->keys = new(List, 0);
  md->root = NULL;
  return self;
}

var Map_Delete(var self) {
  MapData* md = cast(self, Map);
  clear(self);
  delete(md->keys);
  return self;
}

void Map_Assign(var self, var obj) {
  MapData* other = cast(self, Map);
  clear(self);
  foreach(other, key) {
    put(self, key, get(other, key));
  }
}

var Map_Copy(var self) {
  var newmap = new(Map);
  foreach(self, key) {
    put(newmap, key, get(self, key));
  }
  return newmap;
}

int Map_Len(var self) {
  MapData* md = cast(self, Map);
  return len(md->keys);
}

var Map_IsEmpty(var self) {
  return (var)(len(self) == 0);
}

void Map_Clear(var self) {
  foreach(self, key) {
    discard(self, key);
  }
}

var Map_Contains(var self, var key) {
  MapData* md = cast(self, Map);
  return contains(md->keys, key);
}

void Map_Discard(var self, var key){
  // TODO: Implement
}

var Map_Get(var self, var key) {
  // TODO: Implement
  return None;
}

void Map_Put(var self, var key, var val) {
  // TODO: Implement
}

var Map_Iter_Start(var self) {
  MapData* md = cast(self, Map);
  return iter_start(md->keys);
}

var Map_Iter_End(var self) {
  MapData* md = cast(self, Map);
  return iter_end(md->keys);
}

var Map_Iter_Next(var self, var curr) {
  MapData* md = cast(self, Map);
  return iter_next(md->keys, curr);
}
