/*
** == Pool ==
**
**  Pool based reference memory management
**  Allows for reference counting via
**  "retain" and "release"
**
*/
#ifndef CelloPool_h
#define CelloPool_h

#include "Table.h"
#include "Reference.h"

class {
  var (*retain)(var p, var x);
  void (*release)(var p, var x);
} Retain;

var retain(var p, var x);
void release(var p, var x);

/* Pool Type */

global var Pool;

var Pool_New(var self, var_list vl);
var Pool_Delete(var self);
size_t Pool_Size(void);

var Pool_Retain(var self, var x);
void Pool_Release(var self, var x);

int Pool_Len(var self);
void Pool_Clear(var self);
var Pool_Contains(var self, var x);
void Pool_Discard(var self, var x);

var Pool_Get(var self, var k);
void Pool_Put(var self, var k, var v);

instance(Pool, New) = { Pool_New, Pool_Delete, Pool_Size };
instance(Pool, Retain) = { Pool_Retain, Pool_Release };
instance(Pool, Collection) = { Pool_Len, Pool_Clear, Pool_Contains, Pool_Discard };
instance(Pool, Dict) = { Pool_Get, Pool_Put };

#endif
