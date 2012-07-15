#ifndef PlusList_h
#define PlusList_h

#include "+Prelude.h"

extern Type List;

/* Members */

object {
  Type type;
  int num_items;
  int num_slots;
  var* items;
} ListObject;

/* Methods */

/* new(List, int count, var items ...) */
void List_New(var, va_list*);
void List_Delete(var);
var List_Copy(var);

bool List_Eq(var, var);

size_t List_Len(var);
bool List_IsEmpty(var);
void List_Clear(var);

void List_Push_Back(var, var);
void List_Push_Front(var, var);
void List_Push_At(var, var, int);

var List_Pop_Back(var);
var List_Pop_Front(var);
var List_Pop_At(var, int);

var List_At(var, int);
void List_Set(var, int, var);

var List_Iter_Start(var);
var List_Iter_End(var);
var List_Iter_Next(var, var);

/* Instances */

instance(List, New) = { sizeof(ListObject), List_New, List_Delete };
instance(List, Copy) = { List_Copy };
instance(List, Eq) = { List_Eq };
instance(List, Len) = { List_Len };
instance(List, Empty) = { List_IsEmpty, List_Clear };
instance(List, Push) = { List_Push_Back, List_Push_At, List_Push_Back, List_Push_Front, List_Pop_Back, List_Pop_At, List_Pop_Back, List_Pop_Front };
instance(List, At) = { List_At, List_Set };
instance(List, Iter) = { List_Iter_Start, List_Iter_End, List_Iter_Next };



#endif