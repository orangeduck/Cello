#include "Cello/Table.h"

#include "Cello/Array.h"
#include "Cello/List.h"
#include "Cello/Bool.h"
#include "Cello/None.h"
#include "Cello/Exception.h"

#include <stdlib.h>
#include <string.h>

data {
  var type;
  var key_type;
  var val_type;
  int size;
  var keys;
  var* key_buckets;
  var* val_buckets;
} TableData;

var Table = type_data {
  type_begin(Table),
  type_entry(Table, New),
  type_entry(Table, Assign),
  type_entry(Table, Copy),
  type_entry(Table, Eq),
  type_entry(Table, Collection),
  type_entry(Table, Dict),
  type_entry(Table, Iter),
  type_entry(Table, Show),
  type_end(Table)
};

var Table_New(var self, var_list vl) {
  TableData* tab = cast(self, Table);
  
  tab->key_type = cast(var_list_get(vl), Type);
  tab->val_type = cast(var_list_get(vl), Type);
  
  tab->size = Hash_Table_Size(0);
  tab->keys = new(Array, tab->key_type);
  
  tab->key_buckets = malloc(tab->size * sizeof(var));
  tab->val_buckets = malloc(tab->size * sizeof(var));
  
  if (tab->key_buckets == NULL) { throw(OutOfMemoryError, "Cannot create Table. Out of memory!"); }
  if (tab->val_buckets == NULL) { throw(OutOfMemoryError, "Cannot create Table. Out of memory!"); }
  
  for (int i = 0; i < tab->size; i++) {
    tab->key_buckets[i] = new(Array, tab->key_type);
    tab->val_buckets[i] = new(Array, tab->val_type);
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

size_t Table_Size(void) {
  return sizeof(TableData);
}

void Table_Assign(var self, var obj) {

  clear(self);
  
  foreach(key in obj) {
    put(self, key, get(obj, key));
  }
}

var Table_Copy(var self) {
  TableData* tab = cast(self, Table);
  
  var cop = new(Table, tab->key_type, tab->val_type);
  
  foreach(key in self) {
    put(cop, key, get(self, key));
  }
  
  return cop;
}

var Table_Eq(var self, var obj) {
  TableData* tab = cast(self, Table);
  
  if_neq(type_of(obj), Table) { return False; }
  
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

int Table_Len(var self) {
  TableData* tab = cast(self, Table);
  return len(tab->keys);
}

void Table_Clear(var self) {
  TableData* tab = cast(self, Table);
  
  clear(tab->keys);

  for(int i = 0; i < tab->size; i++) {
    clear(tab->key_buckets[i]);
    clear(tab->val_buckets[i]);
  }
  
}

var Table_Contains(var self, var key) {
  TableData* tab = cast(self, Table);
  key = cast(key, tab->key_type);
  return contains(tab->keys, key);
}

static void Table_Rehash(TableData* tab) {
  
  int new_size = Hash_Table_Size(len(tab)/2);
  int old_size = tab->size;
  if (old_size == new_size) { return; }
    
  var* old_keys = tab->key_buckets;
  var* old_vals = tab->val_buckets;
  
  tab->size = new_size;
  tab->key_buckets = malloc(tab->size * sizeof(var));
  tab->val_buckets = malloc(tab->size * sizeof(var));

  if (tab->key_buckets == NULL) { throw(OutOfMemoryError, "Cannot create Table. Out of memory!"); }
  if (tab->val_buckets == NULL) { throw(OutOfMemoryError, "Cannot create Table. Out of memory!"); }
  
  for (int i = 0; i < tab->size; i++) {
    tab->key_buckets[i] = new(Array, tab->key_type);
    tab->val_buckets[i] = new(Array, tab->val_type);
  }
  
  for (int i = 0; i < old_size; i++) {
    var keys = old_keys[i];
    var vals = old_vals[i];
    
    for (int j = 0; j < len(keys); j++) {
      long ni = abs(hash(at(keys, j)) % tab->size);
      push(tab->key_buckets[ni], at(keys, j));
      push(tab->val_buckets[ni], at(vals, j));
    }
    
    delete(keys);
    delete(vals);
  }
  
  free(old_keys);
  free(old_vals);
}

void Table_Discard(var self, var key) {
  TableData* tab = cast(self, Table);
  key = cast(key, tab->key_type);
  
  long i = abs(hash(key) % tab->size);
  
  var keys = tab->key_buckets[i];
  var vals = tab->val_buckets[i];
  
  for(int j = 0; j < len(keys); j++) {
    if_eq(at(keys, j), key) {
      discard(tab->keys, key);
      pop_at(keys, j);
      pop_at(vals, j);
      Table_Rehash(self);
      return;
    }
  }
  
  throw(KeyError, "Key %$ not in Table!", key);
}

var Table_Get(var self, var key) {

  TableData* tab = cast(self, Table);
  key = cast(key, tab->key_type);
  
  long i = abs(hash(key) % tab->size);
  
  var keys = tab->key_buckets[i];
  var vals = tab->val_buckets[i];
  
  for (int j = 0; j < len(keys); j++) {
    if_eq(at(keys, j), key) { return at(vals, j); }
  }
  
  return throw(KeyError, "Key %$ not in Table!", key);
}

void Table_Put(var self, var key, var val) {
  
  TableData* tab = cast(self, Table);
  Table_Rehash(self);
  
  key = cast(key, tab->key_type);
  val = cast(val, tab->val_type);
  
  long i = abs(hash(key) % tab->size);
  
  var keys = tab->key_buckets[i];
  var vals = tab->val_buckets[i];
  
  for(int j = 0; j < len(keys); j++) {
    if_eq(at(keys, j), key) {
      discard(tab->keys, key);
      pop_at(keys, j);
      pop_at(vals, j);
    }
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

int Table_Show(var self, var output, int pos) {
  TableData* td = cast(self, Table);
  
  pos = print_to(output, pos, "<'Table' At 0x%p {", self);
  
  for(int i = 0; i < len(self); i++) {
    var key = at(td->keys, i);
    var val = get(self, key);
    pos = print_to(output, pos, "%$:%$", key, get(self, key));
    if (i < len(self)-1) { pos = print_to(output, pos, ", "); }
  }
  
  pos = print_to(output, pos, "}>");
  
  return pos;
}

