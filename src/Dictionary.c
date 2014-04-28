#include "Cello/Dictionary.h"

#include "Cello/List.h"
#include "Cello/Bool.h"
#include "Cello/None.h"
#include "Cello/Exception.h"

#include <stdlib.h>
#include <string.h>

data {
  var type;
  int size;
  var keys;
  var* key_buckets;
  var* val_buckets;
} DictionaryData;

var Dictionary = type_data {
  type_begin(Dictionary),
  type_entry(Dictionary, New),
  type_entry(Dictionary, Assign),
  type_entry(Dictionary, Copy),
  type_entry(Dictionary, Eq),
  type_entry(Dictionary, Collection),
  type_entry(Dictionary, Dict),
  type_entry(Dictionary, Iter),
  type_entry(Dictionary, Show),
  type_end(Dictionary)
};

var Dictionary_New(var self, var_list vl) {
  DictionaryData* dict = cast(self, Dictionary);
  
  dict->size = Hash_Table_Size(0);
  dict->keys = new(List);
  
  dict->key_buckets = malloc(dict->size * sizeof(var));
  dict->val_buckets = malloc(dict->size * sizeof(var));
  
  if (dict->key_buckets == NULL) { throw(OutOfMemoryError, "Cannot create Dictionary. Out of memory!"); }
  if (dict->val_buckets == NULL) { throw(OutOfMemoryError, "Cannot create Dictionary. Out of memory!"); }
  
  for (int i = 0; i < dict->size; i++) {
    dict->key_buckets[i] = new(List);
    dict->val_buckets[i] = new(List);
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

size_t Dictionary_Size(void) {
  return sizeof(DictionaryData);
}

void Dictionary_Assign(var self, var obj) {

  clear(self);
  
  foreach(key in obj) {
    var val = get(obj, key);
    put(self, key, val);
  }
}

var Dictionary_Copy(var self) {
  
  var cop = new(Dictionary);
  
  foreach(key in self) {
    var val = get(self, key);
    put(cop, key, val);
  }
  
  return cop;
}

var Dictionary_Eq(var self, var obj) {
  DictionaryData* dict = cast(self, Dictionary);
  if_neq(type_of(obj), Dictionary) { return False; }
  
  foreach(key in obj) {
    if (not contains(self, key)) { return False; }
    if_neq(get(obj, key), get(self, key)) { return False; }
  }
	
  foreach(key in self) {
    if (not contains(obj, key)) { return False; }
    if_neq(get(obj, key), get(self, key)) { return False; }
  }
  	
  return True;
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

static void Dictionary_Rehash(DictionaryData* dict) {

  int new_size = Hash_Table_Size(len(dict)/2);
  int old_size = dict->size;
  if (old_size == new_size) { return; }

  var* old_keys = dict->key_buckets;
  var* old_vals = dict->val_buckets;  
  
  dict->size = new_size;
  dict->key_buckets = malloc(dict->size * sizeof(var));
  dict->val_buckets = malloc(dict->size * sizeof(var));
  
  if (dict->key_buckets == NULL) { throw(OutOfMemoryError, "Cannot create Dictionary. Out of memory!"); }
  if (dict->val_buckets == NULL) { throw(OutOfMemoryError, "Cannot create Dictionary. Out of memory!"); }
  
  for (int i = 0; i < dict->size; i++) {
    dict->key_buckets[i] = new(List);
    dict->val_buckets[i] = new(List);
  }
  
  for (int i = 0; i < old_size; i++) {
    var keys = old_keys[i];
    var vals = old_vals[i];
    
    for (int j = 0; j < len(keys); j++) {
      long ni = abs(hash(at(keys, j)) % dict->size);
      push(dict->key_buckets[ni], at(keys, j));
      push(dict->val_buckets[ni], at(vals, j));;
    }
    
    delete(keys);
    delete(vals);
  }
  
  free(old_keys);
  free(old_vals);
}

void Dictionary_Discard(var self, var key) {
  DictionaryData* dict = cast(self, Dictionary);
  
  long i = abs(hash(key) % dict->size);
  
  var keys = dict->key_buckets[i];
  var vals = dict->val_buckets[i];
  
  for(int j = 0; j < len(keys); j++) {
    if_eq(at(keys, j), key) {
      discard(dict->keys, key);
      pop_at(keys, j);
      pop_at(vals, j);
      Dictionary_Rehash(dict);
      return;
    }
  }
  
  throw(KeyError, "Key %$ not in Dictionary!", key);
}

var Dictionary_Get(var self, var key) {

  DictionaryData* dict = cast(self, Dictionary);
  
  long i = abs(hash(key) % dict->size);
  
  var keys = dict->key_buckets[i];
  var vals = dict->val_buckets[i];
  
  for(int i = 0; i < len(keys); i++) {
    var k = at(keys, i);
    var v = at(vals, i);
    if_eq(k, key) { return v; }
  }
  
  return throw(KeyError, "Key %$ not in Dictionary!", key);
}

void Dictionary_Put(var self, var key, var val) {

  DictionaryData* dict = cast(self, Dictionary);
  Dictionary_Rehash(dict);
  
  long i = abs(hash(key) % dict->size);
  
  var keys = dict->key_buckets[i];
  var vals = dict->val_buckets[i];
  
  for(int j = 0; j < len(keys); j++) {
    if_eq(at(keys, j), key) {
      discard(dict->keys, key);
      pop_at(keys, j);
      pop_at(vals, j);
    }
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

int Dictionary_Show(var self, var output, int pos) {
  DictionaryData* dd = cast(self, Dictionary);
  
  pos = print_to(output, pos, "<'Dictionary' At 0x%p {", self);
  
  for(int i = 0; i < len(self); i++) {
    var key = at(dd->keys, i);
    var val = get(self, key);
    pos = print_to(output, pos, "%$:%$", key, get(self, key));
    if (i < len(self)-1) { pos = print_to(output, pos, ", "); }
  }
  
  pos = print_to(output, pos, "}>");
  
  return pos;
}

