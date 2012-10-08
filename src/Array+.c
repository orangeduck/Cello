#include <math.h>
#include <assert.h>
#include <string.h>

#include "Bool+.h"
#include "None+.h"

#include "Array+.h"

var Array = methods {
  methods_begin(Array),
  method(Array, New),
  method(Array, Assign),
  method(Array, Copy),
  method(Array, Eq),
  method(Array, Collection),
  method(Array, Push),
  method(Array, At),
  method(Array, Iter),
  methods_end(Array)
};

var Array_New(var self, va_list* args) {
  
  ArrayData* ad = cast(self, Array);
  ad->item_type = cast(va_arg(*args, var), Type);
  ad->num_items = 0;
  ad->num_slots = 0;
  ad->items = malloc(0);
  
  int count = va_arg(*args, int);
  for(int i = 0; i < count; i++) {
    push(self, va_arg(*args, var));
  }
  
  return self;
}

var Array_Delete(var self) {
  
  foreach(self, item) {
    destruct(item);
  }
  
  return self;
}

void Array_Assign(var self, var obj) {
  clear(self);
  
  foreach(obj, item) {
    push(self, item);
  }
}

var Array_Copy(var self) {
  ArrayData* ad = cast(self, Array);
  
  var newarray = new(Array, ad->item_type, 0);

  foreach(self, obj) {
    push(newarray, obj);
  }
  
  return newarray;
}

var Array_Eq(var self, var obj) {
  
  if (len(self) != len(obj)) {
    return False;
  }
  
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
  
  foreach(self, item) {
    destruct(item);
  }
  
  ad->items = realloc(ad->items, 0);
  ad->num_items = 0;
  ad->num_slots = 0;
  
}

var Array_Contains(var self, var obj) {
  foreach(self, item) {
    if_eq(item, obj) {
      return True;
    }
  }
  
  return False;
}

void Array_Discard(var self, var obj) {
  for(int i = 0; i < len(self); i++) {
    var item = at(self, i);
    if_eq(item, obj) {
      pop_at(self, i);
      return;
    }
  }
}

static void Array_Reserve_More(ArrayData* ad) {
  
  if (ad->num_items > ad->num_slots) {
    int old_size = ad->num_slots;
    ad->num_slots = ceil((ad->num_slots + 1) * 1.5);
    New* inew = type_class(ad->item_type, New);
    ad->items = realloc(ad->items, inew->size * ad->num_slots);
    
    memset(ad->items + old_size * inew->size, 0, (ad->num_slots - old_size) * inew->size);
  }

}

static void Array_Set_Type_At(ArrayData* ad, int i) {
  
  New* inew = type_class(ad->item_type, New);
  
  ObjectData* template = ad->items + (inew->size * i);
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
  
  New* inew = type_class(ad->item_type, New);
  memmove(ad->items + inew->size * (index+1), 
          ad->items + inew->size * index, 
          inew->size * ((ad->num_items-1) - index));
  
  Array_Set_Type_At(self, index);
  set(self, index, obj);
}

static void Array_Reserve_Less(ArrayData* ad) {
  
  if ( ad->num_slots > pow(ad->num_items+1, 1.5)) {
    ad->num_slots = floor((ad->num_slots-1) * (1.0/1.5));
    New* inew = type_class(ad->item_type, New);
    ad->items = realloc(ad->items, inew->size * ad->num_slots);
  }
  
}

var Array_Pop_Back(var self) {
  ArrayData* ad = cast(self, Array);

  if (is_empty(self)) return None;
  
  destruct(at(self, len(self)-1));
  
  ad->num_items--;
  Array_Reserve_Less(ad);
  
  return None;
}

var Array_Pop_Front(var self) {
  return Array_Pop_At(self, 0);
}

var Array_Pop_At(var self, int index) {
  ArrayData* ad = cast(self, Array);
  
  if (is_empty(self)) return None;
  
  destruct(at(self, index));
  
  New* inew = type_class(ad->item_type, New);
  memmove(ad->items + inew->size * index, 
          ad->items + inew->size * (index+1), 
          inew->size * ((ad->num_items-1) - index));
  
  ad->num_items--;
  Array_Reserve_Less(ad);
  
  return None;
}

var Array_At(var self, int i) {
  if (i < 0 or i >= len(self)) return None;
  
  ArrayData* ad = cast(self, Array);
  New* inew = type_class(ad->item_type, New);
  return ad->items + inew->size * i;
}

void Array_Set(var self, int i, var obj) {
  if (i < 0 or i >= len(self)) return;
  
  ArrayData* ad = cast(self, Array);
  New* inew = type_class(ad->item_type, New);
  
  assign(ad->items + inew->size * i, obj);
}

static const var ARRAY_ITER_END = (var)-1;

var Array_Iter_Start(var self) {

  if (len(self) == 0) { return ARRAY_ITER_END; }

  ArrayData* ad = cast(self, Array);
  return ad->items;
}

var Array_Iter_End(var self) {
  return ARRAY_ITER_END;
}

var Array_Iter_Next(var self, var curr) {
  ArrayData* ad = cast(self, Array);
  New* inew = type_class(ad->item_type, New);
  
  if (curr >= ad->items + inew->size * (ad->num_items-1)) {
    return ARRAY_ITER_END;
  } else {
    return curr + inew->size;
  }
}
