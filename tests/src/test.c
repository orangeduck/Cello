#include <Cello/Cello.h>
#include <PTest/PTest.h>

/* Array */

PT_FUNC(test_array_new) {
  
  var a0 = new(Array, Int, $I(1), $I(5), $I(10));
  var a1 = new(Array, Float, $F(1.1), $F(2.2));
  var a2 = copy(a0);
  
  PT_ASSERT(a0);
  PT_ASSERT(a1);
  PT_ASSERT(a2);
  
  PT_ASSERT(a0 isnt a1);
  PT_ASSERT(a0 isnt a2);
  PT_ASSERT(a1 isnt a2);

  PT_ASSERT( eq(get(a0, $I(0)), $I(1)) );
  PT_ASSERT( eq(get(a1, $I(0)), $F(1.1)) );
  PT_ASSERT( eq(get(a2, $I(0)), $I(1)) );
 
  del(a0);
  del(a1);
  del(a2);
  
}

PT_FUNC(test_array_assign) {
  
  var a0 = new(Array, Int, $I(1), $I(2), $I(3));
  var l0 = new(List, Int, $I(4), $I(5), $I(6), $I(7));
  
  PT_ASSERT(eq(get(a0, $I(0)), $I(1)));
  PT_ASSERT(eq(get(a0, $I(1)), $I(2)));
  PT_ASSERT(eq(get(a0, $I(2)), $I(3)));
  
  assign(a0, l0);
  
  PT_ASSERT(eq(get(a0, $I(0)), $I(4)));
  PT_ASSERT(eq(get(a0, $I(1)), $I(5)));
  PT_ASSERT(eq(get(a0, $I(2)), $I(6)));
  PT_ASSERT(eq(get(a0, $I(3)), $I(7)));
  
  assign(a0, range($I(5)));
  
  PT_ASSERT(eq(get(a0, $I(0)), $I(0)));
  PT_ASSERT(eq(get(a0, $I(1)), $I(1)));
  PT_ASSERT(eq(get(a0, $I(2)), $I(2)));
  PT_ASSERT(eq(get(a0, $I(3)), $I(3)));
  PT_ASSERT(eq(get(a0, $I(4)), $I(4)));
  
  del(a0);
  del(l0);
  
}

PT_FUNC(test_array_concat) {
  
  var a0 = new(Array, Int, $I(1), $I(2), $I(3));
  var a1 = new(Array, Int, $I(4), $I(5), $I(6));
  
  concat(a0, a1);
  
  PT_ASSERT(eq(get(a0, $I(0)), $I(1)));
  PT_ASSERT(eq(get(a0, $I(1)), $I(2)));
  PT_ASSERT(eq(get(a0, $I(2)), $I(3)));
  PT_ASSERT(eq(get(a0, $I(3)), $I(4)));
  PT_ASSERT(eq(get(a0, $I(4)), $I(5)));
  PT_ASSERT(eq(get(a0, $I(5)), $I(6)));
  
  append(a0, $I(10));
  
  PT_ASSERT(eq(get(a0, $I(6)), $I(10)));
  
  del(a0); del(a1);
  
}

PT_FUNC(test_array_cmp) {
  
  var a0 = new(Array, Int, $I(1), $I(5), $I(10));
  var a1 = new(Array, Int, $I(1), $I(5), $I(10));
  var a2 = new(Array, Int, $I(2), $I(5), $I(10));
  
  PT_ASSERT(eq(a0, a1));
  PT_ASSERT(neq(a0, a2));
  PT_ASSERT(gt(a2, a1));
  PT_ASSERT(lt(a1, a2));
  
  del(a0); del(a1); del(a2);
  
}

PT_FUNC(test_array_get) {
  
  var a0 = new(Array, Int, $I(1), $I(5), $I(10));
  
  PT_ASSERT(len(a0) is 3);
  PT_ASSERT(mem(a0, $I(1)));
  PT_ASSERT(mem(a0, $I(5)));
  
  rem(a0, $I(5));
  
  PT_ASSERT(len(a0) is 2);
  PT_ASSERT(mem(a0, $I(1)));
  PT_ASSERT(not mem(a0, $I(5)));
  
  resize(a0, 0);
  
  PT_ASSERT(len(a0) is 0);
  PT_ASSERT(not mem(a0, $I(1)));
  
  del(a0);

  var a1 = new(Array, String, 
    $S("Hello"), $S("There"), $S("People"));
  
  PT_ASSERT( eq(get(a1, $I(0)), $S("Hello")) );
  PT_ASSERT( eq(get(a1, $I(1)), $S("There")) );
  PT_ASSERT( eq(get(a1, $I(2)), $S("People")) );
  
  PT_ASSERT( eq(get(a1, $I(-1)), $S("People")) );
  PT_ASSERT( eq(get(a1, $I(-2)), $S("There")) );
  PT_ASSERT( eq(get(a1, $I(-3)), $S("Hello")) );
  
  set(a1, $I(1), $S("Blah"));
  
  PT_ASSERT( eq(get(a1, $I(0)), $S("Hello")) );
  PT_ASSERT( eq(get(a1, $I(1)), $S("Blah")) );
  PT_ASSERT( eq(get(a1, $I(2)), $S("People")) );
  
  set(a1, $I(0), $S("Foo"));
  set(a1, $I(2), $S("Bar"));
  
  PT_ASSERT( eq(get(a1, $I(0)), $S("Foo")) );
  PT_ASSERT( eq(get(a1, $I(1)), $S("Blah")) );
  PT_ASSERT( eq(get(a1, $I(2)), $S("Bar")) );
  
  del(a1);
  
}

PT_FUNC(test_array_hash) {
  
  var a0 = new(Array, String, $S("Hello"), $S("World"));
  
  PT_ASSERT(hash(a0) is (hash($S("Hello")) ^ hash($S("World"))));
  
  del(a0);
  
}

PT_FUNC(test_array_iter) {
  
  var a0 = new(Array, Int);
  var a1 = new(Array, Float);
  
  PT_ASSERT(iter_type(a0) is Int);
  PT_ASSERT(iter_type(a1) is Float);
  
  del(a0); del(a1);
  
  a0 = new(Array, String, 
    $S("Hello"), $S("There"), $S("People"));
  
  int counter = 0;
  
  foreach(item in a0) {
    
    switch(counter) {
      case 0: PT_ASSERT( eq(item, $S("Hello")) ); break;
      case 1: PT_ASSERT( eq(item, $S("There")) ); break;
      case 2: PT_ASSERT( eq(item, $S("People")) ); break;
    }
    
    counter++;
  }
  
  PT_ASSERT(counter is 3);
  
  counter = 0;
  
  foreach(item0 in a0) {
    foreach(item1 in a0) {
      counter++;
    }
  }
  
  PT_ASSERT(counter is 9);
  del(a0);
  
}

PT_FUNC(test_array_len) {
  
  var a0 = new(Array, String, $S("Hello"), $S("World"));
  
  PT_ASSERT(len(a0) is 2);
  
  append(a0, $S("!"));
  
  PT_ASSERT(len(a0) is 3);
  
  resize(a0, 0);
  
  PT_ASSERT(len(a0) is 0);
  
  del(a0);
  
}

PT_FUNC(test_array_push) {
  
  var a0 = new(Array, Int);
  
  PT_ASSERT(len(a0) is 0);
  
  push(a0, $I(1));
  
  PT_ASSERT(len(a0) is 1);
  PT_ASSERT( eq(get(a0, $I(0)), $I(1)) );
  
  push(a0, $I(3));
  
  PT_ASSERT(len(a0) is 2);
  PT_ASSERT( eq(get(a0, $I(0)), $I(1)) );
  PT_ASSERT( eq(get(a0, $I(1)), $I(3)) );
  
  push_at(a0, $I(10), $I(0));
  
  PT_ASSERT(len(a0) is 3);
  PT_ASSERT( eq(get(a0, $I(0)), $I(10)) );
  PT_ASSERT( eq(get(a0, $I(1)), $I(1)) );
  PT_ASSERT( eq(get(a0, $I(2)), $I(3)) );
  
  push_at(a0, $I(20), $I(1));
  
  PT_ASSERT(len(a0) is 4);
  PT_ASSERT( eq(get(a0, $I(0)), $I(10)) );
  PT_ASSERT( eq(get(a0, $I(1)), $I(20)) );
  PT_ASSERT( eq(get(a0, $I(2)), $I(1)) );
  PT_ASSERT( eq(get(a0, $I(3)), $I(3)) );
  
  pop_at(a0, $I(2));
  
  PT_ASSERT(len(a0) is 3);
  PT_ASSERT( eq(get(a0, $I(0)), $I(10)) );
  PT_ASSERT( eq(get(a0, $I(1)), $I(20)) );
  PT_ASSERT( eq(get(a0, $I(2)), $I(3)) );
  
  pop_at(a0, $I(0));
  
  PT_ASSERT(len(a0) is 2);
  PT_ASSERT( eq(get(a0, $I(0)), $I(20)) );
  PT_ASSERT( eq(get(a0, $I(1)), $I(3)) );
  
  pop(a0);
  
  PT_ASSERT(len(a0) is 1);
  PT_ASSERT( eq(get(a0, $I(0)), $I(20)) );
  
  pop(a0);
  
  PT_ASSERT(len(a0) is 0);
  
  del(a0);
  
}

PT_FUNC(test_array_resize) {
  
  var a0 = new(Array, Int);
  resize(a0, 1000);
  del(a0);
  
  a0 = new(Array, Int, $I(1), $I(2), $I(3));
  
  PT_ASSERT(len(a0) is 3);
  
  resize(a0, 0);
  
  PT_ASSERT(len(a0) is 0);
  PT_ASSERT(empty(a0));
  
  del(a0);
  
}

PT_FUNC(test_array_show) {
  
  var a0 = new(Array, Int, $I(1), $I(5), $I(9));
  var s0 = new(String);
  
  show_to(a0, s0, 0);
  
  del(a0); del(s0);
}

PT_FUNC(test_array_sort) {
  
  var a0 = new(Array, Int, $I(100), $I(1233), $I(1), $I(2312), $I(21));
  var a1 = new(Array, Int, $I(1), $I(21), $I(100), $I(1233), $I(2312));
  var a2 = new(Array, Int, $I(2312), $I(1233), $I(100), $I(21), $I(1));
  
  var a3 = copy(a0);
  var a4 = copy(a0);
  
  sort(a3);
  sort_by(a4, gt);
  PT_ASSERT(eq(a3, a1));
  PT_ASSERT(eq(a4, a2));
  
  del(a0); del(a1); del(a2); del(a3); del(a4);
  
  var a5 = new(Array, String, $S("a"), $S("b"), $S("c"), $S("d"), $S("e"));
  var a6 = new(Array, String, $S("c"), $S("b"), $S("e"), $S("a"), $S("d"));
  
  PT_ASSERT(neq(a5, a6));
  sort(a6);
  PT_ASSERT(eq(a5, a6));
  
  del(a5); del(a6);
  
}

