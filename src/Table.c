#include "Cello/Table.h"
#include "Cello/Array.h"
#include "Cello/List.h"
#include "Cello/Bool.h"
#include "Cello/None.h"
#include "Cello/Exception.h"

#include <stdlib.h>
#include <string.h>

var Table = methods {
  methods_begin(Table),
  method(Table, New),
  method(Table, Assign),
  method(Table, Copy),
  method(Table, Eq),
  method(Table, Collection),
  method(Table, Dict),
  method(Table, Iter),
  method(Table, Show),
  methods_end(Table)
};


data {
    uint32_t hash;
    var key;
    var value;
} TableBucket;
var Table_Find_Bucket(TableData* tab, var key, var creation, int *index);
void TableBucket_Delete(TableData* tab, TableBucket *bucket);
void Table_Rehash(TableData* tab);


var Table_New(var self, va_list* args) {
  TableData* tab = cast(self, Table);

  tab->key_type = cast(va_arg(*args, var), Type);
  tab->val_type = cast(va_arg(*args, var), Type);
  
  tab->size = Hashing_Primes[0];
  tab->keys = new(Array, tab->key_type, 0);
  
  tab->buckets = malloc(tab->size * sizeof(var));
  
  if (tab->buckets == NULL) { throw(OutOfMemoryError, "Cannot create Table. Out of memory!"); }
  
  return self;
}

var Table_Delete(var self) {
  TableData* tab = cast(self, Table);
  
  delete(tab->keys);
  
  for (int i = 0; i < tab->size; i++) {
    if(tab->buckets[i] != NULL){
      TableBucket_Delete(tab, tab->buckets[i]);
    }
  }
  
  free(tab->buckets);
  
  return self;
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
    clear(tab->buckets[i]);
  }
  
}

var Table_Contains(var self, var key) {
  TableData* tab = cast(self, Table);
  key = cast(key, tab->key_type);
  return contains(tab->keys, key);
}

void Table_Discard(var self, var key) {
  
  TableData* tab = cast(self, Table);
  key = cast(key, tab->key_type);
  
  int index;
  TableBucket *bucket = Table_Find_Bucket(tab, key, False, &index);
  if(bucket != NULL) {
    // delete bucket;
    TableBucket_Delete(tab, bucket);
    tab->buckets[index] = Hashing_DELETED;
  }

  discard(tab->keys, key);
}

var Table_Get(var self, var key) {

  TableData* tab = cast(self, Table);
  key = cast(key, tab->key_type);

  long i = abs(hash(key) % tab->size);
  
  TableBucket *bucket = Table_Find_Bucket(tab, key, False, NULL);
  if (bucket)
    return bucket->value;

  
  return throw(KeyError, "Key not in Table!");
}

void Table_Put(var self, var key, var val) {
  TableData* tab = cast(self, Table);
   key = cast(key, tab->key_type);
   val = cast(val, tab->val_type);

   float dict_load = (float)len(tab) / (float)tab->size;
   if (dict_load >=  (float)Hashing_Threshold) {
     /* Exceeded threshold we have to rehash. doh' */
     Table_Rehash(tab);
   }

   TableBucket *bucket = Table_Find_Bucket(tab, key, True, NULL);
   if (bucket != NULL) {

     // discard previous value
     print("\n--- bucket has %$ %$ in %p\n", bucket->key, bucket->value, tab);
     print("--- putting %$ %$ \n", key, val);
     if(bucket->key != NULL) {
       delete(bucket->key);
     } else {
       push(tab->keys, key);
     }
     if(bucket->value != NULL) delete(bucket->value);

     bucket->key = copy(key);
     bucket->value = copy(val);

   }
}

var Table_Find_Bucket(TableData* tab, var key, var creation, int *index)
{

  int i = abs(hash(key) % tab->size);
  TableBucket *bucket = tab->buckets[i];

  int probe_seq = 2;
  int new_i = i;
  if (index != NULL) *index = new_i;

  while(bucket != NULL) {
    print("%$ %$\n", key, bucket->key);
    if ((var)bucket == Hashing_DELETED && creation) {
      break;
    } else if ( (var)bucket != Hashing_DELETED && eq(key, bucket->key) ) {
      // it's our Bucket yay o/
      return bucket;
    } else {
      // linear probing could be improved
      new_i = (int)(new_i + 3*probe_seq)  % tab->size;
      bucket = tab->buckets[new_i];
    }

    probe_seq++;
    if (index != NULL) *index = new_i;

    if (probe_seq > tab->size) {
      return NULL;
    }
  }

  // bucket is null if we reach here
  if (creation is True) {
    bucket = malloc(sizeof(TableBucket));
    tab->buckets[new_i] = bucket;
  }
  return bucket;
}

void TableBucket_Delete(TableData* tab, TableBucket *bucket)
{
    var key = cast(bucket->key, tab->key_type);
    var val = cast(bucket->value, tab->val_type);
    delete(key);
    delete(val);
    free(bucket);
}

void Table_Rehash(TableData* tab)
{
  var * old_buckets = tab->buckets;
  int old_size = tab->size;

  var has_prime = False;
  for (int j = 0; j < Hashing_Primes_Count; j++) {
    int new_size = Hashing_Primes[j];
    if(new_size > tab->size){
      tab->size = new_size;
      has_prime = True;
      break;
    }
  }
  if (has_prime is False) {
    // primes are ended
    tab->size *= 2;
  }

  clear(tab->keys);

  tab->buckets = calloc(tab->size, sizeof(TableBucket));

  for (int i = 0; i < old_size; i++) {
    TableBucket *bucket = old_buckets[i];
    if(bucket != NULL && bucket != Hashing_DELETED){
      put(tab, bucket->key, bucket->value);
    }
    free(bucket);
    bucket = NULL;
  }

  free(old_buckets);
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

