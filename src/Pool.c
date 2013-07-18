#include "Cello/Pool.h"

#include "Cello/Reference.h"
#include "Cello/Number.h"

#include <assert.h>

var retain(var p, var x) {
  Retain* iretain = type_class(type_of(p), Retain);
  assert(iretain->retain);
  return iretain->retain(p, x);
}

void release(var p, var x) {
  Retain* iretain = type_class(type_of(p), Retain);
  assert(iretain->release);
  iretain->release(p, x);
}

var Pool = methods {
  methods_begin(Pool),
  method(Pool, New), 
  method(Pool, Retain),
  method(Pool, Collection),
  method(Pool, Dict),
  methods_end(Pool)
};

var Pool_New(var self, va_list* args) {
  PoolData* pd = cast(self, Pool);
  pd->tab = new(Table, Reference, Int);
  return self;
}

var Pool_Delete(var self) {
  PoolData* pd = cast(self, Pool);
  delete(pd->tab);
  return self;
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

