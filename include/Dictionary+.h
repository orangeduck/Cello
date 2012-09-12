/*
** == Dictionary ==
**
**  Collection that maps keys to values
**
**  + Does not store keys or values, only holds references.
**  + Key objects require "Hash" and "Eq"
**  + Not responsible for object deallocation
*/

#ifndef DictionaryPlus_h
#define DictionaryPlus_h

#include "Prelude+.h"
#include "Type+.h"

module Dictionary;

data {
  var type;
  int size;
  var keys;
  var* key_buckets;
  var* val_buckets;
} DictionaryData;

/** Dictionary_New(var self); */
var Dictionary_New(var self, va_list* args);
var Dictionary_Delete(var self);
void Dictionary_Assign(var self, var obj);
var Dictionary_Copy(var self);

int Dictionary_Len(var self);
void Dictionary_Clear(var self);
var Dictionary_Contains(var self, var key);
void Dictionary_Discard(var self, var key);

var Dictionary_Get(var self, var key);
void Dictionary_Put(var self, var key, var val);

var Dictionary_Iter_Start(var self);
var Dictionary_Iter_End(var self);
var Dictionary_Iter_Next(var self, var curr);

instance(Dictionary, New) = { sizeof(DictionaryData), Dictionary_New, Dictionary_Delete };
instance(Dictionary, Assign) = { Dictionary_Assign };
instance(Dictionary, Copy) = { Dictionary_Copy };
instance(Dictionary, Collection) = { Dictionary_Len, Dictionary_Clear, Dictionary_Contains, Dictionary_Discard };
instance(Dictionary, Dict) = { Dictionary_Get, Dictionary_Put };
instance(Dictionary, Iter) = { Dictionary_Iter_Start, Dictionary_Iter_End, Dictionary_Iter_Next };

#endif
