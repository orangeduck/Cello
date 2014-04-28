
#include "ptest.h"
#include "Cello.h"

local var TestType;

data {
  var type;
  int test_data;
} TestTypeData;

local var TestType_New(var self, var_list vl) {
  TestTypeData* ttd = cast(self, TestType);
  ttd->test_data = as_long(var_list_get(vl));
  return self;
}

local var TestType_Delete(var self) {
  return self;
}

local size_t TestType_Size(void) {
  return sizeof(TestTypeData);
}

local var TestType_Eq(var self, var obj) {
  TestTypeData* lhs = cast(self, TestType);
  TestTypeData* rhs = cast(obj, TestType);
  if (lhs->test_data == rhs->test_data) {
    return True;
  } else {
    return False;
  }
}

instance(TestType, New) = { TestType_New, TestType_Delete, TestType_Size };
instance(TestType, Eq) = { TestType_Eq };

class {
  var (*return_true)(var);
} ReturnTrue;

local var return_true(var self) {
  return type_class_method(self, ReturnTrue, return_true, self);
}

local var IntParent_ReturnTrue(var self) {
  return True;
}

instance(IntParent, ReturnTrue) = { IntParent_ReturnTrue };

var IntParent = type_data {
  type_begin(IntParent),
  type_entry(IntParent, ReturnTrue),
  type_end(IntParent),
};

PT_FUNC(test_type) {
  PT_ASSERT(type_of($(Int, 1)) is Int);
  PT_ASSERT(type_of($(Real, 1.0)) is Real);
  PT_ASSERT(type_of(True) is Bool);
  PT_ASSERT(type_of(False) is Bool);
  PT_ASSERT(type_of(Int) is Type);
  PT_ASSERT(type_of(Real) is Type);
  PT_ASSERT(type_of(Type) is Type);
}

PT_FUNC(test_cast) {
  
  var x = $(Int, 1);
  var y = $(Real, 2.0);

  x = cast(x, Int);  
  y = cast(y, Real);
  
  PT_ASSERT(x);
  PT_ASSERT(y);
  
}

PT_FUNC(test_new) {
  
  var x = new(Int, $(Int, 1));
  
  PT_ASSERT(x);
  PT_ASSERT(type_of(x) is Int);
  PT_ASSERT(as_long(x) is 1);
  
  delete(x);
  
  var y = $(Real, 0.0);  
  construct(y, $(Real, 1.0));
  PT_ASSERT(as_double(y) is 1.0);
  
  var z = allocate(String);
  PT_ASSERT(z);
  construct(z, $(String, "Hello"));
  PT_ASSERT_STR_EQ(as_str(z), "Hello");
  z = destruct(z);
  deallocate(z);
  
}

PT_FUNC(test_assign) {
  
  /* Integers */
  
  var x = new(Int, $(Int, 10));
  var y = new(Int, $(Int, 20));

  PT_ASSERT(as_long(x) is 10);
  PT_ASSERT(as_long(y) is 20);
  PT_ASSERT(x isnt y);

  assign(x, y);
  
  PT_ASSERT(as_long(x) is 20);
  PT_ASSERT(as_long(y) is 20);
  PT_ASSERT(x isnt y);
  
  delete(x);
  delete(y);
  
  /* Strings */
  
  var xs = new(String, $(String, "Hello"));
  var ys = new(String, $(String, "There"));
  
  PT_ASSERT_STR_EQ(as_str(xs), "Hello");
  PT_ASSERT_STR_EQ(as_str(ys), "There");
  PT_ASSERT(as_str(xs) isnt as_str(ys));
  PT_ASSERT(xs isnt ys);

  assign(xs, ys);
  
  PT_ASSERT_STR_EQ(as_str(xs), "There");
  PT_ASSERT_STR_EQ(as_str(ys), "There");
  PT_ASSERT(as_str(xs) isnt as_str(ys));
  PT_ASSERT(xs isnt ys);
  
  delete(xs);
  delete(ys);
}

PT_FUNC(test_copy) {
  
  var x = new(String, $(String, "Hello"));
  var y = copy(x);
  
  PT_ASSERT_STR_EQ(as_str(x), as_str(y));
  PT_ASSERT(as_str(x) isnt as_str(y));
  PT_ASSERT(x isnt y);
  
  delete(x);
  delete(y);
  
}

