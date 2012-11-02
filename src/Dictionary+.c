#include "Dictionary+.h"

#include "List+.h"
#include "Bool+.h"
#include "None+.h"

#include <stdlib.h>
#include <string.h>

var Dictionary = methods {
  methods_begin(Dictionary),
  method(Dictionary, New),
  method(Dictionary, Assign),
  method(Dictionary, Copy),
  method(Dictionary, Collection),
  method(Dictionary, Dict),
  method(Dictionary, Iter),
  methods_end(Dictionary)
};

var Dictionary_New(var self, va_list* args) {
  DictionaryData* dict = cast(self, Dictionary);
  dict->size = 1024;
  dict->keys = new(List, 0);
  
  dict->key_buckets = calloc(dict->size, sizeof(var));
  dict->val_buckets = calloc(dict->size, sizeof(var));
  
  for (int i = 0; i < dict->size; i++) {
    dict->key_buckets[i] = new(List, 0);
    dict->val_buckets[i] = new(List, 0);
  }
  
  return self;
}

var Dictionary_Delete(var self) {
  DictionaryData* dict = cast(self, Dictionary);
  
  delete(dict->keys);
  
  for (int i = 0; i < dict->size; i++) {
    delete(dict->key_buckets[i]);
    delete(dict->val_buckets[i]);
  }
  
  free(dict->key_buckets);
  free(dict->val_buckets);
  
  return self;
}

void Dictionary_Assign(var self, var obj) {

  clear(self);
  
  foreach(obj, key) {
    var val = get(obj, key);
    put(self, key, val);
  }
}

var Dictionary_Copy(var self) {
  
  var cop = new(Dictionary);
  
  foreach(self, key) {
    var val = get(self, key);
    put(cop, key, val);
  }
  
  return cop;
}

int Dictionary_Len(var self) {
  DictionaryData* dict = cast(self, Dictionary);
  return len(dict->keys);
}

void Dictionary_Clear(var self) {
  DictionaryData* dict = cast(self, Dictionary);
  
  for(int i = 0; i < dict->size; i++) {
    clear(dict->key_buckets[i]);
    clear(dict->val_buckets[i]);
  }
  
  clear(dict->keys);
}

var Dictionary_Contains(var self, var key) {
  DictionaryData* dict = cast(self, Dictionary);
  return contains(dict->keys, key);
}

void Dictionary_Discard(var self, var key) {
  DictionaryData* dict = cast(self, Dictionary);
  
  long i = abs(hash(key)) % dict->size;
  
  var keys = dict->key_buckets[i];
  var vals = dict->val_buckets[i];
  
  int pos = -1;
  
  for(int i = 0; i < len(keys); i++) {
    var k = at(keys, i);
    if_eq(k, key) { pos = i; }
  }
  
  if (pos != -1) {
    discard(dict->keys, key);
    pop_at(keys, i);
    pop_at(vals, i);
  }
  
}

var Dictionary_Get(var self, var key) {

  DictionaryData* dict = cast(self, Dictionary);
  
  long i = abs(hash(key)) % dict->size;
  
  var keys = dict->key_buckets[i];
  var vals = dict->val_buckets[i];
  
  for(int i = 0; i < len(keys); i++) {
    var k = at(keys, i);
    var v = at(vals, i);
    if_eq(k, key) { return v; }
  }
  
  return Undefined;
}

void Dictionary_Put(var self, var key, var val) {

  DictionaryData* dict = cast(self, Dictionary);
  
  long i = abs(hash(key)) % dict->size;
  
  var keys = dict->key_buckets[i];
  var vals = dict->val_buckets[i];
  
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
  push(dict->keys, key);  
}

var Dictionary_Iter_Start(var self) {
  DictionaryData* dict = cast(self, Dictionary);
  return iter_start(dict->keys);
}

var Dictionary_Iter_End(var self) {
  DictionaryData* dict = cast(self, Dictionary);
  return iter_end(dict->keys);
}

var Dictionary_Iter_Next(var self, var curr) {
  DictionaryData* dict = cast(self, Dictionary);
  return iter_next(dict->keys, curr);
}
