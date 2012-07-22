/*
** Array
**
**  Single typed dynamically sized list of objects
**  Does not contains references But allocates actual objects
**  Constructor and Set use Assign on arguments
**  Is responsible for deallocation of contents
*/

#ifndef ArrayPlus_h
#define ArrayPlus_h

#include "Prelude+.h"
#include "Type+.h"

module Array;

data {
  var type;
  var item_type;
  int num_items;
  int num_slots;
  var items;
} ArrayData;

/** Array_New(var self, var type, int count, ... items) */
var Array_New(var self, va_list* args);
var Array_Delete(var self);
void Array_Assign(var self, var obj);
var Array_Copy(var self);

bool Array_Eq(var self, var obj);

int Array_Len(var self);
bool Array_IsEmpty(var self);
void Array_Clear(var self);
bool Array_Contains(var self, var obj);
void Array_Discard(var self, var obj);

void Array_Append(var self, var obj);

var Array_At(var self, int i);
void Array_Set(var self, int i, var obj);

var Array_Iter_Start(var self);
var Array_Iter_End(var self);
var Array_Iter_Next(var self, var curr);

instance(Array, New) = { sizeof(ArrayData), Array_New, Array_Delete };
instance(Array, Assign) = { Array_Assign };
instance(Array, Copy) = { Array_Copy };
instance(Array, Eq) = { Array_Eq };
instance(Array, Collection) = { Array_Len, Array_IsEmpty, Array_Clear, Array_Contains, Array_Discard };
instance(Array, At) = { Array_At, Array_Set };
instance(Array, Iter) = { Array_Iter_Start, Array_Iter_End, Array_Iter_Next };

#endif