PT_SUITE(suite_array) {
  PT_REG(test_array_new);
  PT_REG(test_array_assign);
  PT_REG(test_array_concat);
  PT_REG(test_array_cmp);
  PT_REG(test_array_get);
  PT_REG(test_array_hash);
  PT_REG(test_array_iter);
  PT_REG(test_array_len);
  PT_REG(test_array_push);
  PT_REG(test_array_resize);
  PT_REG(test_array_show);
  PT_REG(test_array_sort);
}

/* Box */

PT_FUNC(test_box_new) {
  
  var s0 = new(String, $S("Hello World!"));
  
  var b1 = new(Box, s0);
  
  PT_ASSERT(b1);
  PT_ASSERT(deref(b1) is s0);
  
  del(b1);
  
}

PT_FUNC(test_box_assign) {
  
  var s0 = new(String, $S("Hello World!"));
  var b0 = assign($(Box, NULL), s0);
  
  PT_ASSERT(b0);
  PT_ASSERT(deref(b0) is s0);
  
  destruct(b0);
  
}

PT_FUNC(test_box_pointer) {
  
  var s0 = new(String, $S("Hello World 1!"));
  var s1 = new(String, $S("Hello World 2!"));

  var b0 = $(Box, s0);
  var b1 = $(Box, s1);
  
  PT_ASSERT(deref(b0) is s0);
  PT_ASSERT(deref(b1) is s1);
  
  del(s0); del(s1);
  
}

PT_FUNC(test_box_show) {
  
  var b0 = new(Box, new(Int, $I(1)));
  var s0 = new(String);
  
  show_to(b0, s0, 0);
  
  del(b0); del(s0);
  
}

PT_SUITE(suite_box) {
  PT_REG(test_box_new);
  PT_REG(test_box_assign);
  PT_REG(test_box_pointer);
  PT_REG(test_box_show);
}

/* File */

PT_FUNC(test_file_format) {
  
  var f0 = $(File, NULL);
  
  sopen(f0, $S("test.txt"), $S("w"));
  format_to(f0, 0, "%s", "Hello World!");
  sclose(f0);  
  
}

PT_FUNC(test_file_new) {
  
  var f0 = new(File, $S("test.bin"), $S("w")); 
  var f1 = $(File, NULL);
  
  PT_ASSERT(f0);
  PT_ASSERT(f1);
  
  PT_ASSERT(f0 isnt f1);
  
  del(f0);
  
}

PT_FUNC(test_file_start) {
  
  struct File* file = new(File, $S("test.bin"), $S("w"));
  with (f0 in file) {
    PT_ASSERT(f0);
  }; 
     
  PT_ASSERT(file->file is NULL);
  
  del(file);
  
}

PT_FUNC(test_file_stream) {
  
  static char testoutput1[] = "This is a test\n";
  static char testoutput2[] = "This is a sample\n";
  static char testinput[512];
  
  var f0 = $(File, NULL);
  
  sopen(f0, $S("test.txt"), $S("w"));
  swrite(f0, testoutput1, sizeof(testoutput1));
  sclose(f0);  
  
  sopen(f0, $S("test.txt"), $S("r"));
  sread(f0, testinput, sizeof(testoutput1));
  sclose(f0);
  
  PT_ASSERT_STR_EQ(testinput, testoutput1);
  
}

PT_SUITE(suite_file) {
  PT_REG(test_file_format);
  PT_REG(test_file_new);
  PT_REG(test_file_start);
  PT_REG(test_file_stream);
}

/* Float */

PT_FUNC(test_float_assign) {
  
  var float0 = $F(1.0);
  var float1 = new(Float, $F(24.313));
  var float2 = copy(float0);
  
  PT_ASSERT(float0);
  PT_ASSERT(float1);
  PT_ASSERT(float2);
  
  PT_ASSERT(c_float(float0) is 1.0);
  PT_ASSERT(c_float(float1) is 24.313);
  PT_ASSERT(c_float(float2) is 1.0);
  
  assign(float2, float1);
  
  PT_ASSERT( not(float2 is float1) );
  PT_ASSERT(c_float(float2) is 24.313);
  
  del(float1);
  del(float2);
  
}

PT_FUNC(test_float_c_float) {
  
  PT_ASSERT(c_float($F(      10.0)) is       10.0);
  PT_ASSERT(c_float($F(  531.1231)) is   531.1231);
  PT_ASSERT(c_float($F(21312.4535)) is 21312.4535);
  PT_ASSERT(c_float($F(   1352.21)) is    1352.21);
  PT_ASSERT(c_float($F(   676.342)) is    676.342);
  
}

PT_FUNC(test_float_cmp) {
  
  PT_ASSERT( gt($F(5.0),    $F(0.0)) );
  PT_ASSERT( gt($F(51.33), $F(2.32)) );
  PT_ASSERT( lt($F(23.32), $F(99.92)) );
  PT_ASSERT( lt($F(31.0),   $F(32.3)) );
  PT_ASSERT( eq($F(1.11),  $F(1.11)) );
  PT_ASSERT( eq($F(23.55),   $F(23.55)) );
  PT_ASSERT( ge($F(2.89),  $F(2.89)) );
  PT_ASSERT( ge($F(87.34), $F(2.89)) );
  PT_ASSERT( le($F(16.6),  $F(16.6)) );
  PT_ASSERT( le($F(1.1),   $F(88.8)) );
  PT_ASSERT( neq($F(3.24), $F(6.85)) );
  PT_ASSERT( neq($F(3.4),  $F(5.4)) );
  
}

union type_interp {
  double c_float;
  int64_t c_int;
};

PT_FUNC(test_float_hash) {
  
  union type_interp r0 = { 34.0 };
  union type_interp r1 = { 11.0 };
  union type_interp r2 = { 0.6 };
  union type_interp r3 = { 82.13 };
  
  PT_ASSERT( hash($F(34.0)) is r0.c_int );
  PT_ASSERT( hash($F(11.0)) is r1.c_int );
  PT_ASSERT( hash($F(0.6)) is r2.c_int );
  PT_ASSERT( hash($F(82.13)) is r3.c_int );
  
}

PT_FUNC(test_float_show) {
  
  var s0 = new(String);
  
  show_to($F(213.12), s0, 0);
  PT_ASSERT_STR_EQ(c_str(s0),  "213.120000");
  
  show_to($F(3532.11), s0, 0);
  PT_ASSERT_STR_EQ(c_str(s0), "3532.110000");
  
  show_to($F(1023.1), s0, 0);
  PT_ASSERT_STR_EQ(c_str(s0), "1023.100000");
  
  del(s0);
  
}

PT_SUITE(suite_float) {
  PT_REG(test_float_assign);
  PT_REG(test_float_c_float);
  PT_REG(test_float_cmp);
  PT_REG(test_float_hash);
  PT_REG(test_float_show);
}

/* Filter */

var greater_than_two(var x) {
  return c_int(x) > 2 ? x : NULL;
}

var mem_hello(var x) {
  return mem(x, $S("Hello")) ? x : NULL;
}

PT_FUNC(test_filter_get) {
  
  var x = new(Array, Int, $I(0), $I(5), $I(2), $I(9));
  var f = filter(x, $(Function, greater_than_two));
  
  PT_ASSERT(mem(f, $I(5)));
  PT_ASSERT(mem(f, $I(9)));
  PT_ASSERT(not mem(f, $I(0)));
  PT_ASSERT(not mem(f, $I(1)));
  
  del(x);
  
}

PT_FUNC(test_filter_iter) {
  
  var x = new(Tuple, $S("Hello World"), $S("Hello Dan"), $S("Bonjour"));

  var y = new(Tuple);
  foreach (item in filter(x, $(Function, mem_hello))) {
    push(y, item);
  }
  
  PT_ASSERT(eq(get(y, $I(0)), $S("Hello World")));
  PT_ASSERT(eq(get(y, $I(1)), $S("Hello Dan")));
  
  del(x); del(y);
  
}

PT_FUNC(test_filter_new) {
  
  var x = new(Array, Int, $I(0), $I(5), $I(2), $I(9));
  var f = new(Filter, x, $(Function, greater_than_two));
  
  PT_ASSERT(mem(f, $I(5)));
  PT_ASSERT(mem(f, $I(9)));
  PT_ASSERT(not mem(f, $I(0)));
  PT_ASSERT(not mem(f, $I(1)));
  
  del(x);
  del(f);
  
}

PT_SUITE(suite_filter) {
  PT_REG(test_filter_get);
  PT_REG(test_filter_iter);
  PT_REG(test_filter_new);
}

/* Function */

var return_fst(var args) {
  return get(args, $I(0));
}

var return_snd(var args) {
  return get(args, $I(1));
}


PT_FUNC(test_function_call) {
  
  PT_ASSERT(eq(call($(Function, return_fst), $I(100), $I(200)), $I(100)));
  PT_ASSERT(eq(call($(Function, return_snd), $I(100), $I(200)), $I(200)));
  
}

PT_SUITE(suite_function) {
  PT_REG(test_function_call);
}

/* Int */

PT_FUNC(test_int_assign) {
  
  var int0 = $I(1);
  var int1 = new(Int, $I(24313));
  var int2 = copy(int0);
  
  PT_ASSERT(int0);
  PT_ASSERT(int1);
  PT_ASSERT(int2);
  
  PT_ASSERT(c_int(int0) is 1);
  PT_ASSERT(c_int(int1) is 24313);
  PT_ASSERT(c_int(int2) is 1);
  
  assign(int2, int1);
  
  PT_ASSERT( not(int2 is int1) );
  PT_ASSERT(c_int(int2) is 24313);
  
  del(int1);
  del(int2);
  
}

PT_FUNC(test_int_c_int) {
  
  PT_ASSERT( c_int($I(34)) is 34 );
  PT_ASSERT( c_int($I(11)) is 11 );
  PT_ASSERT( c_int($I(06)) is 06 );
  PT_ASSERT( c_int($I(8213)) is 8213 );
  
}

PT_FUNC(test_int_cmp) {
  
  PT_ASSERT( gt($I(5),    $I(0)) );
  PT_ASSERT( gt($I(5133), $I(232)) );
  PT_ASSERT( lt($I(2332), $I(9992)) );
  PT_ASSERT( lt($I(34),   $I(323)) );
  PT_ASSERT( eq($I(111),  $I(111)) );
  PT_ASSERT( eq($I(23),   $I(23)) );
  PT_ASSERT( ge($I(289),  $I(289)) );
  PT_ASSERT( ge($I(8734), $I(289)) );
  PT_ASSERT( le($I(166),  $I(166)) );
  PT_ASSERT( le($I(11),   $I(888)) );
  PT_ASSERT( neq($I(324), $I(685)) );
  PT_ASSERT( neq($I(34),  $I(54)) );
  
}

PT_FUNC(test_int_hash) {
  
  PT_ASSERT( hash($I(34)) is 34 );
  PT_ASSERT( hash($I(11)) is 11 );
  PT_ASSERT( hash($I(06)) is 06 );
  PT_ASSERT( hash($I(8213)) is 8213 );
  
}

