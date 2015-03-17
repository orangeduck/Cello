#include "Cello.h"
#include "ptest.h"

PT_FUNC(test_stack_function) {
  fun (f, args) { return NULL; };
  PT_ASSERT(f);
}

PT_FUNC(test_heap_function) {

  fun (f0, args) { return NULL; }
  var f1 = new(Function, f0);

  PT_ASSERT(f0);
  PT_ASSERT(f1);
  del(f1);
}

PT_FUNC(test_function_assign) {
  
  fun (empty_function1, args) { return NULL; }
  fun (empty_function2, args) { return NULL; }

  var f1 = new(Function, empty_function1);
  var f2 = new(Function, empty_function2);
  
  PT_ASSERT(((struct Function*)f1)->func isnt ((struct Function*)f2)->func);
  
  assign(f1, f2);
  
  PT_ASSERT(((struct Function*)f1)->func == ((struct Function*)f2)->func);
  
  del(f1);
  del(f2);
  
}

PT_FUNC(test_function_copy) {

  fun (empty_function, args) { return NULL; }

  var f1 = new(Function, empty_function);
  var f2 = copy(f1);
  
  PT_ASSERT(((struct Function*)f1)->func is ((struct Function*)f2)->func);
  
  del(f1);
  del(f2);

}

PT_FUNC(test_call) {
  
  fun (empty_function1, args) { return NULL; }
  fun (empty_function2, args) { return NULL; }
  
  var result1 = call(empty_function1);
  var result2 = call(empty_function2);
  
  PT_ASSERT(result1 is NULL);
  PT_ASSERT(result2 is NULL);
  
}

PT_FUNC(test_call_with) {
  
  fun (asserts_args, args) {
    PT_ASSERT(get(args, $I(0)));
    PT_ASSERT(get(args, $I(1)));
    return NULL;
  }
  
  var args = new(Array, Int, $I(1), $I(5));
  var result = call_with(asserts_args, args);
  
  PT_ASSERT(result is NULL);
  
  del(args);
  
}

PT_FUNC(test_call_tuple) {

  fun(asserts_args, args) {
    PT_ASSERT(get(args, $I(0)));
    PT_ASSERT(get(args, $I(1)));
    return NULL;
  }

  var args = tuple($I(1), $I(5));
  var result = call_with(asserts_args, args);
  
  PT_ASSERT(result is NULL);

}

PT_FUNC(test_fun) {

  var out = new(String, $(String, ""));
  
  fun (hello_name, args) {
    var name = cast(get(args, $I(0)), String);
    var out = cast(get(args, $I(1)), String);
    assign(out, name);
    return NULL;
  };
  
  call(hello_name, $(String, "Hello Bob!"), out);
  
  PT_ASSERT( eq(out, $(String, "Hello Bob!")) );
  
  del(out);
}

PT_FUNC(test_map) {
  
  fun (add_one, args) {
    var num = get(args, $I(0));
    assign(num, $I(c_int(num)+1));
    return NULL;
  };
  
  var values = new(Array, Int, $I(5), $I(3), $I(10));
  
  map(values, add_one);
  
  PT_ASSERT(c_int(get(values, $I(0))) is 6);
  PT_ASSERT(c_int(get(values, $I(1))) is 4);
  PT_ASSERT(c_int(get(values, $I(2))) is 11);
  
  del(values);
  
}



PT_SUITE(suite_functional) {
  
  PT_REG(test_stack_function);
  PT_REG(test_heap_function);
  PT_REG(test_function_assign);
  PT_REG(test_function_copy);
  PT_REG(test_call);
  PT_REG(test_call_with);
  PT_REG(test_call_tuple);
  PT_REG(test_fun);
  PT_REG(test_map);

}
