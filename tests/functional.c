#include "Cello.h"
#include "ptest.h"

PT_FUNC(test_stack_function) {

#if defined(__clang__)
  var (^empty_function)(var) = ^ var (var args) { return None; };
#else
  var empty_function(var args) { return None; }  
#endif

  var f = $(Function, empty_function);
  PT_ASSERT(f);
}

PT_FUNC(test_heap_function) {

#if defined(__clang__)
  var (^empty_function)(var) = ^ var (var args) { return None; };
#else
  var empty_function(var args) { return None; }  
#endif

  var f = new(Function, $(Function, empty_function));
  PT_ASSERT(f);
  del(f);
}

PT_FUNC(test_function_assign) {
  
#if defined(__clang__)
  var (^empty_function)(var) = ^ var (var args) { return None; };
  var (^empty_function2)(var) = ^ var (var args) { return Some; };
#else
  var empty_function(var args) { return None; }  
  var empty_function2(var args) { return Okay; }
#endif
  
  var f1 = new(Function, $(Function, empty_function));
  var f2 = new(Function, $(Function, empty_function2));
  
  PT_ASSERT(((struct Function*)f1)->func isnt ((struct Function*)f2)->func);
  
  assign(f1, f2);
  
  PT_ASSERT(((struct Function*)f1)->func == ((struct Function*)f2)->func);
  
  del(f1);
  del(f2);
  
}

PT_FUNC(test_function_copy) {

#if defined(__clang__)
  var (^empty_function)(var) = ^ var (var args) { return None; };
#else
  var empty_function(var args) { return None; }  
#endif

  var f1 = new(Function, $(Function, empty_function));
  var f2 = copy(f1);
  
  PT_ASSERT(((struct Function*)f1)->func is ((struct Function*)f2)->func);
  
  del(f1);
  del(f2);

}

PT_FUNC(test_call) {
  
#if defined(__clang__)
  var (^empty_function)(var) = ^ var (var args) { return None; };
  var (^empty_function2)(var) = ^ var (var args) { return Some; };
#else
  var empty_function(var args) { return None; }  
  var empty_function2(var args) { return Okay; }
#endif
  
  var result1 = call($(Function, empty_function));
  var result2 = call($(Function, empty_function2));
  
  PT_ASSERT(result1 is None);
  PT_ASSERT(result2 is Okay);
  
}

PT_FUNC(test_call_with) {
  
#if defined(__clang__)

  var (^asserts_args)(var) = ^ var (var args) {
    PT_ASSERT(get(args, $(Int, 0)));
    PT_ASSERT(get(args, $(Int, 1)));
    return None;
  };
  
#else

  var asserts_args(var args) {
    PT_ASSERT(get(args, $(Int, 0)));
    PT_ASSERT(get(args, $(Int, 1)));
    return None;
  }
  
#endif
  
  var args = new(Array, Int, $(Int, 1), $(Int, 5));
  
  var assert_func = $(Function, asserts_args);
  
  var result = call_with(assert_func, args);
  
  PT_ASSERT(result is None);
  
  del(args);
  
}

PT_FUNC(test_call_vl) {

#if defined(__clang__)

  var (^asserts_args)(var) = ^ var (var args) {
    PT_ASSERT(get(args, $(Int, 0)));
    PT_ASSERT(get(args, $(Int, 1)));
    return None;
  };
  
#else

  var asserts_args(var args) {
    PT_ASSERT(get(args, $(Int, 0)));
    PT_ASSERT(get(args, $(Int, 1)));
    return None;
  }
  
#endif

  var args = tuple($(Int, 1), $(Int, 5));
  
  var assert_func = $(Function, asserts_args);
  
  var result = call_with(assert_func, args);
  
  PT_ASSERT(result is None);

}

PT_FUNC(test_lambda) {

  var out = new(String, $(String, ""));
  
  function(hello_name, args) {
    var name = cast(get(args, $(Int, 0)), String);
    var out = cast(get(args, $(Int, 1)), String);
    assign(out, name);
    return None;
  };
  
  call(hello_name, $(String, "Hello Bob!"), out);
  
  PT_ASSERT( eq(out, $(String, "Hello Bob!")) );
  
  del(out);
}

PT_FUNC(test_map) {
  
  function(add_one, args) {
    madd(get(args, $(Int, 0)), $(Int, 1));
    return None;
  };
  
  var values = new(Array, Int, $(Int, 5), $(Int, 3), $(Int, 10));
  
  map(values, add_one);
  
  PT_ASSERT(c_int(get(values, $(Int, 0))) is 6);
  PT_ASSERT(c_int(get(values, $(Int, 1))) is 4);
  PT_ASSERT(c_int(get(values, $(Int, 2))) is 11);
  
  del(values);
  
}



PT_SUITE(suite_functional) {
  
  PT_REG(test_stack_function);
  PT_REG(test_heap_function);
  PT_REG(test_function_assign);
  PT_REG(test_function_copy);
  PT_REG(test_call);
  PT_REG(test_call_with);
  PT_REG(test_call_vl);
  PT_REG(test_lambda);
  PT_REG(test_map);

}