PT_FUNC(test_int_show) {
  
  var s0 = new(String);
  
  show_to($I(34), s0, 0);
  PT_ASSERT_STR_EQ(c_str(s0), "34");
  
  show_to($I(06), s0, 0);
  PT_ASSERT_STR_EQ(c_str(s0), "6");
  
  show_to($I(8213), s0, 0);
  PT_ASSERT_STR_EQ(c_str(s0), "8213");
  
  del(s0);
  
}

PT_SUITE(suite_int) {
  PT_REG(test_int_assign);
  PT_REG(test_int_c_int);
  PT_REG(test_int_cmp);
  PT_REG(test_int_hash);
  PT_REG(test_int_show);
}

/* List */

PT_FUNC(test_list_new) {
  
  var l0 = new(List, Int, $I(1), $I(5), $I(10));
  var l1 = new(List, Float, $F(1.1), $F(2.2));
  var l2 = copy(l0);
  
  PT_ASSERT(l0);
  PT_ASSERT(l1);
  PT_ASSERT(l2);
  
  PT_ASSERT(l0 isnt l1);
  PT_ASSERT(l0 isnt l2);
  PT_ASSERT(l1 isnt l2);

  PT_ASSERT( eq(get(l0, $I(0)), $I(1)) );
  PT_ASSERT( eq(get(l1, $I(0)), $F(1.1)) );
  PT_ASSERT( eq(get(l2, $I(0)), $I(1)) );
 
  del(l0);
  del(l1);
  del(l2);
  
}

PT_FUNC(test_list_assign) {
  
  var l0 = new(List, Int, $I(1), $I(2), $I(3));
  var a0 = new(Array, Int, $I(4), $I(5), $I(6), $I(7));
  
  PT_ASSERT(eq(get(l0, $I(0)), $I(1)));
  PT_ASSERT(eq(get(l0, $I(1)), $I(2)));
  PT_ASSERT(eq(get(l0, $I(2)), $I(3)));
  
  assign(l0, a0);
  
  PT_ASSERT(eq(get(l0, $I(0)), $I(4)));
  PT_ASSERT(eq(get(l0, $I(1)), $I(5)));
  PT_ASSERT(eq(get(l0, $I(2)), $I(6)));
  PT_ASSERT(eq(get(l0, $I(3)), $I(7)));
  
  assign(l0, range($I(5)));
  
  PT_ASSERT(eq(get(l0, $I(0)), $I(0)));
  PT_ASSERT(eq(get(l0, $I(1)), $I(1)));
  PT_ASSERT(eq(get(l0, $I(2)), $I(2)));
  PT_ASSERT(eq(get(l0, $I(3)), $I(3)));
  PT_ASSERT(eq(get(l0, $I(4)), $I(4)));
  
  del(l0);
  del(a0);
  
}

PT_FUNC(test_list_concat) {
  
  var l0 = new(List, Int, $I(1), $I(2), $I(3));
  var l1 = new(List, Int, $I(4), $I(5), $I(6));
  
  concat(l0, l1);
  
  PT_ASSERT(eq(get(l0, $I(0)), $I(1)));
  PT_ASSERT(eq(get(l0, $I(1)), $I(2)));
  PT_ASSERT(eq(get(l0, $I(2)), $I(3)));
  PT_ASSERT(eq(get(l0, $I(3)), $I(4)));
  PT_ASSERT(eq(get(l0, $I(4)), $I(5)));
  PT_ASSERT(eq(get(l0, $I(5)), $I(6)));
  
  append(l0, $I(10));
  
  PT_ASSERT(eq(get(l0, $I(6)), $I(10)));
  
  del(l0); del(l1);
  
}

PT_FUNC(test_list_cmp) {
    
  var a0 = new(Array, Int, $I(1), $I(5), $I(10));
  var a1 = new(Array, Int, $I(1), $I(5), $I(10));
  var a2 = new(Array, Int, $I(2), $I(5), $I(10));
  
  PT_ASSERT(eq(a0, a1));
  PT_ASSERT(neq(a0, a2));
  PT_ASSERT(gt(a2, a1));
  PT_ASSERT(lt(a1, a2));
  
  del(a0); del(a1); del(a2);
  
}

PT_FUNC(test_list_get) {
  
  var l0 = new(List, Int, $I(1), $I(5), $I(10));
  
  PT_ASSERT(len(l0) is 3);
  PT_ASSERT(mem(l0, $I(1)));
  PT_ASSERT(mem(l0, $I(5)));
  
  rem(l0, $I(5));
  
  PT_ASSERT(len(l0) is 2);
  PT_ASSERT(mem(l0, $I(1)));
  PT_ASSERT(not mem(l0, $I(5)));
  
  resize(l0, 0);
  
  PT_ASSERT(len(l0) is 0);
  PT_ASSERT(not mem(l0, $I(1)));
  
  del(l0);

  var l1 = new(List, String, 
    $S("Hello"), $S("There"), $S("People"));
  
  PT_ASSERT( eq(get(l1, $I(0)), $S("Hello")) );
  PT_ASSERT( eq(get(l1, $I(1)), $S("There")) );
  PT_ASSERT( eq(get(l1, $I(2)), $S("People")) );
  
  PT_ASSERT( eq(get(l1, $I(-1)), $S("People")) );
  PT_ASSERT( eq(get(l1, $I(-2)), $S("There")) );
  PT_ASSERT( eq(get(l1, $I(-3)), $S("Hello")) );
  
  set(l1, $I(1), $S("Blah"));
  
  PT_ASSERT( eq(get(l1, $I(0)), $S("Hello")) );
  PT_ASSERT( eq(get(l1, $I(1)), $S("Blah")) );
  PT_ASSERT( eq(get(l1, $I(2)), $S("People")) );
  
  set(l1, $I(0), $S("Foo"));
  set(l1, $I(2), $S("Bar"));
  
  PT_ASSERT( eq(get(l1, $I(0)), $S("Foo")) );
  PT_ASSERT( eq(get(l1, $I(1)), $S("Blah")) );
  PT_ASSERT( eq(get(l1, $I(2)), $S("Bar")) );
  
  del(l1);
  
}

PT_FUNC(test_list_hash) {
  
  var l0 = new(List, String, $S("Hello"), $S("World"));
  
  PT_ASSERT(hash(l0) is (hash($S("Hello")) ^ hash($S("World"))));
  
  del(l0);
  
}

PT_FUNC(test_list_iter) {
  
  var l0 = new(List, Int);
  var l1 = new(List, Float);
  
  PT_ASSERT(iter_type(l0) is Int);
  PT_ASSERT(iter_type(l1) is Float);
  
  del(l0); del(l1);
  
  l0 = new(List, String, 
    $S("Hello"), $S("There"), $S("People"));
  
  int counter = 0;
  
  foreach(item in l0) {
    
    switch(counter) {
      case 0: PT_ASSERT( eq(item, $S("Hello")) ); break;
      case 1: PT_ASSERT( eq(item, $S("There")) ); break;
      case 2: PT_ASSERT( eq(item, $S("People")) ); break;
    }
    
    counter++;
  }
  
  PT_ASSERT(counter is 3);
  
  counter = 0;
  
  foreach(item0 in l0) {
    foreach(item1 in l0) {
      counter++;
    }
  }
  
  PT_ASSERT(counter is 9);
  del(l0);
  
}

PT_FUNC(test_list_len) {
  
  var l0 = new(List, String, $S("Hello"), $S("World"));
  
  PT_ASSERT(len(l0) is 2);
  
  append(l0, $S("!"));
  
  PT_ASSERT(len(l0) is 3);
  
  resize(l0, 0);
  
  PT_ASSERT(len(l0) is 0);
  
  del(l0);
  
}

PT_FUNC(test_list_push) {
  
  var l0 = new(List, Int);
  
  PT_ASSERT(len(l0) is 0);
  
  push(l0, $I(1));
  
  PT_ASSERT(len(l0) is 1);
  PT_ASSERT( eq(get(l0, $I(0)), $I(1)) );
  
  push(l0, $I(3));
  
  PT_ASSERT(len(l0) is 2);
  PT_ASSERT( eq(get(l0, $I(0)), $I(1)) );
  PT_ASSERT( eq(get(l0, $I(1)), $I(3)) );
  
  push_at(l0, $I(10), $I(0));
  
  PT_ASSERT(len(l0) is 3);
  PT_ASSERT( eq(get(l0, $I(0)), $I(10)) );
  PT_ASSERT( eq(get(l0, $I(1)), $I(1)) );
  PT_ASSERT( eq(get(l0, $I(2)), $I(3)) );
  
  push_at(l0, $I(20), $I(1));
  
  PT_ASSERT(len(l0) is 4);
  PT_ASSERT( eq(get(l0, $I(0)), $I(10)) );
  PT_ASSERT( eq(get(l0, $I(1)), $I(20)) );
  PT_ASSERT( eq(get(l0, $I(2)), $I(1)) );
  PT_ASSERT( eq(get(l0, $I(3)), $I(3)) );
  
  pop_at(l0, $I(2));
  
  PT_ASSERT(len(l0) is 3);
  PT_ASSERT( eq(get(l0, $I(0)), $I(10)) );
  PT_ASSERT( eq(get(l0, $I(1)), $I(20)) );
  PT_ASSERT( eq(get(l0, $I(2)), $I(3)) );
  
  pop_at(l0, $I(0));
  
  PT_ASSERT(len(l0) is 2);
  PT_ASSERT( eq(get(l0, $I(0)), $I(20)) );
  PT_ASSERT( eq(get(l0, $I(1)), $I(3)) );
  
  pop(l0);
  
  PT_ASSERT(len(l0) is 1);
  PT_ASSERT( eq(get(l0, $I(0)), $I(20)) );
  
  pop(l0);
  
  PT_ASSERT(len(l0) is 0);
  
  del(l0);
  
}

PT_FUNC(test_list_resize) {
  
  var l0 = new(List, Int);
  resize(l0, 1000);
  del(l0);
  
  l0 = new(List, Int, $I(1), $I(2), $I(3));
  
  PT_ASSERT(len(l0) is 3);
  
  resize(l0, 0);
  
  PT_ASSERT(len(l0) is 0);
  PT_ASSERT(empty(l0));
  
  del(l0);
  
}

PT_FUNC(test_list_show) {

  var l0 = new(List, Int, $I(1), $I(5), $I(9));
  var s0 = new(String);
  
  show_to(l0, s0, 0);
  
  del(l0); del(s0);

}

PT_SUITE(suite_list) {
  PT_REG(test_list_new);
  PT_REG(test_list_assign);
  PT_REG(test_list_concat);
  PT_REG(test_list_cmp);
  PT_REG(test_list_get);
  PT_REG(test_list_hash);
  PT_REG(test_list_iter);
  PT_REG(test_list_len);
  PT_REG(test_list_push);
  PT_REG(test_list_resize);
  PT_REG(test_list_show);
}

/* Map */

var convert_to_int(var x) {
  var y = new(Int);
  look_from(y, x, 0);
  return y;
}

var assert_value(var x) {
  PT_ASSERT(x);
  return NULL;
}

