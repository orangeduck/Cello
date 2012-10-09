#include "test.h"

static var empty_function(var args) {
  return None;
}

static void test_stack_function(void) {
  var f = $(Function, empty_function);
  CU_ASSERT(f);
}

static void test_heap_function(void) {
  var f = new(Function, empty_function);
  CU_ASSERT(f);
  delete(f);
}

static var empty_function2(var args) {
  return Some;
}

static void test_function_assign(void) {
  
  var f1 = new(Function, empty_function);
  var f2 = new(Function, empty_function2);
  
  CU_ASSERT(((FunctionData*)f1)->func != ((FunctionData*)f2)->func);
  
  assign(f1, f2);
  
  CU_ASSERT(((FunctionData*)f1)->func == ((FunctionData*)f2)->func);
  
  delete(f1);
  delete(f2);
  
}

static void test_function_copy(void) {

  var f1 = new(Function, empty_function);
  var f2 = copy(f1);
  
  CU_ASSERT(((FunctionData*)f1)->func is ((FunctionData*)f2)->func);
  
  delete(f1);
  delete(f2);

}

static void test_call(void) {
  
  var result1 = call($(Function, empty_function), None);
  var result2 = call($(Function, empty_function2), None);
  
  CU_ASSERT(result1 is None);
  CU_ASSERT(result2 is Some);
  
}

static var asserts_args(var args) {
  CU_ASSERT(at(args,0));
  CU_ASSERT(at(args,1));
  return None;
}

static void test_call_with(void) {
  
  var args = new(List, 2, $(Int, 1), $(Int, 5));
  
  var assert_func = $(Function, asserts_args);
  
  var result = call_with(assert_func, args);
  
  CU_ASSERT(result is None);
  
  delete(args);
  
}

static void test_call_with_ptr(void) {

  var* args = (var[]){ $(Int, 1), $(Int, 5), (var)-1 };
  
  var assert_func = $(Function, asserts_args);
  
  var result = call_with_ptr(assert_func, args);
  
  CU_ASSERT(result is None);

}

static void test_lambda(void) {

  var out = new(String, "");
  
  lambda(hello_name, args) {
    var name = cast(at(args, 0), String);
    var out = cast(at(args, 1), String);
    assign(out, name);
    return None;
  }
  
  call(hello_name, $(String, "Hello Bob!"), out);
  
  CU_ASSERT( eq(out, $(String, "Hello Bob!")) );
  
  delete(out);
}

static void test_lambda_id(void) {
  
  lambda(return_new_int1, args) {
    return new(Int, 1);
  }
  
  lambda_id(return_new_int2, return_new_int1);
  
  var res1 = call(return_new_int1, None);
  var res2 = call(return_new_int2, None);
  
  CU_ASSERT(as_long(res1) is 1);
  CU_ASSERT(as_long(res2) is 1);
  CU_ASSERT(as_long(res1) == as_long(res2));
  
  delete(res1);
  delete(res2);
  
}

static void test_lambda_const(void) {
  
  lambda_const(return_some, Some);
  
  var res = call(return_some, None);
  
  CU_ASSERT(res is Some);
  
}

static void test_lambda_compose(void) {
  
  lambda(return_some, args) {
    return new(List, 1, Some);
  }
  
  lambda(return_arg0, args) {
    var fst = at(args, 0);
    delete(args);
    return fst;
  }
  
  lambda_compose(return_compose, return_arg0, return_some);
  
  var res = call(return_compose, None);
  
  CU_ASSERT(res is Some);
  
}

static void test_lambda_flip(void) {
  
  lambda(return_first, args) {
    return at(args, 0);
  }
  
  lambda_flip(return_first_flip, return_first);
  
  var arg1 = $(Int, 5);
  var arg2 = $(Int, 10);
  
  var res1 = call(return_first, arg1, arg2);
  var res2 = call(return_first_flip, arg1, arg2);
  
  CU_ASSERT(res1 is arg1);
  CU_ASSERT(res2 is arg2);
  
}

