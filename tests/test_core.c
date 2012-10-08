/* ex: set tabstop=2 shiftwidth=2 softtabstop=2 expandtab: */

#include "test.h"

/* Yeah I know... */
#undef CU_ASSERT
#define CU_ASSERT(X) CU_TEST((bool)(X));

static void test_type(void) {
  CU_ASSERT(type_of($(Int, 1)) is Int);
  CU_ASSERT(type_of($(Real, 1.0)) is Real);
  CU_ASSERT(type_of(True) is Bool);
  CU_ASSERT(type_of(False) is Bool);
  CU_ASSERT(type_of(Int) is Type);
  CU_ASSERT(type_of(Real) is Type);
  CU_ASSERT(type_of(Type) is Type);
}

static void test_cast(void) {
  
  var x = $(Int, 1);
  var y = $(Real, 2.0);

  x = cast(x, Int);  
  y = cast(y, Real);
  
  CU_ASSERT(x);
  CU_ASSERT(y);
  
}

static void test_new(void) {
  
  var x = new(Int, 1);
  
  CU_ASSERT(x);
  CU_ASSERT(type_of(x) is Int);
  CU_ASSERT(as_long(x) is 1);
  
  delete(x);
  
  var y = $(Real, 0.0);  
  construct(y, 1.0);
  CU_ASSERT(as_double(y) is 1.0);
  
  var z = allocate(String);
  CU_ASSERT(z);
  construct(z, "Hello");
  CU_ASSERT_STRING_EQUAL(as_str(z), "Hello");
  z = destruct(z);
  deallocate(z);
  
}

static void test_assign(void) {
  
  /* Integers */
  
  var x = new(Int, 10);
  var y = new(Int, 20);
  
  CU_ASSERT(as_long(x) is 10);
  CU_ASSERT(as_long(y) is 20);
  CU_ASSERT(x != y);

  assign(x, y);
  
  CU_ASSERT(as_long(x) is 20);
  CU_ASSERT(as_long(y) is 20);
  CU_ASSERT(x != y);
  
  delete(x);
  delete(y);
  
  /* Strings */
  
  var xs = new(String, "Hello");
  var ys = new(String, "There");
  
  CU_ASSERT_STRING_EQUAL(as_str(xs), "Hello");
  CU_ASSERT_STRING_EQUAL(as_str(ys), "There");
  CU_ASSERT(as_str(xs) != as_str(ys));
  CU_ASSERT(xs != ys);

  assign(xs, ys);
  
  CU_ASSERT_STRING_EQUAL(as_str(xs), "There");
  CU_ASSERT_STRING_EQUAL(as_str(ys), "There");
  CU_ASSERT(as_str(xs) != as_str(ys));
  CU_ASSERT(xs != ys);
  
  delete(xs);
  delete(ys);
}

static void test_copy(void) {
  
  var x = new(String, "Hello");
  var y = copy(x);
  
  CU_ASSERT_STRING_EQUAL(as_str(x), as_str(y));
  CU_ASSERT(as_str(x) != as_str(y));
  CU_ASSERT(x != y);
  
  delete(x);
  delete(y);
  
}

static void test_eq(void) {
  CU_ASSERT(  eq($(Int, 1 ), $(Int, 1 )) );
  CU_ASSERT( neq($(Int, 2 ), $(Int, 20)) );
  CU_ASSERT(  eq($(String, "Hello"), $(String, "Hello")) );
  CU_ASSERT( neq($(String, "Hello"), $(String, "There")) ); 
}

static void test_ord(void) {
  CU_ASSERT(  gt($(Int, 15), $(Int, 3 )) );
  CU_ASSERT(  lt($(Int, 70), $(Int, 81)) );
  CU_ASSERT(  ge($(Int, 71), $(Int, 71)) );
  CU_ASSERT(  ge($(Int, 78), $(Int, 71)) );
  CU_ASSERT(  le($(Int, 32), $(Int, 32)) );
  CU_ASSERT(  le($(Int, 21), $(Int, 32)) );
}

static void test_hash(void) {
  /* TODO: Add hash testing for String and Real */
  long x = hash($(Int, 1  ));
  long y = hash($(Int, 123));
  
  CU_ASSERT(x is 1);
  CU_ASSERT(y is 123);
}

