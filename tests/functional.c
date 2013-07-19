
#include "ptest.h"
#include "Cello.h"

PT_SUITE(suite_functional) {

  var empty_function(var args) {
    return None;
  }
  
  PT_TEST(test_stack_function) {
    var f = $(Function, empty_function);
    PT_ASSERT(f);
  }

  PT_TEST(test_heap_function) {
    var f = new(Function, empty_function);
    PT_ASSERT(f);
    delete(f);
  }

  var empty_function2(var args) {
    return Some;
  }

  PT_TEST(test_function_assign) {
    
    var f1 = new(Function, empty_function);
    var f2 = new(Function, empty_function2);
    
    PT_ASSERT(((FunctionData*)f1)->func isnt ((FunctionData*)f2)->func);
    
    assign(f1, f2);
    
    PT_ASSERT(((FunctionData*)f1)->func == ((FunctionData*)f2)->func);
    
    delete(f1);
    delete(f2);
    
  }

  PT_TEST(test_function_copy) {

    var f1 = new(Function, empty_function);
    var f2 = copy(f1);
    
    PT_ASSERT(((FunctionData*)f1)->func is ((FunctionData*)f2)->func);
    
    delete(f1);
    delete(f2);

  }

  PT_TEST(test_call) {
    
    var result1 = call($(Function, empty_function), None);
    var result2 = call($(Function, empty_function2), None);
    
    PT_ASSERT(result1 is None);
    PT_ASSERT(result2 is Some);
    
  }

  var asserts_args(var args) {
    PT_ASSERT(at(args,0));
    PT_ASSERT(at(args,1));
    return None;
  }

  PT_TEST(test_call_with) {
    
    var args = new(List, 2, $(Int, 1), $(Int, 5));
    
    var assert_func = $(Function, asserts_args);
    
    var result = call_with(assert_func, args);
    
    PT_ASSERT(result is None);
    
    delete(args);
    
  }

  PT_TEST(test_call_with_ptr) {

    var* args = (var[]){ $(Int, 1), $(Int, 5), (var)-1 };
    
    var assert_func = $(Function, asserts_args);
    
    var result = call_with_ptr(assert_func, args);
    
    PT_ASSERT(result is None);

  }

  PT_TEST(test_lambda) {

    var out = new(String, "");
    
    lambda(hello_name, args) {
      var name = cast(at(args, 0), String);
      var out = cast(at(args, 1), String);
      assign(out, name);
      return None;
    }
    
    call(hello_name, $(String, "Hello Bob!"), out);
    
    PT_ASSERT( eq(out, $(String, "Hello Bob!")) );
    
    delete(out);
  }

  PT_TEST(test_lambda_id) {
    
    lambda(return_new_int1, args) {
      return new(Int, 1);
    }
    
    lambda_id(return_new_int2, return_new_int1);
    
    var res1 = call(return_new_int1, None);
    var res2 = call(return_new_int2, None);
    
    PT_ASSERT(as_long(res1) is 1);
    PT_ASSERT(as_long(res2) is 1);
    PT_ASSERT(as_long(res1) == as_long(res2));
    
    delete(res1);
    delete(res2);
    
  }

  PT_TEST(test_lambda_const) {
    
    lambda_const(return_some, Some);
    
    var res = call(return_some, None);
    
    PT_ASSERT(res is Some);
    
  }

  PT_TEST(test_lambda_compose) {
    
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
    
    PT_ASSERT(res is Some);
    
  }

  PT_TEST(test_lambda_flip) {
    
    lambda(return_first, args) {
      return at(args, 0);
    }
    
    lambda_flip(return_first_flip, return_first);
    
    var arg1 = $(Int, 5);
    var arg2 = $(Int, 10);
    
    var res1 = call(return_first, arg1, arg2);
    var res2 = call(return_first_flip, arg1, arg2);
    
    PT_ASSERT(res1 is arg1);
    PT_ASSERT(res2 is arg2);
    
  }

  PT_TEST(test_lambda_pipe) {
    
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
    
    PT_ASSERT(as_long(total) is 111);
    
  }

  PT_TEST(test_lambda_method_pipe) {
    
    var str = new(String, "");
    
    lambda(cat_fizz, args) {
      append(at(args, 0), $(String, "Fizz"));
      return None;
    }
    
    lambda(cat_buzz, args) {
      append(at(args, 0), $(String, "Buzz"));
      return None;
    }
    
    lambda(cat_boo, args) {
      append(at(args, 0), $(String, "Boo"));
      return None;
    }
    
    lambda_method_pipe(cat_all, cat_fizz, cat_buzz, cat_boo);
    
    var res = call(cat_all, str);
    
    PT_ASSERT_STR_EQ(as_str(str), "FizzBuzzBoo");
    
    delete(str);
    
  }

  PT_TEST(test_lambda_partial_l) {
    
    lambda(add_to_first, args) {
      add(at(args,0), at(args,1));
      add(at(args,0), at(args,2));
      add(at(args,0), at(args,3));
      return None;
    }
    
    var total = $(Int, 0);
    
    lambda_partial(add_to_total, add_to_first, total);
    
    var res = call(add_to_total, $(Int, 1), $(Int, 10), $(Int, 100));
    
    PT_ASSERT(as_long(total) is 111);
  }

  PT_TEST(test_lambda_partial_r) {

    lambda(add_to_last, args) {
      add(at(args,3), at(args,0));
      add(at(args,3), at(args,1));
      add(at(args,3), at(args,2));
      return None;
    }
    
    var total = $(Int, 0);
    
    lambda_partial_r(add_to_total, add_to_last, total);
    
    var res = call(add_to_total, $(Int, 1), $(Int, 10), $(Int, 100));
    
    PT_ASSERT(as_long(total) is 111);

  }

  var return_snd(var fst, var snd) {
    return snd;
  }

  PT_TEST(test_lambda_uncurry) {
    
    lambda_uncurry(return_snd_uncurried, return_snd, 2);
    
    var res = call(return_snd_uncurried, None, Some);
    
    PT_ASSERT(res is Some);
    
  }

  void snd_to_fst(var fst, var snd) {
    assign(fst, snd);
  }

  PT_TEST(test_lambda_void_uncurry) {

    lambda_void_uncurry(snd_to_fst_uncurried, snd_to_fst, 2);
    
    var fst = $(Int, 0);
    var snd = $(Int, 64);
    
    var res = call(snd_to_fst_uncurried, fst, snd);
    
    PT_ASSERT(as_long(fst) is 64);
    
  }

  PT_TEST(test_map) {
    
    lambda(add_one, args) {
      add(at(args, 0), $(Int, 1));
      return None;
    }
    
    var values = new(List, 3, $(Int, 5), $(Int, 3), $(Int, 10));
    
    map(values, add_one);
    
    PT_ASSERT(as_long(at(values, 0)) is 6);
    PT_ASSERT(as_long(at(values, 1)) is 4);
    PT_ASSERT(as_long(at(values, 2)) is 11);
    
    delete(values);
    
  }

  PT_TEST(test_new_map) {
    
    lambda(copy_values, args) {
      return at(args, 0);
    }
    
    var values = new(List, 3, $(Int, 5), $(Int, 3), $(Int, 10));
    
    var new_values = new_map(values, copy_values);
    
    PT_ASSERT(new_values);
    PT_ASSERT(at(values,0) is at(new_values,0));
    
    delete(values);
    delete(new_values);

  }

  PT_TEST(test_new_filter) {
    
    lambda(only_some, args) {
      return (var)(intptr_t)(at(args,0) is Some);
    }
    
    var values = new(List, 3, Some, Some, None);
    var somes = new_filter(values, only_some);
    
    PT_ASSERT(len(somes) is 2);
    
    delete(values);
    delete(somes);
    
  }

  PT_TEST(test_new_sum) {

    var values = new(List, 3, $(Int, 5), $(Int, 3), $(Int, 10));
    var total = new_sum(values);

    PT_ASSERT(as_long(total) is 18);
    
    delete(total);
    delete(values);
    
  }

  PT_TEST(test_new_product) {

    var values = new(List, 3, $(Int, 5), $(Int, 3), $(Int, 10));
    var total = new_product(values);
    
    PT_ASSERT(as_long(total) is 150);
    
    delete(total);
    delete(values);

  }

  PT_TEST(test_new_foldl) {
    
    lambda(fold_sum, args) {
      var base = at(args, 0);
      var item = at(args, 1);
      add(base, item);
      return None;
    }
    
    var values = new(List, 3, $(Int, 5), $(Int, 3), $(Int, 10));
    var total = new_foldl(values, fold_sum, $(Int, 0));

    PT_ASSERT(as_long(total) is 18);
    
    delete(total);
    delete(values);
    
  }

  PT_TEST(test_new_foldr) {

    lambda(fold_sum, args) {
      var base = at(args, 0);
      var item = at(args, 1);
      add(base, item);
      return None;
    }
    
    var values = new(List, 3, $(Int, 5), $(Int, 3), $(Int, 10));
    var total = new_foldr(values, fold_sum, $(Int, 0));

    PT_ASSERT(as_long(total) is 18);
    
    delete(total);
    delete(values);

  }

}