PT_FUNC(test_eq) {
  PT_ASSERT(  eq($(Int, 1 ), $(Int, 1 )) );
  PT_ASSERT( neq($(Int, 2 ), $(Int, 20)) );
  PT_ASSERT(  eq($(String, "Hello"), $(String, "Hello")) );
  PT_ASSERT( neq($(String, "Hello"), $(String, "There")) );
  
  var tab1 = new(Table, String, Int);
  var tab2 = new(Table, String, Int);
  
  PT_ASSERT(eq(tab1, tab2));
  put(tab1, $(String, "apple"), $(Int, 10));
  PT_ASSERT(neq(tab1, tab2));
  put(tab2, $(String, "apple"), $(Int, 10));
  PT_ASSERT(eq(tab1, tab2));
  put(tab1, $(String, "pear"), $(Int, 20));
  put(tab2, $(String, "pear"), $(Int, 30));
  PT_ASSERT(neq(tab1, tab2));
  put(tab1, $(String, "pear"), $(Int, 30));
  PT_ASSERT(eq(tab1, tab2));
  put(tab2, $(String, "banana"), $(Int, 10));
  PT_ASSERT(neq(tab1, tab2));
  
  delete(tab1);
  delete(tab2);

  var dict1 = new(Dictionary, 0);
  var dict2 = new(Dictionary, 0);
  
  PT_ASSERT(eq(dict1, dict2));
  put(dict1, $(String, "apple"), $(Int, 10));
  PT_ASSERT(neq(dict1, dict2));
  put(dict2, $(String, "apple"), $(Int, 10));
  PT_ASSERT(eq(dict1, dict2));
  put(dict1, $(String, "pear"), $(Int, 20));
  put(dict2, $(String, "pear"), $(Int, 30));
  PT_ASSERT(neq(dict1, dict2));
  put(dict1, $(String, "pear"), $(Int, 30));
  PT_ASSERT(eq(dict1, dict2));
  put(dict2, $(String, "banana"), $(Int, 10));
  PT_ASSERT(neq(dict1, dict2));
  
  delete(dict1);
  delete(dict2);
  
  var tree1 = new(Tree, String, String);
  var tree2 = new(Tree, String, String);
  
  PT_ASSERT(eq(tree1, tree2));
  put(tree1, $(String, "name"), $(String, "Alex"));
  PT_ASSERT(neq(tree1, tree2));
  put(tree2, $(String, "name"), $(String, "Alex"));
  PT_ASSERT(eq(tree1, tree2));
  put(tree1, $(String, "age"), $(String, "28"));
  put(tree2, $(String, "age"), $(String, "30"));
  PT_ASSERT(neq(tree1, tree2));
  put(tree1, $(String, "age"), $(String, "30"));
  PT_ASSERT(eq(tree1, tree2));
  put(tree2, $(String, "nickname"), $(String, "The Wing Man"));
  PT_ASSERT(neq(tree1, tree2));
  
  delete(tree1);
  delete(tree2);
  
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
  long x = hash($(Int, 1  ));
  long y = hash($(Int, 123));
  
  PT_ASSERT(x is 1);
  PT_ASSERT(y is 123);
}

PT_FUNC(test_collection) {
  
  var x = new(List, $(Int, 1), $(Real, 2.0), $(String, "Hello"));
  
  PT_ASSERT(x);
  PT_ASSERT(len(x) is 3);
  PT_ASSERT(contains(x, $(Int, 1)));
  PT_ASSERT(contains(x, $(Real, 2.0)));
  PT_ASSERT(contains(x, $(String, "Hello")));
  
  discard(x, $(Real, 2.0));
  
  PT_ASSERT(x);
  PT_ASSERT(len(x) is 2);
  PT_ASSERT(contains(x, $(Int, 1)));
  PT_ASSERT(contains(x, $(String, "Hello")));
  
  clear(x);
  
  PT_ASSERT(x);
  PT_ASSERT(len(x) is 0);
  PT_ASSERT(is_empty(x));
  
  delete(x);
  
  var y = new(Array, Real, $(Real, 5.2), $(Real, 7.1), $(Real, 2.2), $(Real, 1.1));
  
  PT_ASSERT(y);
  PT_ASSERT(len(y) is 4);
  PT_ASSERT(contains(y, $(Real, 5.2)));
  PT_ASSERT(contains(y, $(Real, 7.1)));
  PT_ASSERT(contains(y, $(Real, 2.2)));
  PT_ASSERT(contains(y, $(Real, 1.1)));
  
  sort(y);
  
  PT_ASSERT(eq(at(y, 0), $(Real, 1.1)));
  PT_ASSERT(eq(at(y, 1), $(Real, 2.2)));
  PT_ASSERT(eq(at(y, 2), $(Real, 5.2)));
  PT_ASSERT(eq(at(y, 3), $(Real, 7.1)));
  
  var maxval = maximum(y);
  var minval = minimum(y);
  
  PT_ASSERT(eq(maxval, $(Real, 7.1)));
  PT_ASSERT(eq(minval, $(Real, 1.1)));
  
  delete(y);
  
  var z = new(List, $(Real, 5.2), $(Real, 7.1), $(Real, 2.2), $(Real, 1.1));
  
  sort(z);
  
  PT_ASSERT(eq(at(z, 0), $(Real, 1.1)));
  PT_ASSERT(eq(at(z, 1), $(Real, 2.2)));
  PT_ASSERT(eq(at(z, 2), $(Real, 5.2)));
  PT_ASSERT(eq(at(z, 3), $(Real, 7.1)));
  
  delete(z);
  
  var w = new(List, $(Int, 135), $(Int, 11), $(Int, 254), $(Int, 123213), $(Int, 22), $(Int, 1));
  
  sort(w);
  
  PT_ASSERT(eq(at(w, 0), $(Int, 1)));
  PT_ASSERT(eq(at(w, 1), $(Int, 11)));
  PT_ASSERT(eq(at(w, 2), $(Int, 22)));
  PT_ASSERT(eq(at(w, 3), $(Int, 135)));
  PT_ASSERT(eq(at(w, 4), $(Int, 254)));
  PT_ASSERT(eq(at(w, 5), $(Int, 123213)));
  
  delete(w);
  
  var map1 = new(Map);
  var map2 = new(Map);
  
  PT_ASSERT(eq(map1, map2));
  
  put(map1, $(String, "key"), $(String, "val"));
  PT_ASSERT(neq(map1, map2));
  
  put(map2, $(String, "key"), $(String, "val"));
  PT_ASSERT(eq(map1, map2));
  
  put(map1, $(String, "newkey"), $(Int, 10));
  put(map2, $(String, "newkey"), $(String, "newval"));
  PT_ASSERT(neq(map1, map2));
  
  put(map1, $(String, "newkey"), $(String, "newval"));
  PT_ASSERT(eq(map1, map2));
  
  put(map2, $(String, "difkey"), $(Int, 5));
  PT_ASSERT(neq(map1, map2));
  
  delete(map1);
  delete(map2);
  
}

