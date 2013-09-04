#include "Cello/Dictionary.h"

#include "Cello/List.h"
#include "Cello/Bool.h"
#include "Cello/None.h"
#include "Cello/Exception.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#define Dictionary_Primes_Count 20
static uint32_t const Dictionary_Primes[Dictionary_Primes_Count] =
{
    23,      53,      101,     197,
    389,     683,     1259,    2417,
    4733,    9371,    18617,   37097,
    74093,   148073,  296099,  592019,
    1100009, 2200013, 4400021, 8800019
};

static int __DELETED_BUCKET;
#define DELETED_BUCKET (var)&__DELETED_BUCKET
#define Dictionary_Threshold 0.7f

var Dictionary_Find_Bucket(DictionaryData* dict, var key, int creation, int *index);
void Dictionary_Rehash(DictionaryData* dict);

data {
    uint32_t hash;
    var key;
    var value;
} DictionaryBucket;


var Dictionary = methods {
  methods_begin(Dictionary),
  method(Dictionary, New),
  method(Dictionary, Assign),
  method(Dictionary, Copy),
  method(Dictionary, Eq),
  method(Dictionary, Collection),
  method(Dictionary, Dict),
  method(Dictionary, Iter),
  method(Dictionary, Show),
  methods_end(Dictionary)
};

var Dictionary_New(var self, va_list* args) {
  DictionaryData* dict = cast(self, Dictionary);
  dict->size = Dictionary_Primes[0];
  dict->keys = new(List, 0);
  
  dict->buckets = calloc(dict->size, sizeof(DictionaryBucket));
  
  return self;
}

var Dictionary_Delete(var self) {
  DictionaryData* dict = cast(self, Dictionary);
  
  delete(dict->keys);
  
  for (int i = 0; i < dict->size; i++) {
    free(dict->buckets[i]);
  }
  
  free(dict->buckets);
  return self;
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
    if(dict->buckets[i] != DELETED_BUCKET){
      free(dict->buckets[i]);
    }
    dict->buckets[i] = NULL;
  }
  
  clear(dict->keys);
}

var Dictionary_Contains(var self, var key) {
  DictionaryData* dict = cast(self, Dictionary);
  return contains(dict->keys, key);
}

void Dictionary_Discard(var self, var key) {
  DictionaryData* dict = cast(self, Dictionary);
  int found;
  DictionaryBucket *bucket = Dictionary_Find_Bucket(dict, key, 0, &found);
  if(bucket != NULL){
    free(bucket);
    dict->buckets[found] = DELETED_BUCKET;
  }
  discard(dict->keys, key);
}

var Dictionary_Get(var self, var key) {

  DictionaryData* dict = cast(self, Dictionary);
  DictionaryBucket *bucket = Dictionary_Find_Bucket(dict, key, 0, NULL);

  if (bucket)
    return bucket->value;

  return throw(KeyError, "Key '%$' not in Dictionary!", key);
}

void Dictionary_Put(var self, var key, var val) {

  DictionaryData* dict = cast(self, Dictionary);

  float dict_load = (float)len(dict) / (float)dict->size;
  if (dict_load >=  (float)Dictionary_Threshold) {
    /* Exceeded threshold we have to rehash. doh' */
    Dictionary_Rehash(dict);
  }
  int *indice = malloc(sizeof(int));
  DictionaryBucket *bucket = Dictionary_Find_Bucket(dict, key, 1, indice);

  if (bucket != NULL) {
    bucket->key = key;
    bucket->value = val;
    push(dict->keys, key);
  }
}

void Dictionary_Rehash(DictionaryData* dict)
{
  var * old_buckets = dict->buckets;
  int old_size = dict->size;

  var has_prime = False;
  for (int j = 0; j < Dictionary_Primes_Count; j++) {
    int new_size = Dictionary_Primes[j];
    if(new_size > dict->size){
      dict->size = new_size;
      has_prime = True;
      break;
    }
  }

  if (has_prime is False) {
    // primes are ended
    dict->size *= 2;
  }

  clear(dict->keys);

  dict->buckets = calloc(dict->size, sizeof(DictionaryBucket));
  int i;
  for (i = 0; i < old_size; i++) {
    DictionaryBucket *bucket = old_buckets[i];
    if(bucket != NULL && bucket != DELETED_BUCKET){
      put(dict, bucket->key, bucket->value);
    }
    free(bucket);
    bucket = NULL;
  }

  free(old_buckets);
}

var Dictionary_Find_Bucket(DictionaryData* dict, var key, int creation, int *index)
{

  int i = abs(hash(key) % dict->size);
  DictionaryBucket *bucket = dict->buckets[i];

  int probe_seq = 2;
  int new_i = i;
  if (index != NULL)
     *index = new_i;

  while(bucket != NULL) {
    if ((var)bucket == DELETED_BUCKET && creation) {
      break;
    } else if ( (var)bucket != DELETED_BUCKET && eq(key, bucket->key) ) {
      // it's our Bucket yay o/
      return bucket;
    } else {
      // cubic probing inefficent for size differents from given primes
      //new_i = (int)(i + pow(probe_seq+2, 3) ) % dict->size;
      new_i = (int)(new_i + 2*probe_seq)  % dict->size; // 2 4 6 8 10
      bucket = dict->buckets[new_i];
    }
    probe_seq++;
    if (index != NULL)
       *index = new_i;

    if (probe_seq > dict->size) {
      return NULL;
    }
  }

  // bucket is null if we reach here
  if (creation) {
    bucket = malloc(sizeof(DictionaryBucket));
    dict->buckets[new_i] = bucket;
  }
  return bucket;
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

