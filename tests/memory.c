
#include "ptest.h"
#include "C+.h"

PT_SUITE(suite_memory) {
  
  PT_TEST(test_new) {
    
    var x = new(Int, 0);
    delete(x);
    
  }
  
  PT_TEST(test_reference) {
    
    var x = new(Int, 1);
    var y = new(Int, 2);
    
    var rx = $(Reference, x);
    
    PT_ASSERT(rx != x);
    PT_ASSERT(x != y);
    PT_ASSERT(neq(x, y));
    PT_ASSERT(x is at(rx,0));
    PT_ASSERT(eq(x, at(rx,0)));
    
    set(rx, 0, y);

    PT_ASSERT(y is at(rx,0));
    PT_ASSERT(eq(y, at(rx,0)));

    delete(x);
    delete(y);
    
  }
  
  PT_TEST(test_reference_with) {
    
    with($(Reference, new(String, "Almost like an Auto Ptr")), r) {
      
      PT_ASSERT(eq(at(r,0), $(String, "Almost like an Auto Ptr")));
      PT_ASSERT(neq(at(r,0), $(String, "Blah")));
      
    }
    
  }
  
  PT_TEST(test_pool) {
    
    var p = new(Pool);
    
    var x = retain(p, new(String, "Hello Everyone!"));
    var y = retain(p, new(String, "Bonjour"));
    var z = retain(p, new(Int, 3));
    
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
  
}