PT_FUNC(test_iter) {

  var x = new(List, $(Int, 1), $(Real, 2.0), $(String, "Hello"));
  
  foreach(y in x) {
    PT_ASSERT(y);
    PT_ASSERT(type_of(y));
  }
  
  delete(x);
}

PT_FUNC(test_push) {
  
  var x = new(Array, Int);
  var y = new(List);
  
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
  
  PT_ASSERT(eq(at(x, 0), $(Int, 0)));
  PT_ASSERT(eq(at(x, 1), $(Int, 5)));
  PT_ASSERT(eq(at(x, 2), $(Int, 10)));
  
  pop_at(x, 1);
  
  PT_ASSERT(eq(at(x, 0), $(Int, 0)));
  PT_ASSERT(eq(at(x, 1), $(Int, 10)));
  
  delete(x);
  delete(y);
}

PT_FUNC(test_at) {

  var fst = $(Int, 1);
  var snd = $(Real, 2.0);
  var trd = $(String, "Hello");

  var x = new(List, fst, snd, trd);

  PT_ASSERT(at(x, 0) is fst);
  PT_ASSERT(at(x, 1) is snd);
  PT_ASSERT(at(x, 2) is trd);
  
  set(x, 1, trd);
  
  PT_ASSERT(at(x, 1) is trd);
  
  delete(x);
  
}

PT_FUNC(test_dict) {

  var prices = new(Table, String, Int);
  put(prices, $(String, "Apple"),  $(Int, 12)); 
  put(prices, $(String, "Banana"), $(Int,  6)); 
  put(prices, $(String, "Pear"),   $(Int, 55)); 
  
  var pear_price = get(prices, $(String, "Pear"));
  var banana_price = get(prices, $(String, "Banana"));
  var apple_price = get(prices, $(String, "Apple"));
  
  PT_ASSERT(as_long(pear_price) is 55);
  PT_ASSERT(as_long(banana_price) is 6);
  PT_ASSERT(as_long(apple_price) is 12);
  
  char name[5];
  
  for(int i = 0; i < 1000; i++) {
    sprintf(name, "%i", i);
    put(prices, $(String, name), $(Int, i));
  }
  
  for(int i = 0; i < 1000; i++) {
    sprintf(name, "%i", i);
    discard(prices, $(String, name));
  }
  
  delete(prices);
  
}

