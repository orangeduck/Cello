#ifndef HashTablePlus_h
#define HashTablePlus_h

module HashTable;

struct HashBucket {
  var key;
  var value;
  struct HashBucket* next;
};

data {
  Type type;
  int table_size;
  var keys;
  struct HashBucket** buckets;
} HashTableData;

void HashTable_New(var, va_list*);
void HashTable_Delete(var);
var HashTable_Copy(var);

size_t HashTable_Len(var);
bool HashTable_IsEmpty(var);
void HashTable_Clear(var);

bool HashTable_Contains(var, var);
void HashTable_Erase(var, var);

var HashTable_Get(var, var);
void HashTable_Put(var, var, var);

var HashTable_Iter_Start(var);
var HashTable_Iter_End(var);
var HashTable_Iter_Next(var, var);

instance(HashTable, New) = { sizeof(HashTableData), HashTable_New, HashTable_Delete };
instance(HashTable, Copy) = { HashTable_Copy };
instance(HashTable, Len) = { HashTable_Len };
instance(HashTable, Empty) = { HashTable_IsEmpty, HashTable_Clear };
instance(HashTable, Contains) = { HashTable_Contains, HashTable_Erase };
instance(HashTable, Dict) = { HashTable_Get, HashTable_Put };
instance(HashTable, Iter) = { HashTable_Iter_Start, HashTable_Iter_End, HashTable_Iter_Next };

#endif