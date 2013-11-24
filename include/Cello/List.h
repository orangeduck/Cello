/*
** == List ==
**
**  Collection of data objects
**
**  + A dynamically sized list of data objects
**  + Contains references to data objects
**  + Is not responsible object deallocation
*/

#ifndef CelloList_h
#define CelloList_h

#include "Prelude.h"
#include "Type.h"
#include "Format.h"

global var List;

var List_New(var self, var_list vl);
var List_Delete(var self);
size_t List_Size(void);
void List_Assign(var self, var obj);
var List_Copy(var self);

var List_Eq(var self, var obj);

int List_Len(var self);
void List_Clear(var self);
var List_Contains(var self, var obj);
void List_Discard(var self, var obj);

void List_Push_Back(var self, var obj);
void List_Push_Front(var self, var obj);
void List_Push_At(var self, var obj, int i);

var List_Pop_Back(var self);
var List_Pop_Front(var self);
var List_Pop_At(var self, int i);

var List_At(var self, int i);
void List_Set(var self, int i, var obj);

var List_Iter_Start(var self);
var List_Iter_End(var self);
var List_Iter_Next(var self, var curr);

void List_Reverse(var self);
void List_Sort(var self);

int List_Show(var self, var output, int pos);

instance(List, New) = { List_New, List_Delete, List_Size };
instance(List, Assign) = { List_Assign };
instance(List, Copy) = { List_Copy };
instance(List, Eq) = { List_Eq };
instance(List, Collection) = { List_Len, List_Clear, List_Contains, List_Discard };
instance(List, Push) = { List_Push_Back, List_Push_At, List_Push_Back, List_Push_Front, List_Pop_Back, List_Pop_At, List_Pop_Back, List_Pop_Front };
instance(List, At) = { List_At, List_Set };
instance(List, Iter) = { List_Iter_Start, List_Iter_End, List_Iter_Next };
instance(List, Reverse) = { List_Reverse };
instance(List, Sort) = { List_Sort };
instance(List, Append) = { List_Push_Back };
instance(List, Show) = { List_Show, NULL };


#endif
