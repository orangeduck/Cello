#include "Table+.h"

#include "Array+.h"
#include "List+.h"
#include "Bool+.h"
#include "None+.h"

#include <stdlib.h>
#include <string.h>

var Table = methods {
  methods_begin(Table),
  method(Table, New),
  method(Table, Assign),
  method(Table, Copy),
  method(Table, Collection),
  method(Table, Dict),
  method(Table, Iter),
  methods_end(Table)
};

var Table_New(var self, va_list* args) {
  TableData* tab = cast(self, Table);
  
  tab->key_type = cast(va_arg(*args, var), Type);
  tab->val_type = cast(va_arg(*args, var), Type);
  
  tab->size = 1024;
  tab->keys = new(List, 0);
  
  tab->key_buckets = malloc(tab->size * sizeof(ArrayData));
  tab->val_buckets = malloc(tab->size * sizeof(ArrayData));
  
  for (int i = 0; i < tab->size; i++) {
    tab->key_buckets[i] = new(Array, tab->key_type, 0);
    tab->val_buckets[i] = new(Array, tab->val_type, 0);
  }
  
  return self;
}

var Table_Delete(var self) {
  TableData* tab = cast(self, Table);
  
  delete(tab->keys);
  
  for (int i = 0; i < tab->size; i++) {
    delete(tab->key_buckets[i]);
    delete(tab->val_buckets[i]);
  }
  
  free(tab->key_buckets);
  free(tab->val_buckets);
  
  return self;
}

void Table_Assign(var self, var obj) {

  clear(self);
  
  foreach(obj, key) {
    var val = get(obj, key);
    put(self, key, val);
  }
}

var Table_Copy(var self) {
  TableData* tab = cast(self, Table);
  
  var cop = new(Table, tab->key_type, tab->val_type);
  
  foreach(self, key) {
    var val = get(self, key);
    put(cop, key, val);
  }
  
  return cop;
}

int Table_Len(var self) {
  TableData* tab = cast(self, Table);
  return len(tab->keys);
}

void Table_Clear(var self) {
  TableData* tab = cast(self, Table);
  
  for(int i = 0; i < tab->size; i++) {
    clear(tab->key_buckets[i]);
    clear(tab->val_buckets[i]);
  }
  
  clear(tab->keys);
}

var Table_Contains(var self, var key) {
  TableData* tab = cast(self, Table);
  key = cast(key, tab->key_type);
  return contains(tab->keys, key);
}

void Table_Discard(var self, var key) {
  TableData* tab = cast(self, Table);
  key = cast(key, tab->key_type);
  
  long i = hash(key) % tab->size;
  
  var keys = tab->key_buckets[i];
  var vals = tab->val_buckets[i];
  
  int pos = -1;
  
  for(int i = 0; i < len(keys); i++) {
    var k = at(keys, i);
    if_eq(k, key) { pos = i; }
  }
  
  if (pos != -1) {
    discard(tab->keys, key);
    pop_at(keys, i);
    pop_at(vals, i);
  }
  
}

var Table_Get(var self, var key) {

  TableData* tab = cast(self, Table);
  key = cast(key, tab->key_type);
  
  long i = hash(key) % tab->size;
  
  var keys = tab->key_buckets[i];
  var vals = tab->val_buckets[i];
  
  for(int i = 0; i < len(keys); i++) {
    var k = at(keys, i);
    var v = at(vals, i);
    if_eq(k, key) { return v; }
  }
  
  return None;
}

void Table_Put(var self, var key, var val) {

  TableData* tab = cast(self, Table);
  key = cast(key, tab->key_type);
  val = cast(val, tab->val_type);
  
  long i = hash(key) % tab->size;
  
  var keys = tab->key_buckets[i];
  var vals = tab->val_buckets[i];
  
  int pos = -1;
  
  for(int i = 0; i < len(keys); i++) {
    var k = at(keys, i);
    if_eq(k, key) { pos = i; }
  }

  if (pos != -1) {
    pop_at(keys, i);
    pop_at(vals, i);
  }
  
  push(keys, key);
  push(vals, val);
  push(tab->keys, key);  
}

var Table_Iter_Start(var self) {
  TableData* tab = cast(self, Table);
  return iter_start(tab->keys);
}

var Table_Iter_End(var self) {
  TableData* tab = cast(self, Table);
  return iter_end(tab->keys);
}

var Table_Iter_Next(var self, var curr) {
  TableData* tab = cast(self, Table);
  return iter_next(tab->keys, curr);
}
