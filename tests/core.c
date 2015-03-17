#include "Cello.h"
#include "ptest.h"

PT_FUNC(test_doc) {
  /* TODO: Finish */
}

PT_FUNC(test_type) {
  PT_ASSERT(type_of($I(1)) is Int);
  PT_ASSERT(type_of($F(1.0)) is Float);
  PT_ASSERT(type_of(Int) is Type);
  PT_ASSERT(type_of(Float) is Type);
  PT_ASSERT(type_of(Type) is Type);
}

PT_FUNC(test_cast) {
  
  var x = $I(1);
  var y = $F(2.0);

  x = cast(x, Int);  
  y = cast(y, Float);
  
  PT_ASSERT(x);
  PT_ASSERT(y);
  
}

PT_FUNC(test_new) {
  
  var x = new_$I(1);
  
  PT_ASSERT(x);
  PT_ASSERT(type_of(x) is Int);
  PT_ASSERT(c_int(x) is 1);
  
  del(x);
  
  var y = $F(0.0);  
  construct(y, $F(1.0));
  PT_ASSERT(c_float(y) is 1.0);
  
  var z = alloc(String);
  PT_ASSERT(z);
  construct(z, $S("Hello"));
  PT_ASSERT_STR_EQ(c_str(z), "Hello");
  z = destruct(z);
  dealloc(z);
  
}

PT_FUNC(test_assign) {
  
  var x = new(Int, $I(10));
  var y = new(Int, $I(20));

  PT_ASSERT(c_int(x) is 10);
  PT_ASSERT(c_int(y) is 20);
  PT_ASSERT(x isnt y);

  assign(x, y);
  
  PT_ASSERT(c_int(x) is 20);
  PT_ASSERT(c_int(y) is 20);
  PT_ASSERT(x isnt y);
  
  del(x);
  del(y);
  
  var xs = new_$S("Hello");
  var ys = new_$S("There");
  
  PT_ASSERT_STR_EQ(c_str(xs), "Hello");
  PT_ASSERT_STR_EQ(c_str(ys), "There");
  PT_ASSERT(c_str(xs) isnt c_str(ys));
  PT_ASSERT(xs isnt ys);

  assign(xs, ys);
  
  PT_ASSERT_STR_EQ(c_str(xs), "There");
  PT_ASSERT_STR_EQ(c_str(ys), "There");
  PT_ASSERT(c_str(xs) isnt c_str(ys));
  PT_ASSERT(xs isnt ys);
  
  del(xs);
  del(ys);
}

PT_FUNC(test_copy) {
  
  var x = new(String, $S("Hello"));
  var y = copy(x);
  
  PT_ASSERT_STR_EQ(c_str(x), c_str(y));
  PT_ASSERT(c_str(x) isnt c_str(y));
  PT_ASSERT(x isnt y);
  
  del(x);
  del(y);
  
}

PT_FUNC(test_traverse) {
  /* TODO */
}

PT_FUNC(test_eq) {
  PT_ASSERT(  eq($I(1 ), $I(1 )) );
  PT_ASSERT( neq($I(2 ), $I(20)) );
  PT_ASSERT(  eq($S("Hello"), $S("Hello")) );
  PT_ASSERT( neq($S("Hello"), $S("There")) );
  
  var tab1 = new(Table, String, Int);
  var tab2 = new(Table, String, Int);
  
  PT_ASSERT(eq(tab1, tab2));
  set(tab1, $S("apple"), $I(10));
  PT_ASSERT(neq(tab1, tab2));
  set(tab2, $S("apple"), $I(10));
  PT_ASSERT(eq(tab1, tab2));
  set(tab1, $S("pear"), $I(20));
  set(tab2, $S("pear"), $I(30));
  PT_ASSERT(neq(tab1, tab2));
  set(tab1, $S("pear"), $I(30));
  PT_ASSERT(eq(tab1, tab2));
  set(tab2, $S("banana"), $I(10));
  PT_ASSERT(neq(tab1, tab2));
  
  del(tab1);
  del(tab2);
  
  var tab3 = new(Table, String, String);
  var tab4 = new(Table, String, String);
  
  PT_ASSERT(eq(tab3, tab4));
  set(tab3, $S("name"), $S("Alex"));
  PT_ASSERT(neq(tab3, tab4));
  set(tab4, $S("name"), $S("Alex"));
  PT_ASSERT(eq(tab3, tab4));
  set(tab3, $S("age"), $S("28"));
  set(tab4, $S("age"), $S("30"));
  PT_ASSERT(neq(tab3, tab4));
  set(tab3, $S("age"), $S("30"));
  PT_ASSERT(eq(tab3, tab4));
  set(tab4, $S("nickname"), $S("The Wing Man"));
  PT_ASSERT(neq(tab3, tab4));
  
  del(tab3);
  del(tab4);
  
}

