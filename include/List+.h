#ifndef ListPlus_h
#define ListPlus_h

#include "Prelude+.h"
#include "Type+.h"

module List;

data {
  var type;
  int num_items;
  int num_slots;
  var* items;
} ListData;

/* new(List, int count, var items ...) */
var List_New(var, va_list*);
var List_Delete(var);
var List_Copy(var);

bool List_Eq(var, var);

int List_Len(var);
bool List_IsEmpty(var);
void List_Clear(var);
bool List_Contains(var, var);
void List_Discard(var, var);

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

instance(List, New) = { sizeof(ListData), List_New, List_Delete };
instance(List, Copy) = { List_Copy };
instance(List, Eq) = { List_Eq };
instance(List, Collection) = { List_Len, List_IsEmpty, List_Clear, List_Contains, List_Discard };
instance(List, Push) = { List_Push_Back, List_Push_At, List_Push_Back, List_Push_Front, List_Pop_Back, List_Pop_At, List_Pop_Back, List_Pop_Front };
instance(List, At) = { List_At, List_Set };
instance(List, Iter) = { List_Iter_Start, List_Iter_End, List_Iter_Next };



#endif