/*
** == Array ==
**
**  Container of data objects
**
**  + Does not contains references
**  + Allocates space for data objects
**  + Uses "Assign" to set contents
*/

#ifndef CelloArray_h
#define CelloArray_h

#include "Prelude.h"
#include "Type.h"
#include "Format.h"

global var Array;

var Array_New(var self, var_list vl);
var Array_Delete(var self);
size_t Array_Size(void);
void Array_Assign(var self, var obj);
var Array_Copy(var self);

var Array_Eq(var self, var obj);

int Array_Len(var self);
void Array_Clear(var self);
var Array_Contains(var self, var obj);
void Array_Discard(var self, var obj);

void Array_Push_Back(var self, var obj);
void Array_Push_Front(var self, var obj);
void Array_Push_At(var self, var obj, int i);

var Array_Pop_Back(var self);
var Array_Pop_Front(var self);
var Array_Pop_At(var self, int i);

var Array_At(var self, int i);
void Array_Set(var self, int i, var obj);

var Array_Iter_Start(var self);
var Array_Iter_End(var self);
var Array_Iter_Next(var self, var curr);

void Array_Reverse(var self);
void Array_Sort(var self);

int Array_Show(var self, var output, int pos);

instance(Array, New) = { Array_New, Array_Delete, Array_Size };
instance(Array, Assign) = { Array_Assign };
instance(Array, Copy) = { Array_Copy };
instance(Array, Eq) = { Array_Eq };
instance(Array, Collection) = { Array_Len, Array_Clear, Array_Contains, Array_Discard };
instance(Array, Push) = { Array_Push_Back, Array_Push_At, Array_Push_Back, Array_Push_Front, Array_Pop_Back, Array_Pop_At, Array_Pop_Back, Array_Pop_Front };
instance(Array, At) = { Array_At, Array_Set };
instance(Array, Iter) = { Array_Iter_Start, Array_Iter_End, Array_Iter_Next };
instance(Array, Reverse) = { Array_Reverse };
instance(Array, Sort) = { Array_Sort };
instance(Array, Append) = { Array_Push_Back };
instance(Array, Show) = { Array_Show, NULL };

#endif
