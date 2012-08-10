
#include "Closure+.h"
#include "List+.h"
#include "NoneType+.h"
#include "Number+.h"

void map(var self, var func) {
  foreach(self, item) {
    call(func, item);
  }
}

void filter(var self, var eq_func) {
  var to_discard = new(List, 0);
  
  foreach(self, item) {
    if ( call(eq_func, item) is None ) {
      push(to_discard, item);
    }
  }
  
  foreach(to_discard, item) {
    discard(self, item);
  }
  
  delete(to_discard);
}

void sort(var self, var gt_func) {
  //TODO: Implement
}

var maximum(var self) {
  var best = at(self, 0);
  foreach(self, item) {
    if (lt(item, best)) {
      best = item;
    }
  }
  return best;
}

var minimum(var self) {
  var best = at(self, 0);
  foreach(self, item) {
    if (gt(item, best)) {
      best = item;
    }
  }
  return best;
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

var new_sort(var self, var gt_func) {
  // TODO: Implement
  return None;
}

var new_foldl(var self, var func, var base) {
  // TODO: Implement
  return None;
}

var new_foldr(var self, var func, var base) {
  // TODO: Implement
  return None;
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
