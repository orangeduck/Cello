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

/** Array_New(var, var, int, ...) */
var Array_New(var, va_list*);
var Array_Delete(var);
void Array_Assign(var, var);
var Array_Copy(var);

bool Array_Eq(var, var);

int Array_Len(var);
bool Array_IsEmpty(var);
void Array_Clear(var);
bool Array_Contains(var, var);
void Array_Discard(var, var);

void Array_Append(var, var);

var Array_At(var, int);
void Array_Set(var, int, var);

var Array_Iter_Start(var);
var Array_Iter_End(var);
var Array_Iter_Next(var, var);

instance(Array, New) = { sizeof(ArrayData), Array_New, Array_Delete };
instance(Array, Assign) = { Array_Assign };
instance(Array, Copy) = { Array_Copy };
instance(Array, Eq) = { Array_Eq };
instance(Array, Collection) = { Array_Len, Array_IsEmpty, Array_Clear, Array_Contains, Array_Discard };
instance(Array, At) = { Array_At, Array_Set };
instance(Array, Iter) = { Array_Iter_Start, Array_Iter_End, Array_Iter_Next };

#endif