static void test_lambda_pipe(void) {
  
  var total = $(Int, 0);
  
  lambda(add_one, args) {
    add( at(args,0) , $(Int, 1) );
    return args;
  }
  
  lambda(add_ten, args) {
    add( at(args,0) , $(Int, 10) );
    return args;
  }
  
  lambda(add_hundred, args) {
    add( at(args,0) , $(Int, 100) );
    return args;
  }
  
  lambda_pipe(add_all, add_one, add_ten, add_hundred);
  
  var res = call(add_all, total);
  
  CU_ASSERT(as_long(total) is 111);
  
}

static void test_lambda_method_pipe(void) {
  
  var str = new(String, "");
  
  lambda(cat_fizz, args) {
    String_Concat(at(args, 0), $(String, "Fizz"));
    return None;
  }
  
  lambda(cat_buzz, args) {
    String_Concat(at(args, 0), $(String, "Buzz"));
    return None;
  }
  
  lambda(cat_boo, args) {
    String_Concat(at(args, 0), $(String, "Boo"));
    return None;
  }
  
  lambda_method_pipe(cat_all, cat_fizz, cat_buzz, cat_boo);
  
  var res = call(cat_all, str);
  
  CU_ASSERT_STRING_EQUAL(as_str(str), "FizzBuzzBoo");
  
  delete(str);
  
}

static void test_lambda_partial_l(void) {
  
  lambda(add_to_first, args) {
    add(at(args,0), at(args,1));
    add(at(args,0), at(args,2));
    add(at(args,0), at(args,3));
    return None;
  }
  
  var total = $(Int, 0);
  
  lambda_partial(add_to_total, add_to_first, total);
  
  var res = call(add_to_total, $(Int, 1), $(Int, 10), $(Int, 100));
  
  CU_ASSERT(as_long(total) is 111);
}

static void test_lambda_partial_r(void) {

  lambda(add_to_last, args) {
    add(at(args,3), at(args,0));
    add(at(args,3), at(args,1));
    add(at(args,3), at(args,2));
    return None;
  }
  
  var total = $(Int, 0);
  
  lambda_partial_r(add_to_total, add_to_last, total);
  
  var res = call(add_to_total, $(Int, 1), $(Int, 10), $(Int, 100));
  
  CU_ASSERT(as_long(total) is 111);

}

static var return_snd(var fst, var snd) {
  return snd;
}

static void test_lambda_uncurry(void) {
  
  lambda_uncurry(return_snd_uncurried, return_snd, 2);
  
  var res = call(return_snd_uncurried, None, Some);
  
  CU_ASSERT(res is Some);
  
}

static void snd_to_fst(var fst, var snd) {
  assign(fst, snd);
}

static void test_lambda_void_uncurry(void) {

  lambda_void_uncurry(snd_to_fst_uncurried, snd_to_fst, 2);
  
  var fst = $(Int, 0);
  var snd = $(Int, 64);
  
  var res = call(snd_to_fst_uncurried, fst, snd);
  
  CU_ASSERT(as_long(fst) is 64);
  
}

static void test_map(void) {
  
  lambda(add_one, args) {
    add(at(args, 0), $(Int, 1));
    return None;
  }
  
  var values = new(List, 3, $(Int, 5), $(Int, 3), $(Int, 10));
  
  map(values, add_one);
  
  CU_ASSERT(as_long(at(values, 0)) is 6);
  CU_ASSERT(as_long(at(values, 1)) is 4);
  CU_ASSERT(as_long(at(values, 2)) is 11);
  
  delete(values);
  
}

static void test_new_map(void) {
  
  lambda(copy_values, args) {
    return at(args, 0);
  }
  
  var values = new(List, 3, $(Int, 5), $(Int, 3), $(Int, 10));
  
  var new_values = new_map(values, copy_values);
  
  CU_ASSERT(new_values);
  CU_ASSERT(at(values,0) is at(new_values,0));
  
  delete(values);
  delete(new_values);

}

