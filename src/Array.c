#include "Cello/Array.h"

#include "Cello/Bool.h"
#include "Cello/None.h"
#include "Cello/Exception.h"
#include "Cello/Number.h"

#include <math.h>
#include <string.h>

data {
  var type;
  var item_type;
  int num_items;
  int num_slots;
  var items;
} ArrayData;

var Array = type_data {
  type_begin(Array),
  type_entry(Array, New),
  type_entry(Array, Assign),
  type_entry(Array, Copy),
  type_entry(Array, Eq),
  type_entry(Array, Collection),
  type_entry(Array, Push),
  type_entry(Array, At),
  type_entry(Array, Iter),
  type_entry(Array, Reverse),
  type_entry(Array, Append),
  type_entry(Array, Sort),
  type_entry(Array, Show),
  type_end(Array)
};

var Array_New(var self, var_list vl) {
  
  ArrayData* ad = cast(self, Array);
  ad->item_type = cast(var_list_get(vl), Type);
  ad->num_items = 0;
  ad->num_slots = 0;
  ad->items = NULL;
  
  while(not var_list_end(vl)) {
    push(self, var_list_get(vl));
  }
  
  return self;
}

var Array_Delete(var self) {
  ArrayData* ad = cast(self, Array);

  foreach(item in self) {
    destruct(item);
  }
  free(ad->items);
  
  return self;
}

size_t Array_Size(void) {
  return sizeof(ArrayData);
}

void Array_Assign(var self, var obj) {
  clear(self);
  
  foreach(item in obj) {
    push(self, item);
  }
}

var Array_Copy(var self) {
  ArrayData* ad = cast(self, Array);
  
  var newarray = new(Array, ad->item_type);

  foreach(obj in self) {
    push(newarray, obj);
  }
  
  return newarray;
}

var Array_Eq(var self, var obj) {
  
  if (len(self) != len(obj)) { return False; }
  
  for(int i = 0; i < len(self); i++) {
    if_neq( at(self,i) , at(obj,i) ) {
      return False;
    }
  }
  
  return True;
}

int Array_Len(var self) {
  ArrayData* ad = cast(self, Array);
  return ad->num_items;
}

void Array_Clear(var self) {
  ArrayData* ad = cast(self, Array);
  
  foreach(item in self) {
    destruct(item);
  }
  
  ad->items = realloc(ad->items, 0);
  ad->num_items = 0;
  ad->num_slots = 0;
  
}

var Array_Contains(var self, var obj) {
  foreach(item in self) {
    if_eq(item, obj) {
      return True;
    }
  }
  
  return False;
}

void Array_Discard(var self, var obj) {
  for(int i = 0; i < len(self); i++) {
    if_eq(at(self, i), obj) {
      pop_at(self, i);
      return;
    }
  }
}

local void Array_Reserve_More(ArrayData* ad) {
  
  if (ad->num_items > ad->num_slots) {
    int old_size = ad->num_slots;
    ad->num_slots = ceil((ad->num_slots + 1) * 1.5);
    ad->items = realloc(ad->items, size(ad->item_type) * ad->num_slots);
    if (ad->items == NULL) { throw(OutOfMemoryError, "Cannot grow Array, out of memory!"); }
  }

}

local void Array_Set_Type_At(ArrayData* ad, int i) {
  
  memset(ad->items + (size(ad->item_type) * i), 0, size(ad->item_type));
  ObjectData* template = ad->items + (size(ad->item_type) * i);
  template->type = ad->item_type;
  
}

void Array_Push_Back(var self, var obj) {
  ArrayData* ad = cast(self, Array);
  ad->num_items++;
  Array_Reserve_More(ad);
  
  Array_Set_Type_At(self, ad->num_items-1);
  set(self, ad->num_items-1, obj);
  
}

void Array_Push_Front(var self, var obj) {
  Array_Push_At(self, obj, 0);
}

void Array_Push_At(var self, var obj, int index) {
  ArrayData* ad = cast(self, Array);
  ad->num_items++;
  Array_Reserve_More(ad);
  
  memmove(ad->items + size(ad->item_type) * (index+1), 
          ad->items + size(ad->item_type) * index, 
          size(ad->item_type) * ((ad->num_items-1) - index));
  
  Array_Set_Type_At(self, index);
  set(self, index, obj);
}

local void Array_Reserve_Less(ArrayData* ad) {
  
  if ( ad->num_slots > pow(ad->num_items+1, 1.5)) {
    ad->num_slots = floor((ad->num_slots-1) * (1.0/1.5));
    ad->items = realloc(ad->items, size(ad->item_type) * ad->num_slots);
  }
  
}

