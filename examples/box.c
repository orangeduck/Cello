
#include "Cello.h"

/*
local var g_pool = NULL;

local void table_fill(var x) {
  put(x, $(String, "First"),  $(Real, 0.0));
  put(x, $(String, "Second"), $(Real, 0.1));
  put(x, $(String, "Third"),  $(Real, 5.7));
  release(g_pool, x);
}

local void table_process(var x) {
  put(x, $(String, "First"), $(Real, -0.65));
  release(g_pool, x);
}
*/

static void object_lifetime_example(void) {
  
  with(lifebox in $(Box, new_$(String, "Life is long"))) {
  
    print("This reference is: %$\n", lifebox);
    print("This string is alive: '%s'\n", lifebox);
  
  }

  print("Now it has been cleared up!\n");
  
}

static void many_object_lifetimes(void) {
  
  with(lifebox0 in $(Box, new_$(String, "Life is long")))
  with(lifebox1 in $(Box, new_$(String, "Life is Beautiful")))
  with(lifebox2 in $(Box, new_$(String, "Life is Grand"))) {
  
    print("%s :: %s :: %s\n", 
      lifebox0, 
      lifebox1, 
      lifebox2);
  
  }

}

int main(int argc, char** argv) {
  
  //g_pool = new(Pool);
  
  //var x = retain(g_pool, new(Table, String, Real));
  
  //table_fill(retain(g_pool, x));
  //table_process(retain(g_pool, x));
  
  //release(g_pool, x);
  
  //delete(g_pool);
 
  object_lifetime_example();
  many_object_lifetimes();
 
}