static void test_collection(void) {
  
  var x = new(List, 3, $(Int, 1), $(Real, 2.0), $(String, "Hello"));
  
  CU_ASSERT(x);
  CU_ASSERT(len(x) is 3);
  CU_ASSERT(contains(x, $(Int, 1)));
  CU_ASSERT(contains(x, $(Real, 2.0)));
  CU_ASSERT(contains(x, $(String, "Hello")));
  
  discard(x, $(Real, 2.0));
  
  CU_ASSERT(x);
  CU_ASSERT(len(x) is 2);
  CU_ASSERT(contains(x, $(Int, 1)));
  CU_ASSERT(contains(x, $(String, "Hello")));
  
  clear(x);
  
  CU_ASSERT(x);
  CU_ASSERT(len(x) is 0);
  CU_ASSERT(is_empty(x));
  
  delete(x);
  
  var y = new(Array, Real, 4, $(Real, 5.2), $(Real, 7.1), $(Real, 2.2), $(Real, 1.1));
  
  CU_ASSERT(y);
  CU_ASSERT(len(y) is 4);
  CU_ASSERT(contains(y, $(Real, 5.2)));
  CU_ASSERT(contains(y, $(Real, 7.1)));
  CU_ASSERT(contains(y, $(Real, 2.2)));
  CU_ASSERT(contains(y, $(Real, 1.1)));
  
  /* TODO: Get Sorting to work */
  /*
  sort(y);
  
  CU_ASSERT(eq(at(y, 0), $(Real, 1.1)));
  CU_ASSERT(eq(at(y, 1), $(Real, 2.2)));
  CU_ASSERT(eq(at(y, 2), $(Real, 5.2)));
  CU_ASSERT(eq(at(y, 3), $(Real, 7.1)));
  */
  
  var maxval = maximum(y);
  var minval = minimum(y);
  
  CU_ASSERT(eq(maxval, $(Real, 7.1)));
  CU_ASSERT(eq(minval, $(Real, 1.1)));
  
  delete(y);
  
}

static void test_iter(void) {

  var x = new(List, 3, $(Int, 1), $(Real, 2.0), $(String, "Hello"));
  
  foreach(x, y) {
    CU_ASSERT(y);
    CU_ASSERT(type_of(y));
  }
  
  delete(x);
}

static void test_push(void) {
  
  var x = new(Array, Int, 0);
  var y = new(List, 0);
  
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
  
  CU_ASSERT(eq(at(x, 0), $(Int, 0)));
  CU_ASSERT(eq(at(x, 1), $(Int, 5)));
  CU_ASSERT(eq(at(x, 2), $(Int, 10)));
  
  pop_at(x, 1);
  
  CU_ASSERT(eq(at(x, 0), $(Int, 0)));
  CU_ASSERT(eq(at(x, 1), $(Int, 10)));
  
  delete(x);
  delete(y);
}

static void test_at(void) {

  var fst = $(Int, 1);
  var snd = $(Real, 2.0);
  var trd = $(String, "Hello");

  var x = new(List, 3, fst, snd, trd);

  CU_ASSERT(at(x, 0) is fst);
  CU_ASSERT(at(x, 1) is snd);
  CU_ASSERT(at(x, 2) is trd);
  
  CU_ASSERT(at(x, -1) is None);
  CU_ASSERT(at(x, 3) is None);
  
  set(x, 1, trd);
  
  CU_ASSERT(at(x, 1) is trd);
  
  delete(x);
  
}

