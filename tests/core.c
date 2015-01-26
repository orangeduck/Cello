#include "Cello.h"
#include "ptest.h"

extern var TestType;

struct TestType {
  int test_data;
};

static var TestType_New(var self, var args) {
  struct TestType* tt = self;
  tt->test_data = c_int(get(args, $(Int, 0)));
  return self;
}

static var TestType_Del(var self) {
  return self;
}

static size_t TestType_Size(void) {
  return sizeof(struct TestType);
}

static var TestType_Eq(var self, var obj) {
  struct TestType* lhs = cast(self, TestType);
  struct TestType* rhs = cast(obj, TestType);
  if (lhs->test_data == rhs->test_data) {
    return True;
  } else {
    return False;
  }
}

var TestType = typedecl(TestType,
  typeclass(New, TestType_New, TestType_Del, TestType_Size),
  typeclass(Eq, TestType_Eq));


struct ReturnTrue {
  var (*return_true)(var);
};

var ReturnTrue = typedecl(ReturnTrue);

static var return_true(var self) {
  return method(self, ReturnTrue, return_true);
}

static var IntParent_ReturnTrue(var self) {
  return True;
}

var IntParent = typedecl(IntParent,
  typeclass(ReturnTrue, IntParent_ReturnTrue));

PT_FUNC(test_type) {
  PT_ASSERT(type_of($(Int, 1)) is Int);
  PT_ASSERT(type_of($(Float, 1.0)) is Float);
  PT_ASSERT(type_of(True) is Bool);
  PT_ASSERT(type_of(False) is Bool);
  PT_ASSERT(type_of(Int) is Type);
  PT_ASSERT(type_of(Float) is Type);
  PT_ASSERT(type_of(Type) is Type);
}

PT_FUNC(test_cast) {
  
  var x = $(Int, 1);
  var y = $(Float, 2.0);

  x = cast(x, Int);  
  y = cast(y, Float);
  
  PT_ASSERT(x);
  PT_ASSERT(y);
  
}

PT_FUNC(test_new) {
  
  var x = new(Int, $(Int, 1));
  
  PT_ASSERT(x);
  PT_ASSERT(type_of(x) is Int);
  PT_ASSERT(c_int(x) is 1);
  
  del(x);
  
  var y = $(Float, 0.0);  
  construct(y, $(Float, 1.0));
  PT_ASSERT(c_float(y) is 1.0);
  
  var z = alloc(String);
  PT_ASSERT(z);
  construct(z, $(String, "Hello"));
  PT_ASSERT_STR_EQ(c_str(z), "Hello");
  z = destruct(z);
  dealloc(z);
  
}

PT_FUNC(test_assign) {
  
  /* Integers */
  
  var x = new(Int, $(Int, 10));
  var y = new(Int, $(Int, 20));

  PT_ASSERT(c_int(x) is 10);
  PT_ASSERT(c_int(y) is 20);
  PT_ASSERT(x isnt y);

  assign(x, y);
  
  PT_ASSERT(c_int(x) is 20);
  PT_ASSERT(c_int(y) is 20);
  PT_ASSERT(x isnt y);
  
  del(x);
  del(y);
  
  /* Strings */
  
  var xs = new(String, $(String, "Hello"));
  var ys = new(String, $(String, "There"));
  
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
  
  var x = new(String, $(String, "Hello"));
  var y = copy(x);
  
  PT_ASSERT_STR_EQ(c_str(x), c_str(y));
  PT_ASSERT(c_str(x) isnt c_str(y));
  PT_ASSERT(x isnt y);
  
  del(x);
  del(y);
  
}

