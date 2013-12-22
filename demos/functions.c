
#include "Cello.h"

int main(int argc, char** argv) {
  
  /*
  ** Basic method of lambda construction .
  ** Must be at the statement level.
  ** Cannot be used in/as expression.
  */
  
  lambda(hello_name, args) {
    /* Input is a list of arguments */
    var name = cast(at(args, 0), String);
    print("Hello %s!\n", name);
    
    /* Always must return */
    return None;
  };
  
  /* Functions called with "call" */
  call(hello_name, $(String, "Bob"));
  
  /* Higher order functions supported */
  var names = new(List, $(String, "Dan"), $(String, "Robert"), $(String, "Chris"));
  map(names, hello_name);
  delete(names);
  
  /* Here is an example with two arguments */
  lambda(add_print, args) {
    int fst = as_long(cast(at(args, 0), Int));
    int snd = as_long(cast(at(args, 1), Int));
    printf("%i + %i = %i\n", fst, snd, fst+snd);
    return None;
  };
  
  /*
  ** Notice arguments to "call" in curried form.
  ** Use "call_with" for uncurried calling.
  */
  call(add_print, $(Int, 10), $(Int, 21));
  
  /* Partially applied Function, neat! */
  lambda_partial(add_partial, add_print, $(Int, 5));
  
  call(add_partial, $(Int, 1));

  /*
  ** We can use normal c-functions too.
  ** If they have all argument types as "var".
  ** Then they can be uncurried.
  */
#ifndef __clang__

  var Welcome_Pair(var fst, var snd) {
    print("Hello %s and %s!\n", fst, snd);
    return None;
  }
  
#else

  var (^Welcome_Pair)(var, var) = ^ var (var fst, var snd) {
    print("Hello %s and %s!\n", fst, snd);
    return None;
  };

#endif
  
  lambda_uncurry(welcome_uncurried, Welcome_Pair, 2);
  
  call(welcome_uncurried, $(String, "John"), $(String, "David"));
  
  return 0;
}
