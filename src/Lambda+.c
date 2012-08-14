
#include "Lambda+.h"
#include "List+.h"
#include "NoneType+.h"
#include "Number+.h"

void map(var self, var func) {
  foreach(self, item) {
    call(func, item);
  }
}

var new_map(var self, var func) {
  var ret = new(List, 0);
  foreach(self, item) {
    var val = call(func, item);
    push(ret, val);
  }
  return ret;
}

var new_filter(var self, var eq_func) {
  var ret = new(List, 0);
  foreach(self, item) {
    if (call(eq_func, item) != None) {
      push(ret, item);
    }
  }
  return ret;
}

var new_foldl(var self, var func, var base) {
  var total = copy(base);
  foreach(self, item) {
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
  var total = new(Real, 0);
  foreach(self, item) {
    add(total, item);
  }
  return total;
}

var new_product(var self) {
  var total = new(Real, 1);
  foreach(self, item) {
    mul(total, item);
  }
  return total;
}
