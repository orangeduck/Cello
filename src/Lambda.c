#include "Cello/Lambda.h"

#include "Cello/List.h"
#include "Cello/Bool.h"
#include "Cello/None.h"
#include "Cello/Number.h"

void map(var self, var func) {
  foreach(item in self) {
    call(func, item);
  }
}

var new_map(var self, var func) {
  var ret = new(List);
  foreach(item in self) {
    var val = call(func, item);
    push(ret, val);
  }
  return ret;
}

var new_filter(var self, var eq_func) {
  var ret = new(List);
  foreach(item in self) {
    if (call(eq_func, item)) {
      push(ret, item);
    }
  }
  return ret;
}

var new_foldl(var self, var func, var base) {
  var total = copy(base);
  foreach(item in self) {
    call(func, total, item);
  }
  return total;
}

var new_foldr(var self, var func, var base) {
  var total = copy(base);
  for(int i = len(self)-1; i >= 0; i--) {
    call(func, total, at(self, i));
  }
  return total;
}

var new_sum(var self) {
  var total = new(Real, $(Real, 0.0));
  foreach(item in self) {
    add(total, item);
  }
  return total;
}

var new_product(var self) {
  var total = new(Real, $(Real, 1.0));
  foreach(item in self) {
    mul(total, item);
  }
  return total;
}
