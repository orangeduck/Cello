#include "Cello/Pool.h"

#include "Cello/Reference.h"
#include "Cello/Number.h"

var retain(var p, var x) {
  return type_class_method(type_of(p), Retain, retain, p, x);
}

void release(var p, var x) {
  type_class_method(type_of(p), Retain, release, p, x);
}

data {
  var type;
  var tab;
} PoolData;

var Pool = type_data {
  type_begin(Pool),
  type_entry(Pool, New), 
  type_entry(Pool, Retain),
  type_entry(Pool, Collection),
  type_entry(Pool, Dict),
  type_end(Pool)
};

var Pool_New(var self, var_list vl) {
  PoolData* pd = cast(self, Pool);
  pd->tab = new(Table, Reference, Int);
  return self;
}

var Pool_Delete(var self) {
  PoolData* pd = cast(self, Pool);

  foreach(k in pd->tab) {
    delete(at(k,0));
  }  

  delete(pd->tab);
  return self;
}

size_t Pool_Size(void) {
  return sizeof(PoolData);
}

var Pool_Retain(var self, var x) {
  
  if (contains(self, x)) {
    var c = get(self, x);
    add(c, $(Int, 1));
    return x;
  } else {
    put(self, x, $(Int, 1));
    return x;
  }
  
}

void Pool_Release(var self, var x) {

  if (contains(self, x)) {
  
    var c = get(self, x);
    sub(c, $(Int, 1));
    
    if_le(c, $(Int, 0)) {
      discard(self, x);
      delete(x);
    }
  
  }

}

int Pool_Len(var self) {
  PoolData* pd = cast(self, Pool);
  return len(pd->tab);
}

void Pool_Clear(var self) {
  PoolData* pd = cast(self, Pool);
  clear(pd->tab);
}

var Pool_Contains(var self, var x) {
  PoolData* pd = cast(self, Pool);
  return contains(pd->tab, $(Reference, x));
}

void Pool_Discard(var self, var x) {
  PoolData* pd = cast(self, Pool);
  discard(pd->tab, $(Reference, x));
}

var Pool_Get(var self, var k) {
  PoolData* pd = cast(self, Pool);
  return get(pd->tab, $(Reference, k));
}

void Pool_Put(var self, var k, var v) {
  PoolData* pd = cast(self, Pool);
  put(pd->tab, $(Reference, k), v);
}