PT_FUNC(test_map_call) {

  var x = tuple($I(0), $S("Hello!"), $F(2.4));
  call(map(x, $(Function, assert_value)));

}

PT_FUNC(test_map_get) {

  var x = tuple($S("1"), $S("2"), $S("3"));
  var m = map(x, $(Function, convert_to_int));
  
  var i0 = get(m, $I(0));
  var i1 = get(m, $I(1));
  var i2 = get(m, $I(2));
  
  PT_ASSERT(eq(i0, $I(1)));
  PT_ASSERT(eq(i1, $I(2)));
  PT_ASSERT(eq(i2, $I(3)));
  
  del(i0); del(i1); del(i2);
  
}

PT_FUNC(test_map_iter) {

  var x = tuple($I(0), $S("Hello!"), $F(2.4));
  foreach (_ in map(x, $(Function, assert_value)));
  
}

PT_FUNC(test_map_len) {

  var x = tuple($S("1"), $S("2"), $S("3"));
  var y = tuple($S("1"), $S("2"), $S("3"), $S("4"));
  var m = map(x, $(Function, convert_to_int));
  var n = map(y, $(Function, convert_to_int));
  
  PT_ASSERT(len(m) is len(x));
  PT_ASSERT(len(n) is len(y));
  
}

PT_FUNC(test_map_new) {

  var x = tuple($S("1"), $S("2"), $S("3"));
  var m = new(Map, x, $(Function, convert_to_int));
  
  var i0 = get(m, $I(0));
  var i1 = get(m, $I(1));
  var i2 = get(m, $I(2));
  
  PT_ASSERT(eq(i0, $I(1)));
  PT_ASSERT(eq(i1, $I(2)));
  PT_ASSERT(eq(i2, $I(3)));
  
  del(i0); del(i1); del(i2);
  del(m);
  
}

PT_SUITE(suite_map) {
  PT_REG(test_map_call);
  PT_REG(test_map_get);
  PT_REG(test_map_iter);
  PT_REG(test_map_len);
  PT_REG(test_map_new);
}

/* Mutex */

static var increment(var args) {
  var mut = get(args, $I(0));
  var tot = get(args, $I(1));
  lock(mut);
  assign(tot, $I(c_int(tot)+1));
  unlock(mut);
  return NULL;
}

static var increment2(var args) {
  var mut = get(args, $I(0));
  var tot = get(args, $I(1));
  with (m in mut) {
    assign(tot, $I(c_int(tot)+1));
  }
  return NULL;
}

PT_FUNC(test_mutex_lock) {
  
  var mutex = new(Mutex);
  var total = $I(0);
  
  var threads = new(Array, Box,
    new(Thread, $(Function, increment)), new(Thread, $(Function, increment)),
    new(Thread, $(Function, increment)), new(Thread, $(Function, increment)),
    new(Thread, $(Function, increment)));
  
  PT_ASSERT(eq(total, $I(0)));
  
  foreach (t in threads) {
    call(deref(t), mutex, total);
  }
  
  foreach (t in threads) {
    join(deref(t));
  }
  
  PT_ASSERT(eq(total, $I(5)));
  
  del(threads);
  del(mutex);
  
}

PT_FUNC(test_mutex_new) {
  
  var mutex = new(Mutex);
  
  start(mutex);
  
  del(mutex);
  
}

PT_FUNC(test_mutex_start) {
  
  var mutex = new(Mutex);
  var total = $I(0);
  
  var threads = new(Array, Box,
    new(Thread, $(Function, increment2)), new(Thread, $(Function, increment2)),
    new(Thread, $(Function, increment2)), new(Thread, $(Function, increment2)),
    new(Thread, $(Function, increment2)));
  
  PT_ASSERT(eq(total, $I(0)));
  
  foreach (t in threads) {
    call(deref(t), mutex, total);
  }
  
  foreach (t in threads) {
    join(deref(t));
  }
  
  PT_ASSERT(eq(total, $I(5)));
  
  del(threads);
  del(mutex);
  
}

PT_SUITE(suite_mutex) {
  PT_REG(test_mutex_lock);
  PT_REG(test_mutex_new);
  PT_REG(test_mutex_start);
}

/* Range */

PT_FUNC(test_range_assign) {
  var x = new(Range, $I(1), $I(5), $I(2));
  var y = assign(new(Range), x);
  
  PT_ASSERT(eq(x, y));
  PT_ASSERT(c_int(get(y, $I(0))) is 1);
  PT_ASSERT(c_int(get(y, $I(1))) is 3);
  PT_ASSERT(len(y) is 2);
  
  del(x); del(y);
}

PT_FUNC(test_range_cmp) {
  
  var x = range($I(100));
  var y = range($I(0), $I(100));
  var z = range($I(0), $I(100), $I(1));
  
  PT_ASSERT(eq(x, y));
  PT_ASSERT(eq(y, z));
  PT_ASSERT(eq(x, z));
  
  var a = new(Range, $I(100));
  
  PT_ASSERT(eq(x, a));
  PT_ASSERT(eq(y, a));
  PT_ASSERT(eq(z, a));
  
  del(a);
  
}

PT_FUNC(test_range_get) {
  
  var x = range($I(1000));
  var y = range($I(1), $I(20));
  var z = range($I(3), $I(21), $I(2));

  PT_ASSERT(c_int(get(x, $I(100))) is 100);
  PT_ASSERT(c_int(get(x, $I(10))) is 10);
  PT_ASSERT(c_int(get(x, $I(5))) is 5);
  
  PT_ASSERT(c_int(get(x, $I(-1))) is 999);
  PT_ASSERT(c_int(get(x, $I(-2))) is 998);
  PT_ASSERT(c_int(get(x, $I(-3))) is 997);
  
  PT_ASSERT(c_int(get(y, $I(15))) is 16);
  PT_ASSERT(c_int(get(y, $I(10))) is 11);
  PT_ASSERT(c_int(get(y, $I(5))) is 6);
  
  PT_ASSERT(c_int(get(z, $I(0))) is 3);
  PT_ASSERT(c_int(get(z, $I(1))) is 5);
  PT_ASSERT(c_int(get(z, $I(3))) is 9);
  
}

PT_FUNC(test_range_iter) {
  
  PT_ASSERT(iter_type(range($I(10))) is Int);
  
  size_t j;
  var x = range($I(1000));
  var y = range($I(1), $I(20));
  var z = range($I(3), $I(21), $I(2));
  
  j = 0;
  foreach (i in x) {
    PT_ASSERT(c_int(i) is j);
    j++;
  }
  
  j = 1;
  foreach (i in y) {
    PT_ASSERT(c_int(i) is j);
    j++;
  }
  
  j = 3;
  foreach (i in z) {
    PT_ASSERT(c_int(i) is j);
    j+=2;
  }
  
}

PT_FUNC(test_range_len) {
  
  var x = range($I(1000));
  var y = range($I(1), $I(20));
  var z = range($I(3), $I(21), $I(2));
  
  PT_ASSERT(len(x) is 1000);
  PT_ASSERT(len(y) is 19);
  PT_ASSERT(len(z) is 9);
  
}

PT_FUNC(test_range_new) {
  var x = new(Range, $I(1), $I(5), $I(2));
  del(x);
}

PT_FUNC(test_range_show) {
  
  var r0 = new(Range, $I(1), $I(5), $I(2));
  var s0 = new(String);
  
  show_to(r0, s0, 0);
  
  del(r0); del(s0);
}


PT_SUITE(suite_range) {
  PT_REG(test_range_assign);
  PT_REG(test_range_cmp);
  PT_REG(test_range_get);
  PT_REG(test_range_iter);
  PT_REG(test_range_len);
  PT_REG(test_range_new);
  PT_REG(test_range_show);
}

/* Ref */

PT_FUNC(test_ref_assign) {
  
  var s0 = new(String, $S("Hello World!"));
  var r0 = assign($(Ref, NULL), s0);
  
  PT_ASSERT(r0);
  PT_ASSERT(deref(r0) is s0);
  
  del(s0);
  
}

PT_FUNC(test_ref_pointer) {
  
  var s0 = new(String, $S("Hello World 1!"));
  var s1 = NULL;

  var r0 = $(Ref, s0);
  var r1 = $(Ref, s1);
  
  PT_ASSERT(deref(r0) is s0);
  PT_ASSERT(deref(r1) is NULL);
  
  del(s0);
  
}

PT_SUITE(suite_ref) {
  PT_REG(test_ref_assign);
  PT_REG(test_ref_pointer);
}

/* Slice */

PT_FUNC(test_slice_assign) {
  
  var x = tuple($I(100), $I(10), $I(30), $I(50), $I(6), $I(1));
  var s0 = new(Slice, x, _, _, _);
  var s1 = assign(new(Slice, x), s0);
  
  PT_ASSERT(eq(s0, s1));
  PT_ASSERT(c_int(get(s1, $I(0))) is 100);
  PT_ASSERT(c_int(get(s1, $I(1))) is 10);
  PT_ASSERT(c_int(get(s1, $I(2))) is 30);
  
  del(s0); del(s1);
  
}

PT_FUNC(test_slice_cmp) {
  
  
  var x = tuple($I(100), $I(10), $I(30), $I(50), $I(6), $I(1));
  var s0 = slice(x, $I(4));
  var s1 = slice(x, $I(0), $I(4));
  var s2 = slice(x, $I(0), $I(4), $I(1));
  var s3 = slice(x, _, $I(4));
  
  PT_ASSERT(eq(s0, s1));
  PT_ASSERT(eq(s0, s2));
  PT_ASSERT(eq(s0, s3));
  
}

PT_FUNC(test_slice_get) {
  
  var x = tuple($I(100), $I(10), $I(30), $I(50), $I(6), $I(1));
  var s0 = slice(x, $I(4));
  var s1 = slice(x, $I(1), $I(4));
  var s2 = slice(x, $I(0), $I(4), $I(2));
  var s3 = slice(x, _, _, $I(2));
  
  PT_ASSERT(c_int(get(s0, $I(0))) is 100);
  PT_ASSERT(c_int(get(s0, $I(1))) is 10);
  
  PT_ASSERT(c_int(get(s1, $I(0))) is 10);
  PT_ASSERT(c_int(get(s1, $I(1))) is 30);
  
  PT_ASSERT(c_int(get(s2, $I(0))) is 100);
  PT_ASSERT(c_int(get(s2, $I(1))) is 30);
  
  PT_ASSERT(c_int(get(s3, $I(0))) is 100);
  PT_ASSERT(c_int(get(s3, $I(1))) is 30);
  
  var s4 = slice(x, $I(-2));
  var s5 = slice(x, $I(-3), $I(-2));
  var s6 = slice(x, _, _, $I(-1));
  var s7 = slice(x, $I(-4), _, $I(1));
  
  PT_ASSERT(c_int(get(s4, $I(-1))) is 50);
  PT_ASSERT(c_int(get(s4, $I(-2))) is 30);
  
  PT_ASSERT(c_int(get(s5, $I(0))) is 50);

  PT_ASSERT(c_int(get(s6, $I(0))) is 1);
  PT_ASSERT(c_int(get(s6, $I(1))) is 6);
  PT_ASSERT(c_int(get(s6, $I(2))) is 50);
  PT_ASSERT(c_int(get(s6, $I(3))) is 30);
  PT_ASSERT(c_int(get(s6, $I(4))) is 10);
  PT_ASSERT(c_int(get(s6, $I(5))) is 100);

  PT_ASSERT(c_int(get(s7, $I(0))) is 30);
  PT_ASSERT(c_int(get(s7, $I(1))) is 50);
  PT_ASSERT(c_int(get(s7, $I(2))) is 6);
  
  PT_ASSERT(c_int(get(s0, $I(-1))) is 50);
  PT_ASSERT(c_int(get(s1, $I(-1))) is 50);
  PT_ASSERT(c_int(get(s6, $I(-1))) is 100);
  
}