PT_FUNC(test_eq) {
  PT_ASSERT(  eq($(Int, 1 ), $(Int, 1 )) );
  PT_ASSERT( neq($(Int, 2 ), $(Int, 20)) );
  PT_ASSERT(  eq($(String, "Hello"), $(String, "Hello")) );
  PT_ASSERT( neq($(String, "Hello"), $(String, "There")) );
  
  var tab1 = new(Table, String, Int);
  var tab2 = new(Table, String, Int);
  
  PT_ASSERT(eq(tab1, tab2));
  set(tab1, $(String, "apple"), $(Int, 10));
  PT_ASSERT(neq(tab1, tab2));
  set(tab2, $(String, "apple"), $(Int, 10));
  PT_ASSERT(eq(tab1, tab2));
  set(tab1, $(String, "pear"), $(Int, 20));
  set(tab2, $(String, "pear"), $(Int, 30));
  PT_ASSERT(neq(tab1, tab2));
  set(tab1, $(String, "pear"), $(Int, 30));
  PT_ASSERT(eq(tab1, tab2));
  set(tab2, $(String, "banana"), $(Int, 10));
  PT_ASSERT(neq(tab1, tab2));
  
  del(tab1);
  del(tab2);
  
  /*
  var map1 = new(Map, String, String);
  var map2 = new(Map, String, String);
  
  PT_ASSERT(eq(map1, map2));
  set(map1, $(String, "name"), $(String, "Alex"));
  PT_ASSERT(neq(map1, map2));
  set(map2, $(String, "name"), $(String, "Alex"));
  PT_ASSERT(eq(map1, map2));
  set(map1, $(String, "age"), $(String, "28"));
  set(map2, $(String, "age"), $(String, "30"));
  PT_ASSERT(neq(map1, map2));
  set(map1, $(String, "age"), $(String, "30"));
  PT_ASSERT(eq(map1, map2));
  set(map2, $(String, "nickname"), $(String, "The Wing Man"));
  PT_ASSERT(neq(map1, map2));
  
  del(map1);
  del(map2);
  */
  
}

PT_FUNC(test_ord) {
  PT_ASSERT(  gt($(Int, 15), $(Int, 3 )) );
  PT_ASSERT(  lt($(Int, 70), $(Int, 81)) );
  PT_ASSERT(  ge($(Int, 71), $(Int, 71)) );
  PT_ASSERT(  ge($(Int, 78), $(Int, 71)) );
  PT_ASSERT(  le($(Int, 32), $(Int, 32)) );
  PT_ASSERT(  le($(Int, 21), $(Int, 32)) );
}

PT_FUNC(test_hash) {
  uint64_t x = hash($(Int, 1  ));
  uint64_t y = hash($(Int, 123));
  
  PT_ASSERT(x is 1);
  PT_ASSERT(y is 123);
}

PT_FUNC(test_collection) {
  
  var x = new(Array, Int, $(Int, 1), $(Int, 2), $(Int, 3));
  
  PT_ASSERT(x);
  PT_ASSERT(len(x) is 3);
  PT_ASSERT(mem(x, $(Int, 1)));
  PT_ASSERT(mem(x, $(Int, 2)));
  PT_ASSERT(mem(x, $(Int, 3)));
  
  rem(x, $(Int, 2));
  
  PT_ASSERT(x);
  PT_ASSERT(len(x) is 2);
  PT_ASSERT(mem(x, $(Int, 1)));
  PT_ASSERT(mem(x, $(Int, 3)));
  
  clear(x);
  
  PT_ASSERT(x);
  PT_ASSERT(len(x) is 0);
  PT_ASSERT(empty(x));
  
  del(x);
  
  var y = new(Array, Float, 
    $(Float, 5.2), $(Float, 7.1), $(Float, 2.2), $(Float, 1.1));
  
  PT_ASSERT(y);
  PT_ASSERT(len(y) is 4);
  PT_ASSERT(mem(y, $(Float, 5.2)));
  PT_ASSERT(mem(y, $(Float, 7.1)));
  PT_ASSERT(mem(y, $(Float, 2.2)));
  PT_ASSERT(mem(y, $(Float, 1.1)));
  
  sort(y);
  
  PT_ASSERT(eq(get(y, $(Int, 0)), $(Float, 1.1)));
  PT_ASSERT(eq(get(y, $(Int, 1)), $(Float, 2.2)));
  PT_ASSERT(eq(get(y, $(Int, 2)), $(Float, 5.2)));
  PT_ASSERT(eq(get(y, $(Int, 3)), $(Float, 7.1)));
  
  /*
  var maxval = maximum(y);
  var minval = minimum(y);
  
  PT_ASSERT(eq(maxval, $(Float, 7.1)));
  PT_ASSERT(eq(minval, $(Float, 1.1)));
  */
  
  del(y);
  
  var z = new(Array, Float, 
    $(Float, 5.2), $(Float, 7.1), $(Float, 2.2), $(Float, 1.1));
  
  sort(z);
  
  PT_ASSERT(eq(get(z, $(Int, 0)), $(Float, 1.1)));
  PT_ASSERT(eq(get(z, $(Int, 1)), $(Float, 2.2)));
  PT_ASSERT(eq(get(z, $(Int, 2)), $(Float, 5.2)));
  PT_ASSERT(eq(get(z, $(Int, 3)), $(Float, 7.1)));
  
  del(z);
  
  var w = new(Array, Int, 
    $(Int, 135), $(Int, 11), $(Int, 254), 
    $(Int, 123213), $(Int, 22), $(Int, 1));
  
  sort(w);
  
  PT_ASSERT(eq(get(w, $(Int, 0)), $(Int, 1)));
  PT_ASSERT(eq(get(w, $(Int, 1)), $(Int, 11)));
  PT_ASSERT(eq(get(w, $(Int, 2)), $(Int, 22)));
  PT_ASSERT(eq(get(w, $(Int, 3)), $(Int, 135)));
  PT_ASSERT(eq(get(w, $(Int, 4)), $(Int, 254)));
  PT_ASSERT(eq(get(w, $(Int, 5)), $(Int, 123213)));
  
  del(w);
  
  /*
  var map1 = new(Map, String, String);
  var map2 = new(Map, String, String);
  
  PT_ASSERT(eq(map1, map2));
  
  set(map1, $(String, "key"), $(String, "val"));
  PT_ASSERT(neq(map1, map2));
  
  set(map2, $(String, "key"), $(String, "val"));
  PT_ASSERT(eq(map1, map2));
  
  set(map1, $(String, "newkey"), $(Int, 10));
  set(map2, $(String, "newkey"), $(String, "newval"));
  PT_ASSERT(neq(map1, map2));
  
  set(map1, $(String, "newkey"), $(String, "newval"));
  PT_ASSERT(eq(map1, map2));
  
  set(map2, $(String, "difkey"), $(Int, 5));
  PT_ASSERT(neq(map1, map2));
  
  del(map1);
  del(map2);
  */
  
}

