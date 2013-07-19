/*
** == Table ==
**
**  Container that stores keys and values
**
**  + Allocates space for key and value objects
**  + Key objects require "Hash" and "Eq" 
**  + Uses "Assign" to set data contents
*/

#ifndef CelloTable_h
#define CelloTable_h

#include "Prelude.h"
#include "Type.h"

global var Table;

data {
  var type;
  var key_type;
  var val_type;
  int size;
  var keys;
  var* key_buckets;
  var* val_buckets;
} TableData;

/** Table_New(var self, var key_type, var item_type); */
var Table_New(var self, va_list* args);
var Table_Delete(var self);
void Table_Assign(var self, var obj);
var Table_Copy(var self);

var Table_Eq(var self, var obj);

int Table_Len(var self);
void Table_Clear(var self);
var Table_Contains(var self, var key);
void Table_Discard(var self, var key);

var Table_Get(var self, var key);
void Table_Put(var self, var key, var val);

var Table_Iter_Start(var self);
var Table_Iter_End(var self);
var Table_Iter_Next(var self, var curr);

int Table_Show(var self, var output, int pos);

instance(Table, New) = { sizeof(TableData), Table_New, Table_Delete };
instance(Table, Assign) = { Table_Assign };
instance(Table, Copy) = { Table_Copy };
instance(Table, Eq) = { Table_Eq };
instance(Table, Collection) = { Table_Len, Table_Clear, Table_Contains, Table_Discard };
instance(Table, Dict) = { Table_Get, Table_Put };
instance(Table, Iter) = { Table_Iter_Start, Table_Iter_End, Table_Iter_Next };
instance(Table, Show) = { Table_Show, NULL };


#endif