PT_FUNC(test_slice_iter) {
  
  var x = tuple($I(100), $I(10), $I(30), $I(50), $I(6), $I(1));
  var s0 = slice(x, $I(4));
  var s1 = slice(x, $I(1), $I(4));
  var s2 = slice(x, $I(0), $I(4), $I(2));
  var s3 = slice(x, _, _, $I(2));
  
  /* TODO: Improve */
  foreach (i in s0) {}
  foreach (i in s1) {}
  foreach (i in s2) {}
  foreach (i in s3) {}
  
  x = new(Array, Int);
  var y = slice(x);
  
  PT_ASSERT(iter_type(y) is iter_type(x));
  
  del(x);
  
}

PT_FUNC(test_slice_len) {
  
  var x = tuple($I(100), $I(10), $I(30), $I(50), $I(6), $I(1));
  var s0 = slice(x, $I(4));
  var s1 = slice(x, $I(1), $I(4));
  var s2 = slice(x, $I(0), $I(4), $I(2));
  var s3 = slice(x, _, _, $I(2));
  
  PT_ASSERT(len(s0) is 4);
  PT_ASSERT(len(s1) is 3);
  PT_ASSERT(len(s2) is 2);
  PT_ASSERT(len(s3) is 3);
  
}

PT_FUNC(test_slice_new) {
  
  var x = tuple($I(100), $I(10), $I(30), $I(50), $I(6), $I(1));
  var s0 = new(Slice, x, _, _, _);
  var s1 = new(Slice, x, $I(1), _);
  
  del(s0);
  del(s1);
  
}

PT_SUITE(suite_slice) {
  PT_REG(test_slice_assign);
  PT_REG(test_slice_cmp);
  PT_REG(test_slice_get);
  PT_REG(test_slice_iter);
  PT_REG(test_slice_len);
  PT_REG(test_slice_new);
}

/* String */

PT_FUNC(test_string_assign) {
  
  var s0 = new(String, $S("Hello"));
  
  PT_ASSERT_STR_EQ(c_str(s0), "Hello");
  
  assign(s0, $S("There"));

  PT_ASSERT_STR_EQ(c_str(s0), "There");

  del(s0);
  
}

PT_FUNC(test_string_c_str) {
  
  PT_ASSERT_STR_EQ(c_str($S("Ball")),    "Ball");
  PT_ASSERT_STR_EQ(c_str($S("dog")),     "dog");
  PT_ASSERT_STR_EQ(c_str($S("great")),   "great");
  PT_ASSERT_STR_EQ(c_str($S("Yellow")),  "Yellow");
  PT_ASSERT_STR_EQ(c_str($S("Hello")),   "Hello");
  
}

PT_FUNC(test_string_cmp) {
  
  PT_ASSERT( gt($S("Ball"),    $S("Apple")) );
  PT_ASSERT( gt($S("dog"),     $S("cat")) );
  PT_ASSERT( lt($S("great"),   $S("hello")) );
  PT_ASSERT( lt($S("Yellow"),  $S("Zoo")) );
  PT_ASSERT( eq($S("Hello"),   $S("Hello")) );
  PT_ASSERT( eq($S("there"),   $S("there")) );
  PT_ASSERT( ge($S("Hello"),   $S("Hello")) );
  PT_ASSERT( ge($S("tooting"), $S("red")) );
  PT_ASSERT( le($S("guard"),   $S("guardian")) );
  PT_ASSERT( le($S("keep"),    $S("keep")) );
  PT_ASSERT(neq($S("Hello"),   $S("hello")) );
  PT_ASSERT(neq($S("group"),   $S("GROUP")) );
  
}

PT_FUNC(test_string_concat) {
  
  var s0 = new(String, $S("Hello"));
  PT_ASSERT_STR_EQ(c_str(s0), "Hello");
    
  concat(s0, $S(" "));
  PT_ASSERT_STR_EQ(c_str(s0), "Hello ");
  
  concat(s0, $S("World"));
  PT_ASSERT_STR_EQ(c_str(s0), "Hello World");

  append(s0, $S("!"));
  PT_ASSERT_STR_EQ(c_str(s0), "Hello World!");
  
  del(s0);
  
}

PT_FUNC(test_string_format) {
  
  var s0 = new(String);
  resize(s0, 1000);
  
  format_to(s0, 0, "%s", "Hello");  
  PT_ASSERT_STR_EQ(c_str(s0), "Hello");

  format_to(s0, 0, "");  
  PT_ASSERT_STR_EQ(c_str(s0), "");
  
  print_to(s0, 0, "");
  PT_ASSERT_STR_EQ(c_str(s0), "");
  
  del(s0);
  
}

PT_FUNC(test_string_get) {
  
  var s0 = new(String, $S("Balloons"));
  
  PT_ASSERT(len(s0) is 8);
  PT_ASSERT(mem(s0, $S("Ball")));
  PT_ASSERT(mem(s0, $S("oon")));
  PT_ASSERT(mem(s0, $S("Balloons")));
  
  rem(s0, $S("oons"));
  
  PT_ASSERT_STR_EQ(c_str(s0), "Ball");
  
  resize(s0, 0);
  
  PT_ASSERT(len(s0) is 0);
  PT_ASSERT_STR_EQ(c_str(s0), "");
  
  
  del(s0);
  
}

PT_FUNC(test_string_hash) {
  
  uint64_t v0 = 4771441285123272284ULL;
  uint64_t v1 = 17415363727859751682ULL;
  uint64_t v2 = 11867268813077774525ULL;
  
  PT_ASSERT(hash($S("Hello")) is v0);
  PT_ASSERT(hash($S("There")) is v1);
  PT_ASSERT(hash($S("People")) is v2);
  
}

PT_FUNC(test_string_len) {
  
  PT_ASSERT(len($S("tooting")) is 7);
  PT_ASSERT(len($S("guard")) is 5);
  PT_ASSERT(len($S("keep")) is 4);
  PT_ASSERT(len($S("Hello")) is 5);
  PT_ASSERT(len($S("group")) is 5);
  
}

PT_FUNC(test_string_new) {
  
  var s0 = $S("Hello");
  var s1 = new(String, $S("There"));
  var s2 = copy(s0);
  
  PT_ASSERT(s0);
  PT_ASSERT(s1);
  PT_ASSERT(s2);
  
  PT_ASSERT_STR_EQ(c_str(s0), "Hello");
  PT_ASSERT_STR_EQ(c_str(s1), "There");
  PT_ASSERT_STR_EQ(c_str(s2), "Hello");
  
  assign(s2, s1);
  
  PT_ASSERT( not(s1 is s2) );
  PT_ASSERT_STR_EQ( c_str(s2), "There" );
  
  del(s1);
  del(s2);
  
}

PT_FUNC(test_string_resize) {
  
  var s0 = new(String, $S("Hello"));
  resize(s0, 1000);
  del(s0);
  
  s0 = new(String, $S("Hello"));
  
  PT_ASSERT_STR_EQ(c_str(s0), "Hello");
  
  resize(s0, 0);

  PT_ASSERT_STR_EQ(c_str(s0), "");
  PT_ASSERT(len(s0) is 0);
  PT_ASSERT(empty(s0));

  del(s0);
  
}

PT_FUNC(test_string_show) {
  
  var s0 = new(String);
  
  show_to($S("Hello"), s0, 0);
  PT_ASSERT_STR_EQ(c_str(s0), "\"Hello\"");
  
  show_to($S("Hello\n"), s0, 0);
  PT_ASSERT_STR_EQ(c_str(s0), "\"Hello\\n\"");
  
  show_to($S("Hel\"lo\n"), s0, 0);
  PT_ASSERT_STR_EQ(c_str(s0), "\"Hel\\\"lo\\n\"");
  
  show_to($S("Hel\"'lo\n"), s0, 0);
  PT_ASSERT_STR_EQ(c_str(s0), "\"Hel\\\"\\'lo\\n\"");
  
  del(s0);
  
}

PT_SUITE(suite_string) {
  PT_REG(test_string_assign);
  PT_REG(test_string_c_str);
  PT_REG(test_string_cmp);
  PT_REG(test_string_concat);
  PT_REG(test_string_format);
  PT_REG(test_string_get);
  PT_REG(test_string_hash);
  PT_REG(test_string_len);
  PT_REG(test_string_new);
  PT_REG(test_string_resize);
  PT_REG(test_string_show);
}

/* Table */

PT_FUNC(test_table_assign) {
  
  var t0 = new(Table, String, Int);
  var t1 = new(Table, String, Int,
    $S("asfa"), $I(0), $S("gasg"), $I(2),
    $S("asda"), $I(1), $S("dqga"), $I(0),
    $S("jaja"), $I(7), $S("sdfa"), $I(2),
    $S("jads"), $I(7), $S("kyad"), $I(2),
    $S("kyas"), $I(7), $S("hwdw"), $I(2),
    $S("awaa"), $I(7), $S("lkil"), $I(2)); 
  
  PT_ASSERT(len(t0) is 0);
  
  assign(t0, t1);
  
  PT_ASSERT(len(t0) is 12);
  PT_ASSERT(eq(get(t0, $S("asfa")), $I(0)));
  PT_ASSERT(eq(get(t0, $S("kyad")), $I(2)));
  PT_ASSERT(eq(get(t0, $S("lkil")), $I(2)));
  PT_ASSERT(eq(get(t0, $S("jads")), $I(7)));
  
  del(t0); del(t1);
  
}

PT_FUNC(test_table_cmp) {
  
  var t0 = new(Table, String, Int,
    $S("Hello"), $I(2),
    $S("There"), $I(5));
  
  var t1 = new(Table, String, Int,
    $S("Hello"), $I(2),
    $S("There"), $I(5));
  
  var t2 = new(Table, String, Int,
    $S("Bello"), $I(2),
    $S("There"), $I(5));
  
  var t3 = new(Table, String, Int,
    $S("Hello"), $I(2),
    $S("There"), $I(5),
    $S("World"), $I(6));
  
  PT_ASSERT(eq(t0, t1));
  PT_ASSERT(neq(t0, t2));
  PT_ASSERT(lt(t2, t0));
  PT_ASSERT(gt(t0, t2));
  PT_ASSERT(gt(t3, t1));
  PT_ASSERT(gt(t3, t0));
  
  del(t0); del(t1); del(t2); del(t3);
  
}