PT_FUNC(test_iter) {

  var x = new(Array, Ref, $(Int, 1), $(Float, 2.0), $(String, "Hello"));
  
  foreach(y in x) {
    PT_ASSERT(y);
    PT_ASSERT(type_of(y));
  }
  
  del(x);
}

PT_FUNC(test_push) {
  
  var x = new(Array, Int);
  var y = new(Array, Ref);
  
  for(int i = 0; i < 1000; i++) {
    push(x, $(Int, 1));
    push(y, $(Int, 2));
  }
  
  for(int i = 0; i < 1000; i++) {
    pop(x);
    pop(y);
  }
  
  push(x, $(Int, 0));
  push(x, $(Int, 5));
  push(x, $(Int, 10));
  
  PT_ASSERT(eq(get(x, $(Int, 0)), $(Int, 0)));
  PT_ASSERT(eq(get(x, $(Int, 1)), $(Int, 5)));
  PT_ASSERT(eq(get(x, $(Int, 2)), $(Int, 10)));
  
  pop_at(x, $(Int, 1));
  
  PT_ASSERT(eq(get(x, $(Int, 0)), $(Int, 0)));
  PT_ASSERT(eq(get(x, $(Int, 1)), $(Int, 10)));
  
  del(x);
  del(y);
}

PT_FUNC(test_at) {

  var fst = $(Int, 1);
  var snd = $(Float, 2.0);
  var trd = $(String, "Hello");

  var x = new(Array, Ref, fst, snd, trd);

  PT_ASSERT(deref(get(x, $(Int, 0))) is fst);
  PT_ASSERT(deref(get(x, $(Int, 1))) is snd);
  PT_ASSERT(deref(get(x, $(Int, 2))) is trd);
  
  set(x, $(Int, 1), trd);
  
  PT_ASSERT(deref(get(x, $(Int, 1))) is trd);
  
  del(x);
  
}

PT_FUNC(test_dict) {

  var prices = new(Table, String, Int);
  set(prices, $(String, "Apple"),  $(Int, 12)); 
  set(prices, $(String, "Banana"), $(Int,  6)); 
  set(prices, $(String, "Pear"),   $(Int, 55)); 
  
  var pear_price = get(prices, $(String, "Pear"));
  var banana_price = get(prices, $(String, "Banana"));
  var apple_price = get(prices, $(String, "Apple"));
  
  PT_ASSERT(c_int(pear_price) is 55);
  PT_ASSERT(c_int(banana_price) is 6);
  PT_ASSERT(c_int(apple_price) is 12);
  
  char name[5];
  
  for(int i = 0; i < 10; i++) {
    sprintf(name, "%i", i);
    set(prices, $(String, name), $(Int, i));
  }
  
  for(int i = 0; i < 10; i++) {
    sprintf(name, "%i", i);
    rem(prices, $(String, name));
  }
  
  del(prices);
  
}

