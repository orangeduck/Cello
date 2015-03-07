#include "Cello.h"

static void other_other_other_func(void) {
  
  var blah = new(Array, Ref,
    new(String, $S("Hello0")),
    new(String, $S("Hello1")),
    new(String, $S("Hello2")),
    new(String, $S("Hello3")));
  
  println("%$", blah);
  
}

static var other_other_func(var i) {
  
  var x = $S("Hello from other other func");
  var y = new(Array, String,
    $S("Test"), $S("Tes1"), $S("Test2"));
  var z = new(Array, String, $S("Blah"));
  
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

static void stack_test_func1(void) {
  
  var x = new(Array, Int, $I(10), $I(20), $I(30));
  
  var y = new(Int, $I(100));
  var z = new(Int, $I(200));
  
  set(x, $I(0), y);
  
  print("%$", x);
  
}

static void stack_test_func0(void) {
  
  var test0 = new(String, $S("Blah"));
  var tset1 = new(String, $S("Hahahaha"));
  
  stack_test_func1();
  
}

static void stack_test_func3(void) {
  
  var blasdasda = new(String, $S("Bogaslsgf"));
  var asdasd = new(String, $S("asddsad"));
  
  assign(blasdasda, asdasd);
  
}

static void stack_test_func2(void) {
    
  char blah[1024];
  
  show($S("Hello"));
  
  stack_test_func3();
  
}



int main( int argc, char** argv) {
  
  other_func();
  
  stack_test_func0();
  stack_test_func2();
  
  return 0;
  
}

