/* ex: set tabstop=2 shiftwidth=2 softtabstop=2 expandtab: */


#include <stdlib.h>
#include <stdio.h>

#include "C+.h"
#include "CUnit/Basic.h"

int init_cplus_test_suite(void) {
  return 0;
}

void test_list_static_allocation(void) {
  int integers = 0;
  int reals = 0;
  int chars = 0;
  // Stack allocation really it is quite flexible
  // Obviously this object cannot shrink and grow
  // But it can still be interated over etc
  var counting = $(List, 3, 3, (var[]){ $(Int, 1), $(Real, 2.0), $(Char, 'a') });

  foreach(counting, item) {
    // Types are rich objects too e.g overloaded as_str
    if (type_of(item) is Int) {
      integers++;
    } elif (type_of(item) is Real) {
      reals++;
    } elif(type_of(item) is Char) {
      chars++;
    }
  }

  CU_ASSERT(1 == integers);
  CU_ASSERT(1 == reals);
  CU_ASSERT(1 == chars);
}

void test_bool(void) {
  var _true = True;
  var _false = False;

  CU_ASSERT_STRING_EQUAL("True", as_str(_true));
  CU_ASSERT_STRING_EQUAL("Bool", as_str(type_of(_true)));
  CU_ASSERT_STRING_EQUAL("False", as_str(_false));
  CU_ASSERT_STRING_EQUAL("Bool", as_str(type_of(_false)));
}

void test_string_constructor(void) {
  // String allocated on heap using constructor
  var hello1 = new(String, "Hello");

  // String allocated on stack using literal
  var hello2 = $(String, "Hello");

  CU_ASSERT(5 == len(hello1));
  CU_ASSERT_STRING_EQUAL("Hello", as_str(hello1));

  // Equality function overloaded for actual string comparison
  CU_ASSERT(eq(hello1, hello2) is True)

  delete(hello1);
}

void test_list_constructor(void) {
  // Lists easy to create.
  var numbers = new(List, 3, $(Int, 1), $(Real, 10.12), $(Int, 6));

  CU_ASSERT(3 == len(numbers));

  CU_ASSERT(type_of(at(numbers, 0)) is Int);
  CU_ASSERT(type_of(at(numbers, 1)) is Real);
  CU_ASSERT(type_of(at(numbers, 2)) is Int);

  CU_ASSERT(1 == as_long(at(numbers, 0)));
  CU_ASSERT(10.12 == as_double(at(numbers, 1)));
  CU_ASSERT(6 == as_long(at(numbers, 2)));

  delete(numbers);
}

void test_list_foreach(void) {
  int integers = 0;
  int reals = 0;
  int total = 0;

  var numbers = new(List, 3, $(Int, 1), $(Real, 10.12), $(Int, 6));

  // Lists implement "Iter" so "foreach" can work on them.
  foreach(numbers, num) {
    // Types are data too.
    // "is" is just "==" i.e non overloaded equality.
    // Similar semantics to python "is"
    total++;
    if (type_of(num) is Int) {
      integers++;
    } elif (type_of(num) is Real) {
      reals++;
    }
  }

  CU_ASSERT(1 == reals);
  CU_ASSERT(2 == integers);
  CU_ASSERT(3 == total);

  delete(numbers);
}

void test_hashtable_constructor(void) {
  var prices = new(HashTable);

  put(prices, $(String, "Apple"),  $(Int, 12));
  put(prices, $(String, "Banana"), $(Real,  1.34));
  put(prices, $(String, "Pear"),   $(Int, 55));

  CU_ASSERT(55 == as_long(get(prices, $(String, "Pear"))));
  CU_ASSERT(12 == as_long(get(prices, $(String, "Apple"))));
  CU_ASSERT(1.34 == as_double(get(prices, $(String, "Banana"))));

  delete(prices);
}

void test_hashtable_foreach(void) {
  int integers = 0;
  int reals = 0;
  int total = 0;
  int error = 0;
  int not_contains = 0;

  var prices = new(HashTable);

  put(prices, $(String, "Apple"),  $(Int, 12));
  put(prices, $(String, "Banana"), $(Real,  1.34));
  put(prices, $(String, "Pear"),   $(Int, 55));

  foreach(prices, key) {
    var price = get(prices, key);
    total++;
    if (type_of(price) is Int) {
      integers++;
    } elif (type_of(price) is Real) {
      reals++;
    } else {
      error++;
    }

    if (not contains(key, $(String, "na")) ) {
      not_contains++;
    }
  }

  CU_ASSERT(1 == reals);
  CU_ASSERT(2 == integers);
  CU_ASSERT(3 == total);
  CU_ASSERT(0 == error);
  CU_ASSERT(2 == not_contains);

  delete(prices);

}

void test_list_push_pop(void) {
  var testlist = new(List, 0);
  var shadow_testlist = new(List, 0);

  for(int i = 0; i < 5; i++) {
    push(testlist, $(Int, i));
    push(shadow_testlist, $(Int, i));
  }
  for(int i = 0; i < 5; i++) {
    var item = pop(testlist);
    var shadow_item = pop(shadow_testlist);
    CU_ASSERT(as_long(item) == as_long(shadow_item));
  }

}

void test_array_push_pop(void) {
  var testarray = new(Array, Int, 0);
  var shadow_testarray = new(Array, Int, 0);

  for(int i = 0; i < 5; i++) {
    push(testarray, $(Int, i));
    push(shadow_testarray, $(Int, i));
  }
  
  for(int i = 0; i < 5; i++) {
    var item = at(testarray, 0);
    var shadow_item = at(shadow_testarray, 0);
    CU_ASSERT(as_long(item) == as_long(shadow_item));
    pop_front(testarray);
    pop_front(shadow_testarray);
  }
  
}

int main(int argc, char** argv) {

  CU_pSuite cplus_suite = NULL;

  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  cplus_suite = CU_add_suite("CPlus Suite", init_cplus_test_suite, NULL);

  if (NULL == cplus_suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((NULL == CU_add_test(cplus_suite, "test string constructor",
                          test_string_constructor)) ||
      (NULL == CU_add_test(cplus_suite, "test list constructor",
                          test_list_constructor)) ||
      (NULL == CU_add_test(cplus_suite, "test list foreach",
                          test_list_foreach)) ||
      (NULL == CU_add_test(cplus_suite, "test list push pop",
                          test_list_push_pop)) ||
      (NULL == CU_add_test(cplus_suite, "test list rich_objects",
                          test_list_static_allocation)) ||
      (NULL == CU_add_test(cplus_suite, "test array push pop",
                          test_array_push_pop)) ||
      (NULL == CU_add_test(cplus_suite, "test hashtable constructor",
                          test_hashtable_constructor)) ||
      (NULL == CU_add_test(cplus_suite, "test hashtable foreach",
                          test_hashtable_foreach)) ||
      (NULL == CU_add_test(cplus_suite, "test bool",
                          test_bool))
     ) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
