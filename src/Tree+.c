#include "Tree+.h"

#include "Array+.h"
#include "Bool+.h"
#include "None+.h"

var Tree = methods {
  methods_begin(Tree),
  method(Tree, New),
  method(Tree, Assign),
  method(Tree, Copy),
  method(Tree, Collection),
  method(Tree, Dict),
  method(Tree, Iter),
  methods_end(Tree),
};

var Tree_New(var self, va_list* args) {
  TreeData* td = cast(self, Tree);
  td->key_type = cast(va_arg(*args, var), Type);
  td->val_type = cast(va_arg(*args, var), Type);
  td->keys = new(Array, td->key_type, 0);
  td->root = NULL;
  return self;
}

var Tree_Delete(var self) {
  TreeData* td = cast(self, Tree);
  clear(self);
  delete(td->keys);
  return self;
}

void Tree_Assign(var self, var obj) {
  TreeData* other = cast(self, Tree);
  clear(self);
  foreach(other, key) {
    put(self, key, get(other, key));
  }
}

var Tree_Copy(var self) {
  TreeData* td = cast(self, Tree);
  var newmap = new(Tree, td->key_type, td->val_type);
  foreach(self, key) {
    put(newmap, key, get(self, key));
  }
  return newmap;
}

int Tree_Len(var self) {
  TreeData* td = cast(self, Tree);
  return len(td->keys);
}

void Tree_Clear(var self) {
  foreach(self, key) {
    discard(self, key);
  }
}

var Tree_Contains(var self, var key) {
  TreeData* td = cast(self, Tree);
  return contains(td->keys, key);
}

void Tree_Discard(var self, var key){
  // TODO: Implement
}

var Tree_Get(var self, var key) {
  // TODO: Implement
  return Undefined;
}

void Tree_Put(var self, var key, var val) {
  // TODO: Implement
}

var Tree_Iter_Start(var self) {
  TreeData* td = cast(self, Tree);
  return iter_start(td->keys);
}

var Tree_Iter_End(var self) {
  TreeData* td = cast(self, Tree);
  return iter_end(td->keys);
}

var Tree_Iter_Next(var self, var curr) {
  TreeData* td = cast(self, Tree);
  return iter_next(td->keys, curr);
}