static void test_dict(void) {

  var prices = new(Table, String, Int);
  put(prices, $(String, "Apple"),  $(Int, 12)); 
  put(prices, $(String, "Banana"), $(Int,  6)); 
  put(prices, $(String, "Pear"),   $(Int, 55)); 
  
  var pear_price = get(prices, $(String, "Pear"));
  var banana_price = get(prices, $(String, "Banana"));
  var apple_price = get(prices, $(String, "Apple"));
  
  CU_ASSERT(as_long(pear_price) is 55);
  CU_ASSERT(as_long(banana_price) is 6);
  CU_ASSERT(as_long(apple_price) is 12);
  
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

static void test_as_ctype(void) {
  
  CU_ASSERT(as_char($(Char, 'a')) is 'a');
  CU_ASSERT(as_char($(Char, 'b')) is 'b');
  
  CU_ASSERT_STRING_EQUAL(as_str($(String, "Hello")), "Hello");
  CU_ASSERT_STRING_EQUAL(as_str($(String, "There")), "There");
  
  CU_ASSERT(as_long($(Int, 5)) is 5);
  CU_ASSERT(as_long($(Real, 5.6)) is 5);
  CU_ASSERT(as_long($(Real, 5.5)) is 5);
  CU_ASSERT(as_long($(Real, 5.4)) is 5);
  
  CU_ASSERT(as_double($(Real, 5.1)) is 5.1);
  CU_ASSERT(as_double($(Real, 5.2)) is 5.2);
  CU_ASSERT(as_double($(Real, 9.8)) is 9.8);
  CU_ASSERT(as_double($(Int, 5)) is 5.0);
  CU_ASSERT(as_double($(Int, 7)) is 7.0);
 
}

static void test_stream(void) {

  var f = $(File, NULL);
  
  CU_ASSERT(f);
  
  open(f, "test.bin", "w");
    
    CU_ASSERT(f);
    
    put(f, String, $(String, "Hello"));
    put(f, Int, $(Int, 1));
    put(f, Int, $(Int, 22));
  
  close(f);
  
  open(f, "test.bin", "r");
    
    CU_ASSERT(f);
    
    var string = get(f, String);
    var first = get(f, Int);
    var second = get(f, Int);
    
    CU_ASSERT_STRING_EQUAL(as_str(string), "Hello");
    CU_ASSERT(as_long(first) is 1);
    CU_ASSERT(as_long(second) is 22);
    
    delete(string);
    delete(first);
    delete(second);
    
  close(f);
  
  CU_ASSERT(f);
}

static var TestType;

data {
  var type;
  int test_data;
} TestTypeData;

static var TestType_New(var self, va_list* args) {
  TestTypeData* ttd = cast(self, TestType);
  ttd->test_data = va_arg(*args, int);
  return self;
}

static var TestType_Delete(var self) {
  return self;
}

static var TestType_Eq(var self, var obj) {
  TestTypeData* lhs = cast(self, TestType);
  TestTypeData* rhs = cast(obj, TestType);
  if (lhs->test_data == rhs->test_data) {
    return True;
  } else {
    return False;
  }
}

instance(TestType, New) = { sizeof(TestTypeData), TestType_New, TestType_Delete };
instance(TestType, Eq) = { TestType_Eq };

static void test_type_new(void) {
 
  TestType = new(Type, "TestType", 2, 
    (var[]){ &TestTypeNew, &TestTypeEq }, 
    (const char*[]){ "New", "Eq" } );
  
  CU_ASSERT(TestType);
  CU_ASSERT_STRING_EQUAL(as_str(TestType), "TestType");

  var test_obj1 = new(TestType, 1);
  var test_obj2 = new(TestType, 1);
  var test_obj3 = new(TestType, 4);

  CU_ASSERT(test_obj1);
  CU_ASSERT(test_obj2);
  CU_ASSERT(test_obj3);
  
  CU_ASSERT(eq(test_obj1, test_obj2));
  CU_ASSERT(neq(test_obj1, test_obj3));
  
  delete(test_obj1);
  delete(test_obj2);
  delete(test_obj3);
  
  delete(TestType);
  
}

void test_type_implements(void) {
  
  CU_ASSERT(type_implements(Int, New));
  CU_ASSERT(type_implements(Real, Num));
  CU_ASSERT(type_implements(String, Eq));
  
  CU_ASSERT(type_class(Int, Ord));
  CU_ASSERT(type_class(List, At));
  CU_ASSERT(type_class(Type, AsStr));
  
}

int init_core_suite(void) {
  return 0;
}

int build_core_suite(CU_pSuite suite) {

  if (suite is None) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((CU_add_test(suite, "Types", test_type)            is None) or
      (CU_add_test(suite, "Casting", test_cast)          is None) or
      (CU_add_test(suite, "New/Delete", test_new)        is None) or
      (CU_add_test(suite, "Assign", test_assign)         is None) or
      (CU_add_test(suite, "Copy", test_copy)             is None) or
      (CU_add_test(suite, "Equality", test_eq)           is None) or
      (CU_add_test(suite, "Ordering", test_ord)          is None) or
      (CU_add_test(suite, "Hashing", test_hash)          is None) or
      (CU_add_test(suite, "Collection", test_collection) is None) or
      (CU_add_test(suite, "Iteration", test_iter)        is None) or
      (CU_add_test(suite, "Push/Pop", test_push)         is None) or
      (CU_add_test(suite, "At/Set", test_at)             is None) or
      (CU_add_test(suite, "Get/Put", test_dict)          is None) or
      (CU_add_test(suite, "As CTypes", test_as_ctype)    is None) or
      (CU_add_test(suite, "Stream/Parse", test_stream)   is None) or
      (CU_add_test(suite, "New Type", test_type_new)     is None) or
      (CU_add_test(suite, "Type Implements", test_type_implements) is None))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  return 0;
}