PT_FUNC(test_ord) {
  PT_ASSERT(gt($I(15), $I( 3)));
  PT_ASSERT(lt($I(70), $I(81)));
  PT_ASSERT(ge($I(71), $I(71)));
  PT_ASSERT(ge($I(78), $I(71)));
  PT_ASSERT(le($I(32), $I(32)));
  PT_ASSERT(le($I(21), $I(32)));
}

PT_FUNC(test_hash) {
  uint64_t x = hash($I(1  ));
  uint64_t y = hash($I(123));
  
  PT_ASSERT(x is 1);
  PT_ASSERT(y is 123);
}

PT_FUNC(test_len) {

  PT_ASSERT(len($T($I(1), $I(2), $I(3))) is 3);
  PT_ASSERT(len($S("Hello")) is 5);
  
  var x = new(Array, Int, $I(1), $I(2));
  PT_ASSERT(len(x) is 2);
  del(x);

}


PT_FUNC(test_iter) {

  var x = new(Array, Ref, $I(1), $F(2.0), $S("Hello"));
  
  foreach (y in x) {
    PT_ASSERT(y);
    PT_ASSERT(type_of(y) is Ref);
  }
  
  del(x);
}

PT_FUNC(test_push) {
  
  var x = new(Array, Int);
  var y = new(Array, Ref);
  
  for(int i = 0; i < 1000; i++) {
    push(x, $I(1));
    push(y, $I(2));
  }
  
  for(int i = 0; i < 1000; i++) {
    pop(x);
    pop(y);
  }
  
  push(x, $I(0));
  push(x, $I(5));
  push(x, $I(10));
  
  PT_ASSERT(eq(get(x, $I(0)), $I(0)));
  PT_ASSERT(eq(get(x, $I(1)), $I(5)));
  PT_ASSERT(eq(get(x, $I(2)), $I(10)));
  
  pop_at(x, $I(1));
  
  PT_ASSERT(eq(get(x, $I(0)), $I(0)));
  PT_ASSERT(eq(get(x, $I(1)), $I(10)));
  
  del(x);
  del(y);
}

PT_FUNC(test_concat) {
  /* TODO */
}

