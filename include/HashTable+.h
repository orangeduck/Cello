/*
** == Hashtable ==
**
**  Collection that maps keys to values
**
**  + keys require Eq and Hash
**  + Just holds references
**  + Not responsible for object deallocation
*/

#ifndef HashTablePlus_h
#define HashTablePlus_h

#include "Prelude+.h"
#include "Type+.h"

module HashTable;

struct HashBucket {
  var key;
  var value;
  struct HashBucket* next;
};

data {
  var type;
  int table_size;
  var keys;
  struct HashBucket** buckets;
} HashTableData;

/** Hashtable_New(var self); */
var HashTable_New(var self, va_list* args);
var HashTable_Delete(var self);
void HashTable_Assign(var self, var obj);
var HashTable_Copy(var self);

int HashTable_Len(var self);
bool HashTable_IsEmpty(var self);
void HashTable_Clear(var self);
bool HashTable_Contains(var self, var key);
void HashTable_Discard(var self, var key);

var HashTable_Get(var self, var key);
void HashTable_Put(var self, var key, var val);

var HashTable_Iter_Start(var self);
var HashTable_Iter_End(var self);
var HashTable_Iter_Next(var self, var curr);

instance(HashTable, New) = { sizeof(HashTableData), HashTable_New, HashTable_Delete };
instance(HashTable, Assign) = { HashTable_Assign };
instance(HashTable, Copy) = { HashTable_Copy };
instance(HashTable, Collection) = { HashTable_Len, HashTable_IsEmpty, HashTable_Clear, HashTable_Contains, HashTable_Discard };
instance(HashTable, Dict) = { HashTable_Get, HashTable_Put };
instance(HashTable, Iter) = { HashTable_Iter_Start, HashTable_Iter_End, HashTable_Iter_Next };

#endif