PT_FUNC(test_as_ctype) {
  
  PT_ASSERT(c_char($(Char, 'a')) is 'a');
  PT_ASSERT(c_char($(Char, 'b')) is 'b');
  
  PT_ASSERT_STR_EQ(c_str($(String, "Hello")), "Hello");
  PT_ASSERT_STR_EQ(c_str($(String, "There")), "There");
  
  PT_ASSERT(c_int($(Int, 5)) is 5);
  PT_ASSERT(c_int($(Float, 5.6)) is 5);
  PT_ASSERT(c_int($(Float, 5.5)) is 5);
  PT_ASSERT(c_int($(Float, 5.4)) is 5);
  
  PT_ASSERT(c_float($(Float, 5.1)) is 5.1);
  PT_ASSERT(c_float($(Float, 5.2)) is 5.2);
  PT_ASSERT(c_float($(Float, 9.8)) is 9.8);
  PT_ASSERT(c_float($(Int, 5)) is 5.0);
  PT_ASSERT(c_float($(Int, 7)) is 7.0);
 
}

PT_FUNC(test_stream) {

  var f = $(File, None);
  
  PT_ASSERT(f);
  
  sopen(f, $S("test.bin"), $S("w"));
    
    PT_ASSERT(f);
    
    //set(f, Int, $I(1));
    //set(f, Int, $I(22));
  
  sclose(f);
  
  sopen(f, $S("test.bin"), $S("r"));
    
    PT_ASSERT(f);
    
    //var first = get(f, Int);
    //var second = get(f, Int);
    
    //PT_ASSERT(c_int(first) is 1);
    //PT_ASSERT(c_int(second) is 22);
    
    //del(first);
    //del(second);
    
  sclose(f);
  
  PT_ASSERT(f);
}

PT_FUNC(test_type_new) {
 
  TestType = new(Type,
    $(String, "TestType"),
    $(New, TestType_New, TestType_Del, TestType_Size),
    $(Eq, TestType_Eq));
  
  PT_ASSERT(TestType);
  PT_ASSERT_STR_EQ(c_str(TestType), "TestType");

  var test_obj1 = new(TestType, $(Int, 1));
  var test_obj2 = new(TestType, $(Int, 1));
  var test_obj3 = new(TestType, $(Int, 4));

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

PT_FUNC(test_type_implements) {
  
  PT_ASSERT(type_implements(Int, New));
  PT_ASSERT(type_implements(Float, Math));
  PT_ASSERT(type_implements(String, Eq));
  
  PT_ASSERT(type_instance(Int, Ord));
  PT_ASSERT(type_instance(Array, Get));
  PT_ASSERT(type_instance(Type, C_Str));
  
}

PT_FUNC(test_type_parent) {
  
  /*
  PT_ASSERT(not type_implements(Int, ReturnTrue));
  PT_ASSERT(not type_implements(Float, ReturnTrue));
  PT_ASSERT(type_implements(IntParent, ReturnTrue));
  
  PT_ASSERT(return_true(IntParent));
  
  //Type_Inherit(Int, IntParent);
  
  PT_ASSERT(type_implements(Int, ReturnTrue));
  PT_ASSERT(not type_implements(Float, ReturnTrue));
  PT_ASSERT(type_implements(IntParent, ReturnTrue));
  
  PT_ASSERT(return_true(Int));
  */
  
}

PT_FUNC(test_show) {
  
  var out = new(String, $(String, ""));
  
  print_to(out, 0, "This is an %s %i %i",
    $(String, "example"), $(Int, 10), $(Int, 1));
  
  PT_ASSERT_STR_EQ(c_str(out), "This is an example 10 1");
  
  del(out);
  
}

PT_FUNC(test_look) {
  
  var x = $(Int, 0); 
  var y = $(Int, 0); 
  var z = $(Int, 0);
  var w = $(Int, 0);
  
  scan_from($(String, "5 10 1 0"), 0, "%i %i %i %i", x, y, z, w);
  
  PT_ASSERT(eq(x, $(Int, 5)));
  PT_ASSERT(eq(y, $(Int, 10)));
  PT_ASSERT(eq(z, $(Int, 1)));
  PT_ASSERT(eq(w, $(Int, 0)));
  
}

PT_SUITE(suite_core) {
  
  PT_REG(test_type);
  PT_REG(test_cast);
  PT_REG(test_new);
  PT_REG(test_assign);
  PT_REG(test_copy);
  PT_REG(test_eq);
  PT_REG(test_ord);
  PT_REG(test_hash);
  PT_REG(test_collection);
  PT_REG(test_iter);
  PT_REG(test_push);
  PT_REG(test_at);
  PT_REG(test_dict);
  PT_REG(test_as_ctype);
  PT_REG(test_stream);
  PT_REG(test_type_new);
  PT_REG(test_type_implements);
  PT_REG(test_type_parent);
  PT_REG(test_show);
  PT_REG(test_look);

}
