#ifndef PlusList_h
#define PlusList_h

#include "+Prelude.h"

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

/* Interfaces */

implements(List, New) as { sizeof(ListObject), List_New, List_Delete };
implements(List, Copy) as { List_Copy };
implements(List, Eq) as { List_Eq };
implements(List, Len) as { List_Len };
implements(List, Empty) as { List_IsEmpty, List_Clear };
implements(List, Push) as { List_Push_Back, List_Push_At, List_Push_Back, List_Push_Front, List_Pop_Back, List_Pop_At, List_Pop_Back, List_Pop_Front };
implements(List, At) as { List_At, List_Set };
implements(List, Iter) as { List_Iter_Start, List_Iter_End, List_Iter_Next };

/* Type */

methods(List) as {
  method_begin(List),
  method(List, New), method(List, Copy),
  method(List, Eq), method(List, Len),
  method(List, Empty), method(List, Push),
  method(List, At), method(List, Iter),
  method_end(List),
};


#endif