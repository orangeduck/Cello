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

var HashTable_New(var, va_list*);
var HashTable_Delete(var);
var HashTable_Copy(var);

int HashTable_Len(var);
bool HashTable_IsEmpty(var);
void HashTable_Clear(var);
bool HashTable_Contains(var, var);
void HashTable_Discard(var, var);

var HashTable_Get(var, var);
void HashTable_Put(var, var, var);

var HashTable_Iter_Start(var);
var HashTable_Iter_End(var);
var HashTable_Iter_Next(var, var);

instance(HashTable, New) = { sizeof(HashTableData), HashTable_New, HashTable_Delete };
instance(HashTable, Copy) = { HashTable_Copy };
instance(HashTable, Collection) = { HashTable_Len, HashTable_IsEmpty, HashTable_Clear, 
                                    HashTable_Contains, HashTable_Discard };
                                    
instance(HashTable, Dict) = { HashTable_Get, HashTable_Put };
instance(HashTable, Iter) = { HashTable_Iter_Start, HashTable_Iter_End, HashTable_Iter_Next };

#endif