PT_FUNC(test_table_get) {
  
  var t0 = new(Table, String, Int);
  set(t0, $S("Hello"), $I(2));
  set(t0, $S("There"), $I(5));
  
  PT_ASSERT(len(t0) is 2);
  PT_ASSERT(mem(t0, $S("Hello")));
  
  rem(t0, $S("Hello"));
  
  PT_ASSERT(len(t0) is 1);
  PT_ASSERT(not mem(t0, $S("Hello")));
  PT_ASSERT(mem(t0, $S("There")));
  
  resize(t0, 0);
  
  PT_ASSERT(len(t0) is 0);
  PT_ASSERT(not mem(t0, $S("Hello")));
  PT_ASSERT(not mem(t0, $S("There")));
  
  del(t0);


  var t1 = new(Table, String, Int);
  set(t1, $S("Hello"), $I(2));
  set(t1, $S("There"), $I(5));
  
  var i0 = get(t1, $S("Hello"));
  var i1 = get(t1, $S("There"));
  
  PT_ASSERT( eq(i0, $I(2)) );
  PT_ASSERT( eq(i1, $I(5)) );
  
  set(t1, $S("Hello"), $I(6));
  
  var i2 = get(t1, $S("Hello"));
  PT_ASSERT( eq(i2, $I(6)) );
  
  del(t1);
  
  var t3 = new(Table, String, Int,
    $S("asfa"), $I(0), $S("gasg"), $I(2),
    $S("asda"), $I(1), $S("dqga"), $I(0),
    $S("jaja"), $I(7), $S("sdfa"), $I(2),
    $S("jads"), $I(7), $S("kyad"), $I(2),
    $S("kyas"), $I(7), $S("hwdw"), $I(2),
    $S("oaqv"), $I(7), $S("fgds"), $I(2),
    $S("awaa"), $I(7), $S("lkil"), $I(2));
  
  rem(t3, $S("gasg"));
  rem(t3, $S("oaqv"));
  rem(t3, $S("fgds"));
  rem(t3, $S("awaa"));
  rem(t3, $S("kyad"));
  rem(t3, $S("dqga"));
  rem(t3, $S("jaja"));
    
  del(t3);
  
}

PT_FUNC(test_table_hash) {
  
  var t0 = new(Table, String, Int);
  set(t0, $S("Hello"), $I(2));
  set(t0, $S("There"), $I(5));
  
  PT_ASSERT(hash(t0) is (
    hash($S("Hello")) ^ hash($I(2)) ^
    hash($S("There")) ^ hash($I(5))));
  
  del(t0);
  
}

PT_FUNC(test_table_iter) {
  
  var t0 = new(Table, String, Int);
  set(t0, $S("Hello"), $I(2));
  set(t0, $S("There"), $I(5));
  
  foreach(key in t0) {
    
    var val = get(t0, key);
    
    PT_ASSERT(
       (eq(key, $S("Hello")) and eq(val, $I(2)))
    or (eq(key, $S("There")) and eq(val, $I(5))));
    
  }
  
  del(t0);
  
}

PT_FUNC(test_table_len) {
  
  var t0 = new(Table, String, Int);
  set(t0, $S("Hello"), $I(2));
  set(t0, $S("There"), $I(5));
  
  PT_ASSERT(len(t0) is 2);
  
  del(t0);
  
}

PT_FUNC(test_table_new) {
  
  var t0 = new(Table, String, Int);
  set(t0, $S("Hello"), $I(2));
  set(t0, $S("There"), $I(5));
  
  var t1 = new(Table, String, Int);
  set(t1, $S("Bonjour"), $I(9));
  set(t1, $S("Where"), $I(5));
  
  var t2 = copy(t0);
  
  PT_ASSERT(t0);
  PT_ASSERT(t1);
  PT_ASSERT(t2);
  
  PT_ASSERT(t0 isnt t2);
  PT_ASSERT(t0 isnt t1);
  
  PT_ASSERT(mem(t0, $S("Hello")));
  PT_ASSERT(mem(t1, $S("Bonjour")));
  PT_ASSERT(mem(t2, $S("There")));
  
  assign(t2, t1);
  
  PT_ASSERT(mem(t2, $S("Where")));
  PT_ASSERT(mem(t2, $S("Bonjour")));
  PT_ASSERT(not mem(t2, $S("Hello")));
  PT_ASSERT(not mem(t2, $S("There")));
  
  del(t0);
  del(t1);
  del(t2);
  
  var t3 = new(Table, String, Int,
    $S("asfa"), $I(0), $S("gasg"), $I(2),
    $S("asda"), $I(1), $S("dqga"), $I(0),
    $S("jaja"), $I(7), $S("sdfa"), $I(2),
    $S("jads"), $I(7), $S("kyad"), $I(2),
    $S("kyas"), $I(7), $S("hwdw"), $I(2),
    $S("awaa"), $I(7), $S("lkil"), $I(2));
  
  del(t3);
  
  t0 = new(Table, Int, Float);
  t1 = new(Table, String, Int);
  
  PT_ASSERT(key_type(t0) is Int);
  PT_ASSERT(val_type(t0) is Float);
  PT_ASSERT(key_type(t1) is String);
  PT_ASSERT(val_type(t1) is Int);
  
  del(t0); del(t1);
  
}

PT_FUNC(test_table_resize) {
  
  var t0 = new(Table, String, Int);
  resize(t0, 1000);
  del(t0);
  
  t0 = new(Table, String, Int);
  set(t0, $S("Hello"), $I(2));
  set(t0, $S("There"), $I(5));
  
  PT_ASSERT(len(t0) is 2);
  
  resize(t0, 0);
  
  PT_ASSERT(len(t0) is 0);
  PT_ASSERT(empty(t0));
  
  del(t0);
  
}

PT_FUNC(test_table_show) {
  
  var t0 = new(Table, String, Int);
  set(t0, $S("Bonjour"), $I(9));
  set(t0, $S("Where"), $I(5));
  
  var s0 = new(String);
  
  show_to(t0, s0, 0);
  
  del(t0); del(s0);
  
}

PT_FUNC(test_table_rehash) {
  
  var t0 = new(Table, Int, Int);
  var value = $I(23);
  var test_key = NULL;

  size_t max = 1000;
  int64_t r = rand() % max;

  for (size_t i = 0; i < max; i++) {
    var key = $I(i);
    if (i == r) {
      test_key = key;
    }
    set(t0, key, value);
  }

  PT_ASSERT(test_key isnt NULL);
  PT_ASSERT(eq(get(t0, test_key), value));
  PT_ASSERT(len(t0) is max);

  del(t0);
}

PT_SUITE(suite_table) {
  PT_REG(test_table_assign);
  PT_REG(test_table_cmp);
  PT_REG(test_table_get);
  PT_REG(test_table_hash);
  PT_REG(test_table_iter);
  PT_REG(test_table_len);
  PT_REG(test_table_new);
  PT_REG(test_table_resize);
  PT_REG(test_table_show);
  PT_REG(test_table_rehash);
}

/* Thread */

PT_FUNC(test_thread_c_int) {
  
  var x = new(Thread);
  int64_t cid = c_int(current(Thread));  
  del(x);
  
}

static var set_value(var args) {
  assign(get(args, $I(0)), $I(1));
  return NULL;
}

PT_FUNC(test_thread_call) {
  
  var i = $I(0);
  
  var x = new(Thread, $(Function, set_value));
  call(x, i);
  join(x);
  
  PT_ASSERT(c_int(i) is 1);
  
  del(x);
  
}

PT_FUNC(test_thread_cmp) {
  
  PT_ASSERT(eq(current(Thread), current(Thread)));
  
}

PT_FUNC(test_thread_current) {
  var x = current(Thread);
  PT_ASSERT(x);
}

PT_FUNC(test_thread_get) {
  
  set(current(Thread), $S("Hello"), $I(1));
  PT_ASSERT(mem(current(Thread), $S("Hello")));
  PT_ASSERT(c_int(get(current(Thread), $S("Hello"))) is 1);
  rem(current(Thread), $S("Hello"));
  PT_ASSERT(not mem(current(Thread), $S("Hello")));
  
}

PT_FUNC(test_thread_hash) {
  var x = new(Thread);
  int64_t h0 = hash(current(Thread));
  int64_t h1 = hash(current(Thread));
  PT_ASSERT(h0 is h1);
  del(x);
}

PT_FUNC(test_thread_new) {
  var x = new(Thread);
  del(x);
}

PT_FUNC(test_thread_start) {
  
  
  /*
  ** TODO: Separate out Enter/Exit to Start/Stop.
  ** For Threads With should do `join` on stop
  */
  
}

#ifdef CELLO_UNIX
#include <unistd.h>
#endif

void cello_sleep(int ms) {
#if defined(CELLO_UNIX)
  usleep(ms * 1000);
#elif defined(CELLO_WINDOWS)
  Sleep(ms);
#endif
}

static var exception_sleep(var args) {
  try {
    cello_sleep(20);
    PT_ASSERT(len(current(Exception)) is 1);
  } catch(e) { }
  return NULL;
}

PT_FUNC(test_thread_exception) {
  
  var t = new(Thread, $(Function, exception_sleep));
  
  call(t);
  cello_sleep(10);
  PT_ASSERT(len(current(Exception)) is 0);
  join(t);
  
  del(t);
  
}

PT_SUITE(suite_thread) {
  PT_REG(test_thread_c_int);
  PT_REG(test_thread_call);
  PT_REG(test_thread_cmp);
  PT_REG(test_thread_current);
  PT_REG(test_thread_get);
  PT_REG(test_thread_hash);
  PT_REG(test_thread_new);
  PT_REG(test_thread_start);
  PT_REG(test_thread_exception);
}

/* Tree */

PT_FUNC(test_tree_assign) {
  
  var m0 = new(Tree, String, Int);
  var m1 = new(Tree, String, Int,
    $S("asfa"), $I(0), $S("gasg"), $I(2),
    $S("asda"), $I(1), $S("dqga"), $I(0),
    $S("jaja"), $I(7), $S("sdfa"), $I(2),
    $S("jads"), $I(7), $S("kyad"), $I(2),
    $S("kyas"), $I(7), $S("hwdw"), $I(2),
    $S("awaa"), $I(7), $S("lkil"), $I(2)); 
  
  PT_ASSERT(len(m0) is 0);
  
  assign(m0, m1);
  
  PT_ASSERT(len(m0) is 12);
  PT_ASSERT(eq(get(m0, $S("asfa")), $I(0)));
  PT_ASSERT(eq(get(m0, $S("kyad")), $I(2)));
  PT_ASSERT(eq(get(m0, $S("lkil")), $I(2)));
  PT_ASSERT(eq(get(m0, $S("jads")), $I(7)));
  
  del(m0); del(m1);
  
}

PT_FUNC(test_tree_resize) {
  
  var m0 = new(Tree, String, Int);
  set(m0, $S("Hello"), $I(2));
  set(m0, $S("There"), $I(5));
  
  PT_ASSERT(len(m0) is 2);
  
  resize(m0, 0);
  
  PT_ASSERT(len(m0) is 0);
  PT_ASSERT(empty(m0));
  
  del(m0);
  
}

