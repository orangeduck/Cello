
#include "ptest.h"
#include "Cello.h"

local var DivideByZeroError = Singleton(DivideByZeroError);
loval var OtherError = Singleton(OtherError);

local int exception_divide(int x, int y) {
  if (y == 0) {
    throw(DivideByZeroError, "Division By Zero: %i / %i", x, y);
    return 0;
  } else {
    return x / y;
  }
}

local void exception_result(int x, int y) {
  var result = $(Int, x + y);
  throw(result, "Unexpected Result");
}

PT_SUITE(suite_exception) {
  
  PT_TEST(test_throw) {
    
    int r0 = exception_divide(2, 1);
    int r1 = exception_divide(4, 2);
    int r2 = exception_divide(9, 3);
    
    PT_ASSERT(r0 == 2);
    PT_ASSERT(r1 == 2);
    PT_ASSERT(r2 == 3);
    
  }
  
  PT_TEST(test_catch) {
    
    volatile bool reached0 = false;
    volatile bool reached1 = false;
    volatile bool reached2 = false;
    
    try {
      int r3 = exception_divide(2, 0);
    } catch (e in DivideByZeroError) {
      reached0 = true;
    }
    
    try {
      int r3 = exception_divide(2, 1);
    } catch (e in DivideByZeroError) {
      reached1 = true;
    }
    
    try {
      int r3 = exception_divide(2, 1);
    } catch (e in None) {
      reached2 = true;
    }
    
    PT_ASSERT(reached0);
    PT_ASSERT(not reached1);
    PT_ASSERT(not reached2);
    
  }
  
  PT_TEST(test_throw_int) {
    
    volatile bool reached0 = false;
    volatile bool reached1 = false;
    
    try {
      exception_result(2, 2);
    } catch (e in $(Int, 1), $(Int, 2), $(Int, 3), $(Int, 4)) {
      PT_ASSERT(eq(e, $(Int, 4)));
      PT_ASSERT(neq(e, $(Int, 1)));
      reached0 = true;
    }
    
  }
  
  PT_TEST(test_catch_all) {
    
    volatile bool reached0 = false;
    volatile bool reached1 = false;

    try {
      exception_result(2, 0);
    } catch (e) {
      reached0 = true;
    }
    
    try {
      throw(OtherError, "Something else went wrong");
    } catch (e) {
      reached1 = true;
    }
    
    PT_ASSERT(reached0);
    PT_ASSERT(reached1);
    
  }
  
  PT_TEST(test_catch_outer) {
  
    volatile bool reached0 = false;
    volatile bool reached1 = false;
    
    try {
      try {
        exception_result(2, 0);
      } catch (e in TypeError) {
        reached0 = true;
      }
    } catch (e) {
      reached1  = true;
    }
    
    PT_ASSERT(not reached0);
    PT_ASSERT(reached1);
  
  }

}