PT_FUNC(test_as_ctype) {
  
  PT_ASSERT(as_char($(Char, 'a')) is 'a');
  PT_ASSERT(as_char($(Char, 'b')) is 'b');
  
  PT_ASSERT_STR_EQ(as_str($(String, "Hello")), "Hello");
  PT_ASSERT_STR_EQ(as_str($(String, "There")), "There");
  
  PT_ASSERT(as_long($(Int, 5)) is 5);
  PT_ASSERT(as_long($(Real, 5.6)) is 5);
  PT_ASSERT(as_long($(Real, 5.5)) is 5);
  PT_ASSERT(as_long($(Real, 5.4)) is 5);
  
  PT_ASSERT(as_double($(Real, 5.1)) is 5.1);
  PT_ASSERT(as_double($(Real, 5.2)) is 5.2);
  PT_ASSERT(as_double($(Real, 9.8)) is 9.8);
  PT_ASSERT(as_double($(Int, 5)) is 5.0);
  PT_ASSERT(as_double($(Int, 7)) is 7.0);
 
}

PT_FUNC(test_stream) {

  var f = $(File, NULL);
  
  PT_ASSERT(f);
  
  stream_open(f, "test.bin", "w");
    
    PT_ASSERT(f);
    
    put(f, Int, $(Int, 1));
    put(f, Int, $(Int, 22));
  
  stream_close(f);
  
  stream_open(f, "test.bin", "r");
    
    PT_ASSERT(f);
    
    var first = get(f, Int);
    var second = get(f, Int);
    
    PT_ASSERT(as_long(first) is 1);
    PT_ASSERT(as_long(second) is 22);
    
    delete(first);
    delete(second);
    
  stream_close(f);
  
  PT_ASSERT(f);
}

PT_FUNC(test_type_new) {
 
  TestType = new(Type, $(String, "TestType"), $(Int, 2), 
    (var[]){ &TestTypeNew, &TestTypeEq }, 
    (const char*[]){ "New", "Eq" } );
  
  PT_ASSERT(TestType);
  PT_ASSERT_STR_EQ(as_str(TestType), "TestType");

  var test_obj1 = new(TestType, $(Int, 1));
  var test_obj2 = new(TestType, $(Int, 1));
  var test_obj3 = new(TestType, $(Int, 4));

  PT_ASSERT(test_obj1);
  PT_ASSERT(test_obj2);
  PT_ASSERT(test_obj3);
  
  PT_ASSERT(eq(test_obj1, test_obj2));
  PT_ASSERT(neq(test_obj1, test_obj3));
  
  delete(test_obj1);
  delete(test_obj2);
  delete(test_obj3);
  
  delete(TestType);
  
}

PT_FUNC(test_type_implements) {
  
  PT_ASSERT(type_implements(Int, New));
  PT_ASSERT(type_implements(Real, Num));
  PT_ASSERT(type_implements(String, Eq));
  
  PT_ASSERT(type_class(Int, Ord));
  PT_ASSERT(type_class(List, At));
  PT_ASSERT(type_class(Type, AsStr));
  
}

PT_FUNC(test_type_parent) {
  
  PT_ASSERT(not type_implements(Int, ReturnTrue));
  PT_ASSERT(not type_implements(Real, ReturnTrue));
  PT_ASSERT(type_implements(IntParent, ReturnTrue));
  
  PT_ASSERT(return_true(IntParent));
  
  Type_Inherit(Int, IntParent);
  
  PT_ASSERT(type_implements(Int, ReturnTrue));
  PT_ASSERT(not type_implements(Real, ReturnTrue));
  PT_ASSERT(type_implements(IntParent, ReturnTrue));
  
  PT_ASSERT(return_true(Int));
  
}

PT_FUNC(test_show) {
  
  var out = new(String, $(String, ""));
  
  print_to(out, 0, "This is an %s %i %i",
    $(String, "example"), $(Int, 10), $(Int, 1));
  
  PT_ASSERT_STR_EQ(as_str(out), "This is an example 10 1");
  
  delete(out);
  
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

PT_FUNC(test_module) {  
      
#ifdef _WIN32
  with(python in module("python27.dll")) {
  
    const char* (*Py_GetVersion)(void) = get(python, $(String, "Py_GetVersion"));
    const char* (*Py_GetPlatform)(void) = get(python, $(String, "Py_GetPlatform"));
    const char* (*Py_GetCopyright)(void) = get(python, $(String, "Py_GetCopyright"));
  
    //println("");
    //println("Python Version is '%s'", $(String, (char*)Py_GetVersion()));
    //println("Python Platform is '%s'", $(String, (char*)Py_GetPlatform()));
    //println("Python Copyright is '%s'", $(String, (char*)Py_GetCopyright()));
    
    PT_ASSERT(Py_GetVersion());
    PT_ASSERT(Py_GetPlatform());
    PT_ASSERT(Py_GetCopyright());
    
  }
#else
  //with(math in module("libm.so")) {
    
    //double (*cosine)(double) = get(math, $(String, "cos"));
    
    //PT_ASSERT(cosine);
    
  //}
#endif
  
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
  PT_REG(test_module);

}