PT_FUNC(test_get) {

  var x = new(Array, Int, $I(1), $I(2), $I(3));
  
  PT_ASSERT(x);
  PT_ASSERT(len(x) is 3);
  PT_ASSERT(mem(x, $I(1)));
  PT_ASSERT(mem(x, $I(2)));
  PT_ASSERT(mem(x, $I(3)));
  
  rem(x, $I(2));
  
  PT_ASSERT(x);
  PT_ASSERT(len(x) is 2);
  PT_ASSERT(mem(x, $I(1)));
  PT_ASSERT(mem(x, $I(3)));
  
  clear(x);
  
  PT_ASSERT(x);
  PT_ASSERT(len(x) is 0);
  PT_ASSERT(empty(x));
  
  del(x);
  
  var y = new(Array, Float, 
    $F(5.2), $F(7.1), $F(2.2), $F(1.1));
  
  PT_ASSERT(y);
  PT_ASSERT(len(y) is 4);
  PT_ASSERT(mem(y, $F(5.2)));
  PT_ASSERT(mem(y, $F(7.1)));
  PT_ASSERT(mem(y, $F(2.2)));
  PT_ASSERT(mem(y, $F(1.1)));
  
  sort(y);
  
  PT_ASSERT(eq(get(y, $I(0)), $F(1.1)));
  PT_ASSERT(eq(get(y, $I(1)), $F(2.2)));
  PT_ASSERT(eq(get(y, $I(2)), $F(5.2)));
  PT_ASSERT(eq(get(y, $I(3)), $F(7.1)));
  
  del(y);
  
  var z = new(Array, Float, 
    $F(5.2), $F(7.1), $F(2.2), $F(1.1));
  
  sort(z);
  
  PT_ASSERT(eq(get(z, $I(0)), $F(1.1)));
  PT_ASSERT(eq(get(z, $I(1)), $F(2.2)));
  PT_ASSERT(eq(get(z, $I(2)), $F(5.2)));
  PT_ASSERT(eq(get(z, $I(3)), $F(7.1)));
  
  del(z);
  
  var w = new(Array, Int, 
    $I(135), $I(11), $I(254), 
    $I(123213), $I(22), $I(1));
  
  sort(w);
  
  PT_ASSERT(eq(get(w, $I(0)), $I(1)));
  PT_ASSERT(eq(get(w, $I(1)), $I(11)));
  PT_ASSERT(eq(get(w, $I(2)), $I(22)));
  PT_ASSERT(eq(get(w, $I(3)), $I(135)));
  PT_ASSERT(eq(get(w, $I(4)), $I(254)));
  PT_ASSERT(eq(get(w, $I(5)), $I(123213)));
  
  del(w);
  
  var tab3 = new(Table, String, String);
  var tab4 = new(Table, String, String);
  
  PT_ASSERT(eq(tab3, tab4));
  
  set(tab3, $S("key"), $S("val"));
  PT_ASSERT(neq(tab3, tab4));
  
  set(tab4, $S("key"), $S("val"));
  PT_ASSERT(eq(tab3, tab4));
  
  set(tab3, $S("newkey"), $S("10"));
  set(tab4, $S("newkey"), $S("newval"));
  PT_ASSERT(neq(tab3, tab4));
  
  set(tab3, $S("newkey"), $S("newval"));
  PT_ASSERT(eq(tab3, tab4));
  
  set(tab4, $S("difkey"), $S("5"));
  PT_ASSERT(neq(tab3, tab4));
  
  del(tab3);
  del(tab4);

  var fst = $I(1);
  var snd = $F(2.0);
  var trd = $S("Hello");

  var arr = new(Array, Ref, fst, snd, trd);

  PT_ASSERT(deref(get(arr, $I(0))) is fst);
  PT_ASSERT(deref(get(arr, $I(1))) is snd);
  PT_ASSERT(deref(get(arr, $I(2))) is trd);
  
  set(arr, $I(1), trd);
  
  PT_ASSERT(deref(get(arr, $I(1))) is trd);
  
  del(arr);

  var prices = new(Table, String, Int);
  set(prices, $S("Apple"),  $I(12)); 
  set(prices, $S("Banana"), $I( 6)); 
  set(prices, $S("Pear"),   $I(55)); 
  
  var pear_price = get(prices, $S("Pear"));
  var banana_price = get(prices, $S("Banana"));
  var apple_price = get(prices, $S("Apple"));
  
  PT_ASSERT(c_int(pear_price) is 55);
  PT_ASSERT(c_int(banana_price) is 6);
  PT_ASSERT(c_int(apple_price) is 12);
  
  char name[5];
  
  for(int i = 0; i < 10; i++) {
    sprintf(name, "%i", i);
    set(prices, $S(name), $I(i));
  }
  
  for(int i = 0; i < 10; i++) {
    sprintf(name, "%i", i);
    rem(prices, $S(name));
  }
  
  del(prices);
  
}

PT_FUNC(test_reverse) {
  /* TODO */
}

PT_FUNC(test_sort) {
  /* TODO */
}

PT_FUNC(test_clear) {
  /* TODO */
}

PT_FUNC(test_reserve) {
  /* TODO */
}

PT_FUNC(test_ctype) {
  
  PT_ASSERT_STR_EQ(c_str($S("Hello")), "Hello");
  PT_ASSERT_STR_EQ(c_str($S("There")), "There");
  
  PT_ASSERT(c_int($I(5)) is 5);
  PT_ASSERT(c_int($I(1)) is 1);
  PT_ASSERT(c_int($I(6)) is 6);
    
  PT_ASSERT(c_float($F(5.1)) is 5.1);
  PT_ASSERT(c_float($F(5.2)) is 5.2);
  PT_ASSERT(c_float($F(9.8)) is 9.8);
 
}

