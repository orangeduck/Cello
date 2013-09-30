#include "ptest.h"
#include "Cello.h"

local var DivideByZeroError = Singleton(DivideByZeroError);
local var OtherError = Singleton(OtherError);

local int exception_divide(int x, int y) {
  if (y == 0) {
    throw(DivideByZeroError, "%i / %i", $(Int, x), $(Int, y));
    return 0;
  } else {
    return x / y;
  }
}

PT_FUNC(test_throw) {
  
  int r0 = exception_divide(2, 1);
  int r1 = exception_divide(4, 2);
  int r2 = exception_divide(9, 3);
  
  PT_ASSERT(r0 == 2);
  PT_ASSERT(r1 == 2);
  PT_ASSERT(r2 == 3);
  
  PT_ASSERT(Exception_Depth() is 0);
  
}

PT_FUNC(test_catch) {
  
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
  
  PT_ASSERT(Exception_Depth() is 0);
  
}

PT_FUNC(test_catch_all) {
  
  volatile bool reached0 = false;
  volatile bool reached1 = false;

  try {
    exception_divide(2, 0);
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
  PT_ASSERT(Exception_Depth() is 0);
  
}

PT_FUNC(test_catch_outer) {

  volatile bool reached0 = false;
  volatile bool reached1 = false;
  
  try {

  PT_ASSERT(Exception_Depth() is 1);

    try {
      PT_ASSERT(Exception_Depth() is 2);
      exception_divide(2, 0);
    } catch (e in TypeError) {
      reached0 = true;
    }    

    PT_ASSERT(Exception_Depth() is 1);
  
  } catch (e) {
    reached1  = true;
  }
  
  PT_ASSERT(not reached0);
  PT_ASSERT(reached1);
  
  PT_ASSERT(Exception_Depth() is 0);
  
}

PT_SUITE(suite_exception) {
  
  PT_REG(test_throw);
  PT_REG(test_catch);
  PT_REG(test_catch_all);
  PT_REG(test_catch_outer);

}
