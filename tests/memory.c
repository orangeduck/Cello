
#include "ptest.h"
#include "Cello.h"

PT_FUNC(test_new) {
  
  var x = new(Int, $(Int, 0));
  delete(x);
  
}

PT_FUNC(test_reference) {
  
  var x = new(Int, $(Int, 1));
  var y = new(Int, $(Int, 2));
  
  var rx = $(Reference, x);
  
  PT_ASSERT(rx isnt x);
  PT_ASSERT(x isnt y);
  PT_ASSERT(neq(x, y));
  PT_ASSERT(x is at(rx,0));
  PT_ASSERT(eq(x, at(rx,0)));
  
  set(rx, 0, y);

  PT_ASSERT(y is at(rx,0));
  PT_ASSERT(eq(y, at(rx,0)));

  delete(x);
  delete(y);
  
}

PT_FUNC(test_reference_with) {
  
  with(r in $(Reference, new(String, $(String, "Almost like an Auto Ptr")))) {
    
    PT_ASSERT(eq(at(r,0), $(String, "Almost like an Auto Ptr")));
    PT_ASSERT(neq(at(r,0), $(String, "Blah")));
    
  }
  
}

PT_FUNC(test_reference_with_many) {
  
  with(liferef0 in $(Reference, new(String, $(String, "Life is Long"))))
  with(liferef1 in $(Reference, new(String, $(String, "Life is Beautiful"))))
  with(liferef2 in $(Reference, new(String, $(String, "Life is Grand")))) {
    
    PT_ASSERT(eq(at(liferef0,0), $(String, "Life is Long")));
    PT_ASSERT(eq(at(liferef1,0), $(String, "Life is Beautiful")));
    PT_ASSERT(eq(at(liferef2,0), $(String, "Life is Grand")));
  
  }

}

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

PT_SUITE(suite_memory) {
  
  PT_REG(test_new);
  PT_REG(test_reference);
  PT_REG(test_reference_with);
  PT_REG(test_reference_with_many);
  PT_REG(test_pool);

}
