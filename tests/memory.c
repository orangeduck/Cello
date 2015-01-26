#include "Cello.h"
#include "ptest.h"

PT_FUNC(test_new) {
  
  var x = new_$(Int, 0);
  del(x);
  
}

PT_FUNC(test_box) {
  
  var x = new_$(Int, 1);
  var y = new_$(Int, 2);
  
  var rx = $(Box, x);
  
  PT_ASSERT(rx isnt x);
  PT_ASSERT(x isnt y);
  PT_ASSERT(neq(x, y));
  PT_ASSERT(x is deref(rx));
  PT_ASSERT(eq(x, deref(rx)));
  
  ref(rx, y);

  PT_ASSERT(y is deref(rx));
  PT_ASSERT(eq(y, deref(rx)));

  del(x);
  del(y);
  
}

PT_FUNC(test_box_with) {
  
  with(r in $(Box, new_$(String, "Almost like an Auto Ptr"))) {
    
    PT_ASSERT(eq(deref(r), $(String, "Almost like an Auto Ptr")));
    PT_ASSERT(neq(deref(r), $(String, "Blah")));
    
  }
  
}

PT_FUNC(test_box_with_many) {
  
  with(liferef0 in $(Box, new_$(String, "Life is Long")))
  with(liferef1 in $(Box, new_$(String, "Life is Beautiful")))
  with(liferef2 in $(Box, new_$(String, "Life is Grand"))) {
    
    PT_ASSERT(eq(deref(liferef0), $(String, "Life is Long")));
    PT_ASSERT(eq(deref(liferef1), $(String, "Life is Beautiful")));
    PT_ASSERT(eq(deref(liferef2), $(String, "Life is Grand")));
  
  }

}

/*
PT_FUNC(test_pool) {
  
  var p = new(Pool);
  
  var x = retain(p, new(String, $(String, "Hello Everyone!")));
  var y = retain(p, new(String, $(String, "Bonjour")));
  var z = retain(p, new(Int, $(Int, 3)));
  
  PT_ASSERT(contains(p, x));
  PT_ASSERT(contains(p, y));
  PT_ASSERT(contains(p, z));
  
  retain(p, x);
  PT_ASSERT(eq(get(p, x), $(Int, 2)));
  PT_ASSERT(eq(get(p, y), $(Int, 1)));
  PT_ASSERT(eq(get(p, z), $(Int, 1)));
  release(p, x);
  
  PT_ASSERT(eq(get(p, x), $(Int, 1)));
  PT_ASSERT(eq(get(p, y), $(Int, 1)));
  PT_ASSERT(eq(get(p, z), $(Int, 1)));
  
  delete(p);
  
}
*/

PT_SUITE(suite_memory) {
  
  PT_REG(test_new);
  PT_REG(test_box);
  PT_REG(test_box_with);
  PT_REG(test_box_with_many);
  //PT_REG(test_pool);

}
