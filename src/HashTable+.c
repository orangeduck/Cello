#include <stdlib.h>
#include <string.h>

#include "List+.h"
#include "NoneType+.h"

#include "HashTable+.h"

var HashTable = methods {
  methods_begin(HashTable),
  method(HashTable, New),
  method(HashTable, Assign),
  method(HashTable, Copy),
  method(HashTable, Collection),
  method(HashTable, Dict),
  method(HashTable, Iter),
  methods_end(HashTable)
};

static void HashBucket_DeleteAll(struct HashBucket* hb) {
  
  if (hb == NULL) return;
  
  HashBucket_DeleteAll(hb->next);
  free(hb);
}

var HashTable_New(var self, va_list* args) {
  HashTableData* ht = cast(self, HashTable);
  ht->keys = new(List, 0);
  ht->table_size = 87;
  ht->buckets = calloc(sizeof(struct HashBucket*), ht->table_size);
  return self;
}

var HashTable_Delete(var self) {
  HashTableData* ht = cast(self, HashTable);
  
  for(int i = 0; i < ht->table_size; i++) {
    HashBucket_DeleteAll(ht->buckets[i]);
  }
  
  delete(ht->keys);
  free(ht->buckets);
  return self;
}

void HashTable_Assign(var self, var obj) {
  HashTableData* ht = cast(self, HashTable);
  
  for(int i = 0; i < ht->table_size; i++) {
    HashBucket_DeleteAll(ht->buckets[i]);
  }
  
  clear(ht->keys);
  
  foreach(obj, key) {
    var val = get(obj, key);
    put(self, key, val);
  }
}

var HashTable_Copy(var self) {
  
  var cop = new(HashTable);
  
  foreach(self, key) {
    var val = get(self, key);
    put(cop, key, val);
  }
  
  return cop;
}

int HashTable_Len(var self) {
  HashTableData* ht = cast(self, HashTable);
  return len(ht->keys);
}

bool HashTable_IsEmpty(var self) {
  return (len(self) == 0);
}

void HashTable_Clear(var self) {
  
  HashTableData* ht = cast(self, HashTable);
  
  for(int i = 0; i < ht->table_size; i++) {
    HashBucket_DeleteAll(ht->buckets[i]);
  }
  
  memset(ht->buckets, 0, sizeof(struct HashBucket*) * ht->table_size);
  clear(ht->keys);
}

bool HashTable_Contains(var self, var key) {
  HashTableData* ht = cast(self, HashTable);
  return contains(ht->keys, key);
}

void HashTable_Discard(var self, var key) {
  HashTableData* ht = cast(self, HashTable);
  
  long loc = hash(key) % ht->table_size;
  struct HashBucket* b = ht->buckets[loc];
  
  if ( b == NULL ) return;
  
  if ( eq(b->key, key) ) {
    ht->buckets[loc] = b->next;
    discard(ht->keys, key);
    free(b);
  }
  
  while (1) {
    if ( eq(b->next->key, key) ) {
      b->next = b->next->next;
      discard(ht->keys, key);
      free(b);
    }
    
    if ( b->next == NULL ) return;
    
    b = b->next;
  }
  
  return;
}

var HashTable_Get(var self, var key) {

  HashTableData* ht = cast(self, HashTable);
  long loc = hash(key) % ht->table_size;
  struct HashBucket* b = ht->buckets[loc];
  
  if ( b == NULL ) return None;
  
  while (1) {
    if ( eq(b->key, key) ) {
      return b->value;
    }
    
    if ( b->next == NULL ) {
      return None;
    }
    
    b = b->next;
  }
  
  return None;
}

static struct HashBucket* HashBucket_New(var key, var val, struct HashBucket* next) {
  struct HashBucket* hb = malloc(sizeof(struct HashBucket));
  hb->key = key;
  hb->value = val;
  hb->next = next;
  return hb;
};

void HashTable_Put(var self, var key, var val) {
  HashTableData* ht = cast(self, HashTable);
  long loc = hash(key) % ht->table_size;
  struct HashBucket* b = ht->buckets[loc];
  
  if ( b == NULL ) {
    push(ht->keys, key);
    ht->buckets[loc] = HashBucket_New(key, val, NULL);
    return;
  }
  
  while (1) {
    if ( eq(b->key, key) ) {
      discard(ht->keys, b->key);
      push(ht->keys, key);
      b->key = key;
      b->value = val;
      return;
    }
    
    if ( b->next == NULL ) {
      b->next = HashBucket_New(key, val, NULL);
      push(ht->keys, key);
      return;
    }
    
    b = b->next;
  }
}

var HashTable_Iter_Start(var self) {
  HashTableData* ht = cast(self, HashTable);
  return iter_start(ht->keys);
}

var HashTable_Iter_End(var self) {
  HashTableData* ht = cast(self, HashTable);
  return iter_end(ht->keys);
}

var HashTable_Iter_Next(var self, var curr) {
  HashTableData* ht = cast(self, HashTable);
  return iter_next(ht->keys, curr);
}