PT_FUNC(test_tree_cmp) {
  
  var m0 = new(Tree, String, Int,
    $S("Hello"), $I(2),
    $S("There"), $I(5));
  
  var m1 = new(Tree, String, Int,
    $S("Hello"), $I(2),
    $S("There"), $I(5));
  
  var m2 = new(Tree, String, Int,
    $S("Bello"), $I(2),
    $S("There"), $I(5));
  
  var m3 = new(Table, String, Int,
    $S("Hello"), $I(2),
    $S("There"), $I(5),
    $S("World"), $I(6));
  
  PT_ASSERT(eq(m0, m1));
  PT_ASSERT(neq(m0, m2));
  PT_ASSERT(lt(m2, m0));
  PT_ASSERT(gt(m0, m2));
  PT_ASSERT(gt(m3, m1));
  PT_ASSERT(gt(m3, m0));
  
  del(m0); del(m1); del(m2); del(m3);
  
}

PT_FUNC(test_tree_get) {
  
  var m0 = new(Tree, String, Int);
  set(m0, $S("Hello"), $I(2));
  set(m0, $S("There"), $I(5));
  
  PT_ASSERT(len(m0) is 2);
  PT_ASSERT(mem(m0, $S("Hello")));
  
  rem(m0, $S("Hello"));
  
  PT_ASSERT(len(m0) is 1);
  PT_ASSERT(not mem(m0, $S("Hello")));
  PT_ASSERT(mem(m0, $S("There")));
  
  resize(m0, 0);
  
  PT_ASSERT(len(m0) is 0);
  PT_ASSERT(not mem(m0, $S("Hello")));
  PT_ASSERT(not mem(m0, $S("There")));
  
  del(m0);


  var m1 = new(Tree, String, Int);
  set(m1, $S("Hello"), $I(2));
  set(m1, $S("There"), $I(5));
  
  var i0 = get(m1, $S("Hello"));
  var i1 = get(m1, $S("There"));
  
  PT_ASSERT( eq(i0, $I(2)) );
  PT_ASSERT( eq(i1, $I(5)) );
  
  set(m1, $S("Hello"), $I(6));
  
  var i2 = get(m1, $S("Hello"));
  PT_ASSERT( eq(i2, $I(6)) );
  
  del(m1);
  
  var m3 = new(Tree, String, Int,
    $S("asfa"), $I(0), $S("gasg"), $I(2),
    $S("asda"), $I(1), $S("dqga"), $I(0),
    $S("jaja"), $I(7), $S("sdfa"), $I(2),
    $S("jads"), $I(7), $S("kyad"), $I(2),
    $S("kyas"), $I(7), $S("hwdw"), $I(2),
    $S("oaqv"), $I(7), $S("fgds"), $I(2),
    $S("awaa"), $I(7), $S("lkil"), $I(2));
  
  rem(m3, $S("gasg"));
  rem(m3, $S("oaqv"));
  rem(m3, $S("fgds"));
  rem(m3, $S("awaa"));
  rem(m3, $S("kyad"));
  rem(m3, $S("dqga"));
  rem(m3, $S("jaja"));
    
  del(m3);
  
  m0 = new(Tree, Int, Float);
  m1 = new(Tree, String, Int);
  
  PT_ASSERT(key_type(m0) is Int);
  PT_ASSERT(val_type(m0) is Float);
  PT_ASSERT(key_type(m1) is String);
  PT_ASSERT(val_type(m1) is Int);
  
  del(m0); del(m1);
  
}

PT_FUNC(test_tree_hash) {
  
  var m0 = new(Tree, String, Int);
  set(m0, $S("Hello"), $I(2));
  set(m0, $S("There"), $I(5));
  
  PT_ASSERT(hash(m0) is (
    hash($S("Hello")) ^ hash($I(2)) ^
    hash($S("There")) ^ hash($I(5))));
  
  del(m0);
  
}

PT_FUNC(test_tree_iter) {
  
  var m0 = new(Tree, String, Int);
  set(m0, $S("Hello"), $I(2));
  set(m0, $S("There"), $I(5));
  
  foreach(key in m0) {
    
    var val = get(m0, key);
    
    PT_ASSERT(
       (eq(key, $S("Hello")) and eq(val, $I(2)))
    or (eq(key, $S("There")) and eq(val, $I(5))));
    
  }
  
  del(m0);
  
}

PT_FUNC(test_tree_len) {
  
  var m0 = new(Tree, String, Int);
  set(m0, $S("Hello"), $I(2));
  set(m0, $S("There"), $I(5));
  
  PT_ASSERT(len(m0) is 2);
  
  del(m0);
  
}

PT_FUNC(test_tree_new) {
  
  var m0 = new(Tree, String, Int);
  set(m0, $S("Hello"), $I(2));
  set(m0, $S("There"), $I(5));
  
  var m1 = new(Tree, String, Int);
  set(m1, $S("Bonjour"), $I(9));
  set(m1, $S("Where"), $I(5));
  
  var m2 = copy(m0);
  
  PT_ASSERT(m0);
  PT_ASSERT(m1);
  PT_ASSERT(m2);
  
  PT_ASSERT(m0 isnt m2);
  PT_ASSERT(m0 isnt m1);
  
  PT_ASSERT(mem(m0, $S("Hello")));
  PT_ASSERT(mem(m1, $S("Bonjour")));
  PT_ASSERT(mem(m2, $S("There")));
  
  assign(m2, m1);
  
  PT_ASSERT(mem(m2, $S("Where")));
  PT_ASSERT(mem(m2, $S("Bonjour")));
  PT_ASSERT(not mem(m2, $S("Hello")));
  PT_ASSERT(not mem(m2, $S("There")));
  
  del(m0);
  del(m1);
  del(m2);
  
  var m3 = new(Tree, String, Int,
    $S("asfa"), $I(0), $S("gasg"), $I(2),
    $S("asda"), $I(1), $S("dqga"), $I(0),
    $S("jaja"), $I(7), $S("sdfa"), $I(2),
    $S("jads"), $I(7), $S("kyad"), $I(2),
    $S("kyas"), $I(7), $S("hwdw"), $I(2),
    $S("awaa"), $I(7), $S("lkil"), $I(2));
  
  del(m3);
  
}

PT_FUNC(test_tree_show) {
  
  var m0 = new(Tree, String, Int);
  set(m0, $S("Bonjour"), $I(9));
  set(m0, $S("Where"), $I(5));
  
  var s0 = new(String);
  
  show_to(m0, s0, 0);
  
  del(m0); del(s0);
  
}

PT_SUITE(suite_tree) {
  PT_REG(test_tree_assign);
  PT_REG(test_tree_resize);
  PT_REG(test_tree_cmp);
  PT_REG(test_tree_get);
  PT_REG(test_tree_hash);
  PT_REG(test_tree_iter);
  PT_REG(test_tree_len);
  PT_REG(test_tree_new);
  PT_REG(test_tree_show);
}

/* Tuple */

PT_FUNC(test_tuple_assign) {
  
  var x = new(Tuple);
  var y = tuple($I(100), $I(200), $I(300));
  assign(x, y);
  
  PT_ASSERT(c_int(get(x, $I(0))) is 100);
  PT_ASSERT(c_int(get(x, $I(1))) is 200);
  PT_ASSERT(c_int(get(x, $I(2))) is 300);
  PT_ASSERT(len(y) is 3);
  
  var z = new(Array, Ref);
  assign(z, x);
  
  PT_ASSERT(c_int(deref(get(z, $I(0)))) is 100);
  PT_ASSERT(c_int(deref(get(z, $I(1)))) is 200);
  PT_ASSERT(c_int(deref(get(z, $I(2)))) is 300);
  PT_ASSERT(len(z) is 3);
  
  del(z);
  del(x);
  
  
}

PT_FUNC(test_tuple_resize) {
  
  var x = new(Tuple, $I(100), $I(200), $I(300));
  
  PT_ASSERT(len(x) is 3);
  resize(x, 0);
  PT_ASSERT(len(x) is 0);
  
  del(x);
  
}

PT_FUNC(test_tuple_cmp) {
  
  var x = tuple($I(100), $I(200), $I(300));
  var y = tuple($I(100), $I(200), $I(300));
  PT_ASSERT(eq(x, y));
  
  set(y, $I(0), $I(0));
  PT_ASSERT(neq(x, y));
  
}

PT_FUNC(test_tuple_concat) {
  
  var x = new(Tuple, $I(100), $I(200), $I(300));
  concat(x, tuple($I(10), $I(20)));
  
  PT_ASSERT(c_int(get(x, $I(0))) is 100);
  PT_ASSERT(c_int(get(x, $I(1))) is 200);
  PT_ASSERT(c_int(get(x, $I(2))) is 300);
  PT_ASSERT(c_int(get(x, $I(3))) is 10);
  PT_ASSERT(c_int(get(x, $I(4))) is 20);
  
  del(x);
  
}

PT_FUNC(test_tuple_get) {
  
  var x = new(Tuple, $I(100), $I(200), $I(300));
  
  PT_ASSERT(c_int(get(x, $I(0))) is 100);
  PT_ASSERT(c_int(get(x, $I(1))) is 200);
  PT_ASSERT(c_int(get(x, $I(2))) is 300);
  
  PT_ASSERT(c_int(get(x, $I(-1))) is 300);
  PT_ASSERT(c_int(get(x, $I(-2))) is 200);
  PT_ASSERT(c_int(get(x, $I(-3))) is 100);
  
  set(x, $I(1), $I(10));
  
  PT_ASSERT(c_int(get(x, $I(1))) is 10);

  PT_ASSERT(mem(x, $I(10)));
  PT_ASSERT(mem(x, $I(100)));
  PT_ASSERT(mem(x, $I(300)));

  rem(x, $I(10));
  
  PT_ASSERT(c_int(get(x, $I(0))) is 100);
  PT_ASSERT(c_int(get(x, $I(1))) is 300);

  del(x);
  
}

PT_FUNC(test_tuple_hash) {
  
  var x = tuple($S("Hello"), $S("World"), $S("!"));
  
  PT_ASSERT(hash(x) is (
    hash($S("Hello")) ^ hash($S("World")) ^ hash($S("!"))));
  
}

PT_FUNC(test_tuple_iter) {
  
  size_t i = 0;
  foreach (x in tuple($I(10), $I(20), $I(30))) {
    if (i is 0) { PT_ASSERT(eq(x, $I(10))); }
    if (i is 1) { PT_ASSERT(eq(x, $I(20))); }
    if (i is 2) { PT_ASSERT(eq(x, $I(30))); }
    i++;
  }
  
}

PT_FUNC(test_tuple_len) {
  
  var x = tuple($I(1), $I(2));
  var y = tuple($I(1), $I(2), $I(3));
  var z = tuple($I(1), $I(2), $I(3), $I(10));
  
  PT_ASSERT(len(x) is 2);
  PT_ASSERT(len(y) is 3);
  PT_ASSERT(len(z) is 4);
  
}