PT_FUNC(test_begin) {
  /* TODO */
}

PT_FUNC(test_stream) {

  var f = $(File);
  
  PT_ASSERT(f);
  
  sopen(f, $S("test.bin"), $S("w"));
    
    PT_ASSERT(f);
    
    swrite(f, $I(1), $I(size(Int)));
    swrite(f, $I(22), $I(size(Int)));
  
  sclose(f);
  
  sopen(f, $S("test.bin"), $S("r"));
    
    PT_ASSERT(f);
    
    var fst = $I(0);
    var snd = $I(0);
    
    sread(f, fst, $I(size(Int)));
    sread(f, snd, $I(size(Int)));
    
    PT_ASSERT(c_int(fst) is 1);
    PT_ASSERT(c_int(snd) is 22);
    
  sclose(f);
  
  PT_ASSERT(f);
}

static var TestType;

struct TestType {
  int64_t test_data;
};

static var TestType_New(var self, var args) {
  struct TestType* tt = self;
  tt->test_data = c_int(get(args, $I(0)));
  return self;
}

static var TestType_Del(var self) {
  return self;
}

static bool TestType_Eq(var self, var obj) {
  struct TestType* lhs = cast(self, TestType);
  struct TestType* rhs = cast(obj, TestType);
  return lhs->test_data == rhs->test_data;
}

PT_FUNC(test_type_new) {
 
  TestType = new_root(Type, $S("TestType"), $I(sizeof(struct TestType)),
    $(New, TestType_New, TestType_Del),
    $(Eq, TestType_Eq));
  
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
  
  del(TestType);
  
}

PT_FUNC(test_pointer) {
    /* TODO */
}

PT_FUNC(test_call) {
  /* TODO */
}

PT_FUNC(test_math) {
  /* TODO */
}

PT_FUNC(test_start) {
  /* TODO */
}

PT_FUNC(test_type_implements) {
  
  PT_ASSERT(type_implements(Int, New));
  PT_ASSERT(type_implements(Float, C_Float));
  PT_ASSERT(type_implements(String, Eq));
  
  PT_ASSERT(type_instance(Int, Ord));
  PT_ASSERT(type_instance(Array, Get));
  PT_ASSERT(type_instance(Type, C_Str));
  
}

PT_FUNC(test_show) {
  
  var out = new(String, $S(""));
  
  print_to(out, 0, "This is an %s %i %i",
    $S("example"), $I(10), $I(1));
  
  PT_ASSERT_STR_EQ(c_str(out), "This is an example 10 1");
  
  del(out);
  
}

PT_FUNC(test_look) {
  
  var x = $I(0); 
  var y = $I(0); 
  var z = $I(0);
  var w = $I(0);
  
  scan_from($S("5 10 1 0"), 0, "%i %i %i %i", x, y, z, w);
  
  PT_ASSERT(eq(x, $I(5)));
  PT_ASSERT(eq(y, $I(10)));
  PT_ASSERT(eq(z, $I(1)));
  PT_ASSERT(eq(w, $I(0)));
  
}

PT_SUITE(suite_core) {

  PT_REG(test_doc);  
  PT_REG(test_type);
  PT_REG(test_cast);
  PT_REG(test_new);
  PT_REG(test_assign);
  PT_REG(test_copy);
  PT_REG(test_traverse);
  PT_REG(test_eq);
  PT_REG(test_ord);
  PT_REG(test_hash);
  PT_REG(test_len);
  PT_REG(test_iter);
  PT_REG(test_push);
  PT_REG(test_concat);
  PT_REG(test_get);
  PT_REG(test_reverse);
  PT_REG(test_sort);
  PT_REG(test_clear);
  PT_REG(test_reserve);
  PT_REG(test_ctype);
  PT_REG(test_begin);
  PT_REG(test_stream);
  PT_REG(test_pointer);
  PT_REG(test_call);
  PT_REG(test_show);
  PT_REG(test_look);
  PT_REG(test_math);
  PT_REG(test_start);
  PT_REG(test_type_new);
  PT_REG(test_type_implements);

}
