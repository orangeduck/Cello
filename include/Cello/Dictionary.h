/*
** == Dictionary ==
**
**  Collection that maps keys to values
**
**  + Does not store keys or values, only holds references.
**  + Key objects require "Hash" and "Eq"
**  + Not responsible for object deallocation
*/

#ifndef CelloDictionary_h
#define CelloDictionary_h

#include "Prelude.h"
#include "Type.h"

global var Dictionary;

var Dictionary_New(var self, var_list vl);
var Dictionary_Delete(var self);
size_t Dictionary_Size(void);
void Dictionary_Assign(var self, var obj);
var Dictionary_Copy(var self);

var Dictionary_Eq(var self, var obj);

int Dictionary_Len(var self);
void Dictionary_Clear(var self);
var Dictionary_Contains(var self, var key);
void Dictionary_Discard(var self, var key);

var Dictionary_Get(var self, var key);
void Dictionary_Put(var self, var key, var val);

var Dictionary_Iter_Start(var self);
var Dictionary_Iter_End(var self);
var Dictionary_Iter_Next(var self, var curr);

int Dictionary_Show(var self, var output, int pos);

instance(Dictionary, New) = { Dictionary_New, Dictionary_Delete, Dictionary_Size };
instance(Dictionary, Assign) = { Dictionary_Assign };
instance(Dictionary, Copy) = { Dictionary_Copy };
instance(Dictionary, Eq) = { Dictionary_Eq };
instance(Dictionary, Collection) = { Dictionary_Len, Dictionary_Clear, Dictionary_Contains, Dictionary_Discard };
instance(Dictionary, Dict) = { Dictionary_Get, Dictionary_Put };
instance(Dictionary, Iter) = { Dictionary_Iter_Start, Dictionary_Iter_End, Dictionary_Iter_Next };
instance(Dictionary, Show) = { Dictionary_Show, NULL };

#endif
