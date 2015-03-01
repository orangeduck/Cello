#include "Cello.h"

static void other_other_other_func(void) {
  printf("Stack Top!\n");
  
  var blah = new_gc(Array, Ref,
    new_gc(String, $S("Hello0")),
    new_gc(String, $S("Hello1")),
    new_gc(String, $S("Hello2")),
    new_gc(String, $S("Hello3")));
  
  printf("Blah: %p\n", blah);
  Cello_GC_Mark();
  Cello_GC_Sweep();
  
  println("%$", blah);
  
}

static var other_other_func(var i) {
  
  var x = $S("Hello from other other func");
  var y = new(Array, String,
    $S("Test"), $S("Tes1"), $S("Test2"));
  var z = new_gc(Array, String, $S("Blah"));
  
  other_other_other_func();
  
  del(y);
  
  return z;
  
}

static void other_func(void) {
  
  var i0 = $I(0);
  var i1 = $I(1);
  var i2 = $I(2);
  var s = $S("Hello, This is a String");
  
  int x = 4 + 4;
  int y = x + 10;
  
  println("%$", other_other_func(i2));
}

int main( int argc, char** argv) {
  
  other_func();
  
  printf("Stack Bottom!\n");
  Cello_GC_Mark();
  Cello_GC_Sweep();
  
  return 0;
  
}