static void test_new_filter(void) {
  
  lambda(only_some, args) {
    return (var)(at(args,0) is Some);
  }
  
  var values = new(List, 3, Some, Some, None);
  var somes = new_filter(values, only_some);
  
  CU_ASSERT(len(somes) is 2);
  
  delete(values);
  delete(somes);
  
}

static void test_new_sum(void) {

  var values = new(List, 3, $(Int, 5), $(Int, 3), $(Int, 10));
  var total = new_sum(values);

  CU_ASSERT(as_long(total) is 18);
  
  delete(total);
  delete(values);
  
}

static void test_new_product(void) {

  var values = new(List, 3, $(Int, 5), $(Int, 3), $(Int, 10));
  var total = new_product(values);
  
  CU_ASSERT(as_long(total) is 150);
  
  delete(total);
  delete(values);

}

static void test_new_foldl(void) {
  
  lambda(fold_sum, args) {
    var base = at(args, 0);
    var item = at(args, 1);
    add(base, item);
    return None;
  }
  
  var values = new(List, 3, $(Int, 5), $(Int, 3), $(Int, 10));
  var total = new_foldl(values, fold_sum, $(Int, 0));

  CU_ASSERT(as_long(total) is 18);
  
  delete(total);
  delete(values);
  
}

static void test_new_foldr(void) {

  lambda(fold_sum, args) {
    var base = at(args, 0);
    var item = at(args, 1);
    add(base, item);
    return None;
  }
  
  var values = new(List, 3, $(Int, 5), $(Int, 3), $(Int, 10));
  var total = new_foldr(values, fold_sum, $(Int, 0));

  CU_ASSERT(as_long(total) is 18);
  
  delete(total);
  delete(values);

}

int init_function_suite(void) {
  return 0;
}

int build_function_suite(CU_pSuite suite) {
  
  if (suite is None) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if ((CU_add_test(suite, "Stack Function", test_stack_function)    is None) or 
      (CU_add_test(suite, "Heap Function", test_heap_function)      is None) or
      (CU_add_test(suite, "Function Assign", test_function_assign)  is None) or 
      (CU_add_test(suite, "Function Copy", test_function_copy)      is None) or 
      
      (CU_add_test(suite, "Call", test_call)                        is None) or 
      (CU_add_test(suite, "Call With", test_call_with)              is None) or 
      (CU_add_test(suite, "Call With Ptr", test_call_with_ptr)      is None) or 
      
      (CU_add_test(suite, "Lambda", test_lambda)                    is None) or
      (CU_add_test(suite, "Lambda Id", test_lambda_id)              is None) or 
      (CU_add_test(suite, "Lambda Const", test_lambda_const)        is None) or 
      (CU_add_test(suite, "Lambda Compose", test_lambda_compose)    is None) or 
      (CU_add_test(suite, "Lambda Flip", test_lambda_flip)          is None) or 
      (CU_add_test(suite, "Lambda Pipe", test_lambda_pipe)          is None) or 
      (CU_add_test(suite, "Lambda Method Pipe", test_lambda_method_pipe)      is None) or 
      (CU_add_test(suite, "Lambda Partial Left", test_lambda_partial_l)       is None) or 
      (CU_add_test(suite, "Lambda Partial Right", test_lambda_partial_r)      is None) or
      (CU_add_test(suite, "Lambda Uncurrying", test_lambda_uncurry)           is None) or 
      (CU_add_test(suite, "Lambda Void Uncurrying", test_lambda_void_uncurry) is None) or
      
      (CU_add_test(suite, "Map", test_map)                  is None) or
      (CU_add_test(suite, "New Map", test_new_map)          is None) or
      (CU_add_test(suite, "New Filter", test_new_filter)    is None) or
      (CU_add_test(suite, "New Sum", test_new_sum)          is None) or
      (CU_add_test(suite, "New Product", test_new_product)  is None) or
      (CU_add_test(suite, "New Fold Left", test_new_foldl)  is None) or
      (CU_add_test(suite, "New Fold Right", test_new_foldr) is None))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  return 0;
  
}