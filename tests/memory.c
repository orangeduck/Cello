
#include "ptest.h"
#include "Cello.h"

PT_FUNC(test_new) {
  
  var x = new$(Int, 0);
  delete(x);
  
}

PT_FUNC(test_box) {
  
  var x = new$(Int, 1);
  var y = new$(Int, 2);
  
  var rx = $(Box, x);
  
  PT_ASSERT(rx isnt x);
  PT_ASSERT(x isnt y);
  PT_ASSERT(neq(x, y));
  PT_ASSERT(x is unbox(rx));
  PT_ASSERT(eq(x, unbox(rx)));
  
  box(rx, y);

  PT_ASSERT(y is unbox(rx));
  PT_ASSERT(eq(y, unbox(rx)));

  delete(x);
  delete(y);
  
}

PT_FUNC(test_box_with) {
  
  with(r in $(Box, new$(String, "Almost like an Auto Ptr"))) {
    
    PT_ASSERT(eq(unbox(r), $(String, "Almost like an Auto Ptr")));
    PT_ASSERT(neq(unbox(r), $(String, "Blah")));
    
  }
  
}

PT_FUNC(test_box_with_many) {
  
  with(liferef0 in $(Box, new$(String, "Life is Long")))
  with(liferef1 in $(Box, new$(String, "Life is Beautiful")))
  with(liferef2 in $(Box, new$(String, "Life is Grand"))) {
    
    PT_ASSERT(eq(unbox(liferef0), $(String, "Life is Long")));
    PT_ASSERT(eq(unbox(liferef1), $(String, "Life is Beautiful")));
    PT_ASSERT(eq(unbox(liferef2), $(String, "Life is Grand")));
  
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
