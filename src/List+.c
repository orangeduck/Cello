#include <math.h>
#include <assert.h>
#include <string.h>

#include "Bool+.h"
#include "None+.h"

#include "List+.h"

var List = methods {
  methods_begin(List),
  method(List, New),
  method(List, Assign),
  method(List, Copy),
  method(List, Eq),
  method(List, Collection),
  method(List, Push),
  method(List, At),
  method(List, Iter),
  method(List, Reverse),
  method(List, Append),
  method(List, Sort),
  methods_end(List)
};

var List_New(var self, va_list* args) {
  ListData* lo = cast(self, List);
  
  lo->num_items = 0;
  lo->num_slots = 0;
  lo->items = malloc(sizeof(var) * lo->num_slots);
  
  int obj_count = va_arg(*args, int);
  
  for(int i = 0; i < obj_count; i++) {
    push(self, va_arg(*args, var));
  }
  
  lo->cursor = 0;
  
  return self;
}

var List_Delete(var self) {
  ListData* lo = cast(self, List);
  free(lo->items);
  return self;
}

void List_Assign(var self, var obj) {
  ListData* lo = cast(self, List);
  
  lo->num_items = 0;
  lo->num_slots = 0;
  lo->items = realloc(lo->items, 0);
  lo->cursor = 0;
  
  foreach(obj, item) {
    push(self, item);
  }
}

var List_Copy(var self) {
  
  var newlist = new(List, 0);
  
  foreach(self, val) {
    push(newlist, val);
  }
  
  return newlist;
  
}
var List_Eq(var self, var other) {
  
  if (len(self) != len(other)) {
    return False;
  }
  
  for(int i = 0; i < len(self); i++) {
    if_neq( at(self,i) , at(other,i) ) {
      return False;
    }
  }
  
  return True;
}

int List_Len(var self) {
  ListData* lo = cast(self, List);
  return lo->num_items;
}

void List_Clear(var self) {
  
  while(not is_empty(self)) {
    var discard = pop(self);
  }
  
}

var List_Contains(var self, var obj) {
  ListData* lo = cast(self, List);
  foreach(self, item) {
    if_eq(item, obj) return True;
  }
  
  return False;
}

void List_Discard(var self, var obj) {
  ListData* lo = cast(self, List);
  for (int i = 0; i < len(self); i++) {
    if ( eq(at(self, i), obj) ) {
      pop_at(self, i);
      return;
    }
  }
}

static void List_Reserve_More(ListData* lo) {
  
  if (lo->num_items > lo->num_slots) {
    lo->num_slots = ceil((lo->num_slots + 1) * 1.5);
    lo->items = realloc(lo->items, sizeof(var) * lo->num_slots);
  }

}

void List_Push_Back(var self, var val) {

  ListData* lo = cast(self, List);
  lo->num_items++;
  List_Reserve_More(lo);
  
  lo->items[lo->num_items-1] = val;
}

void List_Push_Front(var self, var val) {
  List_Push_At(self, val, 0);
}

void List_Push_At(var self, var val, int index) {
  ListData* lo = cast(self, List);
  
  if (index < 0 or index > lo->num_items-1) return;
  
  lo->num_items++;
  List_Reserve_More(lo);
  
  memmove(&lo->items[index+1], 
          &lo->items[index], 
          sizeof(var) * ((lo->num_items-1) - index));
  
  lo->items[index] = val;
}

static void List_Reserve_Less(ListData* lo) {
  
  if ( lo->num_slots > pow(lo->num_items+1, 1.5)) {
    lo->num_slots = floor((lo->num_slots-1) * (1.0/1.5));
    lo->items = realloc(lo->items, sizeof(var) * lo->num_slots);
  }
  
}

var List_Pop_Back(var self) {
  ListData* lo = cast(self, List);
  
  if (is_empty(self)) return Undefined;

  var retval = lo->items[lo->num_items-1];
  
  lo->num_items--;
  List_Reserve_Less(lo);
  
  return retval;
}

var List_Pop_Front(var self) {
  return List_Pop_At(self, 0);
}

var List_Pop_At(var self, int index) {
  ListData* lo = cast(self, List);
  
  if (index < 0 or index > lo->num_items-1) {
    return Undefined;
  }
  
  var retval = lo->items[index];
  
  memmove(&lo->items[index], 
          &lo->items[index+1], 
          sizeof(var) * ((lo->num_items-1) - index));
  
  lo->num_items--;
  List_Reserve_Less(lo);
  
  return retval;
  
}

var List_At(var self, int index) {
  ListData* lo = cast(self, List);
  
  if (index < 0 or index > lo->num_items-1) {
    return Undefined;
  }
  
  return lo->items[index];
}

void List_Set(var self, int index, var val) {
  ListData* lo = cast(self, List);
  
  if (index < 0 or index > lo->num_items-1) {
    return;
  }
  
  lo->items[index] = val;
}

static const var LIST_ITER_END = (var)-1;

var List_Iter_Start(var self) {
  
  if (len(self) == 0) { return LIST_ITER_END; } 

  ListData* lo = cast(self, List);
  lo->cursor = 0;
  return lo->items[0];
}

var List_Iter_End(var self) {
  return LIST_ITER_END;
}

var List_Iter_Next(var self, var curr) {
  ListData* lo = cast(self, List);
  
  if (lo->items[lo->cursor] is curr) {
    lo->cursor++;
    if (lo->cursor == lo->num_items) {
      return LIST_ITER_END;
    } else {
      return lo->items[lo->cursor];
    }
  }
  
  for(int i = 0; i < len(self)-1; i++) {
    var val = at(self, i);
    
    if (val is curr) {
      lo->cursor = i+1;
      return at(self, i+1);
    }
  }
  
  return LIST_ITER_END;
  
}

static void List_Swap_Items(var self, int i0, int i1) {
  var lft = at(self, i0);
  var rht = at(self, i1);
  set(self, i0, rht);
  set(self, i1, lft);
}

void List_Reverse(var self) {
  for(int i = 0; i < len(self) / 2; i++) {
    List_Swap_Items(self, i, len(self)-1-i);
  }
}

static int List_Sort_Partition(var self, int left, int right, int pivot) {
  
  var pival = at(self, pivot);
  int fix = 0;
  
  List_Swap_Items(self, pivot, right);
  int storei = left;
  for(int i = left; i < right; i++) {
    if_lt( at(self, i) , pival ) {
      if(fix) {
        List_Swap_Items(self, i, storei);
      }
      storei++;
    } else {
      if (!fix) {
        fix = 1;
      }
    }
  }
  if ( fix ) {
    List_Swap_Items(self, storei, right);
  }
  
  return storei;
}

static void List_Sort_Part(var self, int left, int right) {
  if (left < right) {
    int pivot = left + (right-left) / 2;
    int newpivot = List_Sort_Partition(self, left, right, pivot);
    List_Sort_Part(self, left, newpivot-1);
    List_Sort_Part(self, newpivot+1, right);
  }
}

void List_Sort(var self) {
  List_Sort_Part(self, 0, len(self)-1);
}
