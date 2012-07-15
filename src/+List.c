#include <math.h>
#include <assert.h>

#include "+List.h"

methods(List) = {
  methods_begin(List),
  method(List, New),
  method(List, Copy),
  method(List, Eq),
  method(List, Len),
  method(List, Empty),
  method(List, Push),
  method(List, At),
  method(List, Iter),
  methods_end(List)
};

Type List = methods_table(List);

void List_New(var self, va_list* args) {
  ListObject* lo = cast(self, List);
  
  lo->num_items = 0;
  lo->num_slots = 0;
  lo->items = malloc(sizeof(var) * lo->num_slots);
  
  int obj_count = va_arg(*args, int);
  
  for(int i = 0; i < obj_count; i++) {
    push(self, va_arg(*args, var));
  }
  
}

void List_Delete(var self) {
  ListObject* lo = cast(self, List);
  free(lo->items);
}

var List_Copy(var self) {
  
  var newlist = new(List, 0);
  
  foreach(self, val) {
    push(newlist, val);
  }
  
  return newlist;
  
}

bool List_Eq(var self, var other) {
  
  if (len(self) != len(other)) {
    return false;
  }
  
  for(int i = 0; i < len(self); i++) {
    if (neq( at(self,i) , at(other,i) )) {
      return false;
    }
  }
  
  return true;
}

size_t List_Len(var self) {
  ListObject* lo = cast(self, List);
  return lo->num_items;
} 

bool List_IsEmpty(var self) {
  ListObject* lo = cast(self, List);
  return (lo->num_items is 0);
}

void List_Clear(var self) {
  
  while(not is_empty(self)) {
    var discard = pop(self);
  }
  
}

static void List_Reserve_More(ListObject* lo) {
  
  if (lo->num_items > lo->num_slots) {
    if (lo->num_slots < 10) {
      lo->num_slots += 5;
    } else {
      lo->num_slots = pow(lo->num_slots, 1.5);
    }
    
    lo->items = realloc(lo->items, sizeof(var) * lo->num_slots);
  }

}

void List_Push_Back(var self, var val) {
  ListObject* lo = cast(self, List);
  lo->num_items++;
  List_Reserve_More(lo);
  
  lo->items[lo->num_items-1] = val;
}

void List_Push_Front(var self, var val) {
  List_Push_At(self, val, 0);
}

void List_Push_At(var self, var val, int index) {
  ListObject* lo = cast(self, List);
  lo->num_items++;
  List_Reserve_More(lo);
  
  for(int i = lo->num_items-1; i > index; i--) {
    lo->items[i] = lo->items[i-1];
  }
  
  lo->items[index] = val;
}

/* TODO: Free space when appropriate */

static void List_Reserve_Less(ListObject* lo) {
  
  //if ( pow(lo->num_items, 1.5) > lo->num_slots) {
  //  lo->num_slots = pow(lo->num_slots, 1.0/1.5);
  //  lo->items = realloc(lo->items, sizeof(var) * lo->num_slots);
  //}
  
  assert(lo->num_slots >= lo->num_items);
  
}

var List_Pop_Back(var self) {
  ListObject* lo = cast(self, List);
  
  var retval = lo->items[lo->num_items-1];
  
  lo->num_items--;
  List_Reserve_Less(lo);
  
  return retval;
}

var List_Pop_Front(var self) {
  return List_Pop_At(self, 0);
}

var List_Pop_At(var self, int index) {
  ListObject* lo = cast(self, List);
  
  var retval = lo->items[index];
  
  for(int i = index; i < lo->num_items; i++) {
    lo->items[i] = lo->items[i+1];
  }
  
  lo->num_items--;
  List_Reserve_Less(lo);
  
  return retval;
  
}

var List_At(var self, int index) {
  ListObject* lo = cast(self, List);
  return lo->items[index];
}

void List_Set(var self, int index, var val) {
  ListObject* lo = cast(self, List);
  lo->items[index] = val;
}

var List_Iter_Start(var self) {
  ListObject* lo = cast(self, List);
  return lo->items[0];
}

static const var LIST_ITER_END = (var)-1;

var List_Iter_End(var self) {
  return LIST_ITER_END;
}

var List_Iter_Next(var self, var curr) {
  
  for(int i = 0; i < len(self)-1; i++) {
    
    var val = at(self, i);
    if (val is curr) { return at(self, i+1); }
    
  }
  
  return LIST_ITER_END;
  
}