PT_FUNC(test_tuple_new) {
  
  var x = new(Tuple, $I(100), $S("Test"));
  PT_ASSERT(x);
  del(x);
  
}

PT_FUNC(test_tuple_push) {
  
  var x = new(Tuple);
  
  push(x, $S("Hello"));
  PT_ASSERT(eq(get(x, $I(0)), $S("Hello")));
  
  push(x, $S("World"));
  PT_ASSERT(eq(get(x, $I(0)), $S("Hello")));
  PT_ASSERT(eq(get(x, $I(1)), $S("World")));
  
  pop_at(x, $I(0));
  PT_ASSERT(eq(get(x, $I(0)), $S("World")));
  
  push_at(x, $S("Hello"), $I(0));
  PT_ASSERT(eq(get(x, $I(0)), $S("Hello")));
  PT_ASSERT(eq(get(x, $I(1)), $S("World")));
  
  del(x);
  
}

PT_FUNC(test_tuple_show) {
  
  var a0 = tuple($I(1), $I(5), $I(9));
  var s0 = new(String);
  
  show_to(a0, s0, 0);
  
  del(s0);
  
}

PT_FUNC(test_tuple_sort) {
  
  var a0 = tuple($I(100), $I(1233), $I(1), $I(2312), $I(21));
  var a1 = tuple($I(1), $I(21), $I(100), $I(1233), $I(2312));
  var a2 = tuple($I(2312), $I(1233), $I(100), $I(21), $I(1));
  
  var a3 = copy(a0);
  var a4 = copy(a0);
  
  sort(a3);
  sort_by(a4, gt);
  PT_ASSERT(eq(a3, a1));
  PT_ASSERT(eq(a4, a2));
  
  del(a3); del(a4);
  
}

PT_SUITE(suite_tuple) {
  PT_REG(test_tuple_assign);
  PT_REG(test_tuple_resize);
  PT_REG(test_tuple_cmp);
  PT_REG(test_tuple_concat);
  PT_REG(test_tuple_get);
  PT_REG(test_tuple_hash);
  PT_REG(test_tuple_iter);
  PT_REG(test_tuple_len);
  PT_REG(test_tuple_new);
  PT_REG(test_tuple_push);
  PT_REG(test_tuple_show);
  PT_REG(test_tuple_sort);
}

/* Type */

static var TestType;

struct TestType {
  int64_t test_data;
};

static void TestType_New(var self, var args) {
  struct TestType* tt = self;
  tt->test_data = c_int(get(args, $I(0)));
}

static int TestType_Cmp(var self, var obj) {
  struct TestType* lhs = cast(self, TestType);
  struct TestType* rhs = cast(obj, TestType);
  return (int)(lhs->test_data - rhs->test_data);
}

PT_FUNC(test_type_new) {
  
  TestType = new_root(Type, 
    $S("TestType"), 
    $I(sizeof(struct TestType)),
    $(New, TestType_New, NULL),
    $(Cmp, TestType_Cmp));
  
  PT_ASSERT(TestType);
  PT_ASSERT_STR_EQ(c_str(TestType), "TestType");

  var test_obj1 = new(TestType, $I(1));
  var test_obj2 = new(TestType, $I(1));
  var test_obj3 = new(TestType, $I(4));

  PT_ASSERT(test_obj1);
  PT_ASSERT(test_obj2);
  PT_ASSERT(test_obj3);
  
  PT_ASSERT(eq(test_obj1, test_obj2));
  PT_ASSERT(neq(test_obj1, test_obj3));
  
  del(test_obj1);
  del(test_obj2);
  del(test_obj3);
  
  del_root(TestType);
  
}

PT_FUNC(test_type_c_str) {
  PT_ASSERT_STR_EQ(c_str(Type),  "Type");
  PT_ASSERT_STR_EQ(c_str(Int),   "Int");
  PT_ASSERT_STR_EQ(c_str(Float), "Float");
  PT_ASSERT_STR_EQ(c_str(Array), "Array");
}

PT_FUNC(test_type_cmp) {
  PT_ASSERT(gt(Type, Int));
  PT_ASSERT(lt(Array, Float));
}

PT_FUNC(test_type_hash) {
  PT_ASSERT(hash(Type) is hash($S("Type")));
  PT_ASSERT(hash(Int) is hash($S("Int")));
  PT_ASSERT(hash(Array) is hash($S("Array")));
}

PT_FUNC(test_type_help) {
  /* TODO */
}

PT_FUNC(test_type_show) {
  var s = new(String);
  show_to(Int, s, 0);
  PT_ASSERT_STR_EQ(c_str(s), "Int");
  show_to(Type, s, 0);
  PT_ASSERT_STR_EQ(c_str(s), "Type");
  show_to(Array, s, 0);
  PT_ASSERT_STR_EQ(c_str(s), "Array");
  del(s);
}

PT_SUITE(suite_type) {
  PT_REG(test_type_new);
  PT_REG(test_type_c_str);
  PT_REG(test_type_cmp);
  PT_REG(test_type_hash);
  PT_REG(test_type_help);
  PT_REG(test_type_show);
}

/* Zip */

PT_FUNC(test_zip_get) {
  
  var x = tuple($I(100), $I(10), $I(30), $I(50), $I(6), $I(1));
  var y = tuple($I(10), $I(213), $I(12), $I(88), $I(99));
  var z = zip(x, y);
  
  PT_ASSERT(eq(get(z, $I(0)), tuple($I(100), $I( 10))));
  PT_ASSERT(eq(get(z, $I(1)), tuple($I( 10), $I(213))));
  PT_ASSERT(eq(get(z, $I(2)), tuple($I( 30), $I( 12))));
  PT_ASSERT(eq(get(z, $I(3)), tuple($I( 50), $I( 88))));
  PT_ASSERT(eq(get(z, $I(4)), tuple($I(  6), $I( 99))));
  
}

PT_FUNC(test_zip_iter) {
  
  var x = tuple($I(100), $I(10), $I(30), $I(50), $I(6), $I(1));
  var y = tuple($I(10), $I(213), $I(12), $I(88), $I(99));
  var z = tuple($I(10), $I(213));
  
  /* TODO: Improve */
  foreach (i in zip(x)) {}
  foreach (i in zip(x, y)) {}
  foreach (i in zip(x, y, z)) {}
  foreach (i in zip(y, z)) {}
  
  PT_ASSERT(iter_type(zip(x, y, z)) is Tuple);
  
  x = new(Array, Int, $I(100), $I(200), $I(130));
  y = new(Array, Float, $F(0.1), $F(0.2), $F(1.3));

  foreach (pair in zip(x, y)) {}
  foreach (pair in enumerate(x)) {}
  
  del(x);
  del(y);
  
}

PT_FUNC(test_zip_len) {
  
  var x = tuple($I(100), $I(10), $I(30), $I(50), $I(6), $I(1));
  var y = tuple($I(10), $I(213), $I(12), $I(88), $I(99));
  var z = tuple($I(10), $I(213));
  
  PT_ASSERT(len(zip(z)) is 2);
  PT_ASSERT(len(zip(z, y)) is 2);
  PT_ASSERT(len(zip(x, y, z)) is 2);
  PT_ASSERT(len(zip(x, y)) is 5);
  PT_ASSERT(len(zip(x)) is 6);
  
}

PT_FUNC(test_zip_new) {
  
  var x = tuple($I(100), $I(10), $I(30), $I(50), $I(6), $I(1));
  var y = tuple($I(10), $I(213), $I(12), $I(88), $I(99));
  var z = new(Zip, x, y);
  
  PT_ASSERT(eq(get(z, $I(0)), tuple($I(100), $I( 10))));
  PT_ASSERT(eq(get(z, $I(1)), tuple($I( 10), $I(213))));
  PT_ASSERT(eq(get(z, $I(2)), tuple($I( 30), $I( 12))));
  
  del(z);
  
}

PT_SUITE(suite_zip) {
  PT_REG(test_zip_get);
  PT_REG(test_zip_iter);
  PT_REG(test_zip_len);
  PT_REG(test_zip_new);
}


/* Exception */

static var DivideByZeroError = CelloEmpty(DivideByZeroError);
static var OtherError = CelloEmpty(OtherError);

static int exception_divide(int x, int y) {
  if (y == 0) {
    throw(DivideByZeroError, "%i / %i", $I(x), $I(y));
    return 0;
  } else {
    return x / y;
  }
}

PT_FUNC(test_exception_throw) {
  
  int r0 = exception_divide(2, 1);
  int r1 = exception_divide(4, 2);
  int r2 = exception_divide(9, 3);
  
  PT_ASSERT(r0 == 2);
  PT_ASSERT(r1 == 2);
  PT_ASSERT(r2 == 3);
  
  PT_ASSERT(len(current(Exception)) is 0);
  
}

PT_FUNC(test_exception_catch) {
  
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
  } catch (e) {
    reached2 = true;
  }
  
  PT_ASSERT(reached0);
  PT_ASSERT(not reached1);
  PT_ASSERT(not reached2);
  
  PT_ASSERT(len(current(Exception)) is 0);
  
}

PT_FUNC(test_exception_catch_all) {
  
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
  PT_ASSERT(len(current(Exception)) is 0);
  
}

PT_FUNC(test_exception_catch_outer) {

  volatile bool reached0 = false;
  volatile bool reached1 = false;
  
  try {

  PT_ASSERT(len(current(Exception)) is 1);

    try {
      PT_ASSERT(len(current(Exception)) is 2);
      exception_divide(2, 0);
    } catch (e in TypeError) {
      reached0 = true;
    }    

    PT_ASSERT(len(current(Exception)) is 1);
  
  } catch (e) {
    reached1  = true;
  }
  
  PT_ASSERT(not reached0);
  PT_ASSERT(reached1);
  
  PT_ASSERT(len(current(Exception)) is 0);
  
}

PT_SUITE(suite_exception) {
  
  PT_REG(test_exception_throw);
  PT_REG(test_exception_catch);
  PT_REG(test_exception_catch_all);
  PT_REG(test_exception_catch_outer);

}

int main(int argc, char** argv) {
  
  pt_add_suite(suite_array);
  pt_add_suite(suite_box);
  pt_add_suite(suite_file);
  pt_add_suite(suite_float);
  pt_add_suite(suite_filter);
  pt_add_suite(suite_function);
  pt_add_suite(suite_int);
  pt_add_suite(suite_list);
  pt_add_suite(suite_map);
#if defined(CELLO_WINDOWS) || defined(CELLO_UNIX)
  pt_add_suite(suite_mutex);
#endif
  pt_add_suite(suite_range);
  pt_add_suite(suite_ref);
  pt_add_suite(suite_slice);
  pt_add_suite(suite_string);
  pt_add_suite(suite_table);
#if defined(CELLO_WINDOWS) || defined(CELLO_UNIX)
  pt_add_suite(suite_thread);
#endif
  pt_add_suite(suite_tree);
  pt_add_suite(suite_tuple);
  pt_add_suite(suite_type);
  pt_add_suite(suite_zip);
  pt_add_suite(suite_exception);
  
  pt_run();
  
  return 0;
  
}