var Array_Pop_Back(var self) {
  ArrayData* ad = cast(self, Array);

  if (is_empty(self)) {
    return throw(IndexOutOfBoundsError, "Cannot pop back. Array is empty!");
  }
  
  destruct(at(self, len(self)-1));
  
  ad->num_items--;
  Array_Reserve_Less(ad);
  
  return Undefined;
}

var Array_Pop_Front(var self) {
  return Array_Pop_At(self, 0);
}

var Array_Pop_At(var self, int index) {
  ArrayData* ad = cast(self, Array);
  
  if (is_empty(self)) {
    return throw(IndexOutOfBoundsError, "Cannot pop at %i. Array is empty!", $(Int, index));
  }
  
  destruct(at(self, index));
  
  memmove(ad->items + size(ad->item_type) * index, 
          ad->items + size(ad->item_type) * (index+1), 
          size(ad->item_type) * ((ad->num_items-1) - index));
  
  ad->num_items--;
  Array_Reserve_Less(ad);
  
  return Undefined;
}

var Array_At(var self, int i) {
  if (i < 0 or i >= len(self)) {
    return throw(IndexOutOfBoundsError,
      "Index %i out of bounds [%i-%i]", 
      $(Int, i), $(Int, 0), $(Int, len(self)));
  }
  
  ArrayData* ad = cast(self, Array);
  return ad->items + size(ad->item_type) * i;
}

void Array_Set(var self, int i, var obj) {
  if (i < 0 or i >= len(self)) {
    throw(IndexOutOfBoundsError, 
      "Index %i out of bounds [%i-%i]", 
      $(Int, i), $(Int, 0), $(Int, len(self)));
    return;
  }
  
  ArrayData* ad = cast(self, Array);  
  assign(ad->items + size(ad->item_type) * i, obj);
}

var Array_Iter_Start(var self) {

  if (len(self) == 0) { return Iter_End; }

  ArrayData* ad = cast(self, Array);
  return ad->items;
}

var Array_Iter_End(var self) {
  return Iter_End;
}

var Array_Iter_Next(var self, var curr) {
  ArrayData* ad = cast(self, Array);
  
  if (curr >= ad->items + size(ad->item_type) * (ad->num_items-1)) {
    return Iter_End;
  } else {
    return curr + size(ad->item_type);
  }
}

local void Array_Swap_Items(var self, var temp, int i0, int i1) {
  assign(temp, at(self, i0));
  set(self, i0, at(self, i1));
  set(self, i1, temp);
}

void Array_Reverse(var self) {
  ArrayData* ad = cast(self, Array);
  
  var temp = allocate(ad->item_type);
  
  for(int i = 0; i < len(self) / 2; i++) {
    Array_Swap_Items(self, temp, i, len(self)-1-i);
  }
  
  delete(temp);
}

local int Array_Sort_Partition(var self, int left, int right, int pivot) {
  
  ArrayData* ad = cast(self, Array); 
  
  var pival = allocate(ad->item_type);
  var temp = allocate(ad->item_type);
  
  assign(pival, at(self, pivot));

  Array_Swap_Items(self, temp, pivot, right);
  
  int storei = left;
  bool fix = false;
  
  for (int i = left; i < right; i++) {
    if_lt ( at(self, i) , pival ) {
      if (fix) {
        Array_Swap_Items(self, temp, i, storei);
      }
      storei++;
    } else {
      fix = true;
    }
  }
  
  if (fix) {
    Array_Swap_Items(self, temp, storei, right);
  }

  deallocate(temp);
  deallocate(pival);
  
  return storei;
}

local void Array_Sort_Part(var self, int left, int right) {
  if (left < right) {
    int pivot = left + (right - left) / 2;
    int newpivot = Array_Sort_Partition(self, left, right, pivot);
    Array_Sort_Part(self, left, newpivot-1);
    Array_Sort_Part(self, newpivot+1, right);
  }
}

void Array_Sort(var self) {
  Array_Sort_Part(self, 0, len(self)-1);
}

int Array_Show(var self, var output, int pos) {
  pos = print_to(output, pos, "<'Array' At 0x%p [", self);
  for(int i = 0; i < len(self); i++) {
    pos = print_to(output, pos, "%$", at(self, i));
    if (i < len(self)-1) { pos = print_to(output, pos, ", "); }
  }
  pos = print_to(output, pos, "]>");
  return pos;
}

