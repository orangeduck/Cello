/*
** == Closure ==
**
**  Higher Order Functional Tools
**
**  All "lambda" expressions must be
**  at the /statement/ level. This means
**  they cannot be used inside expressions.
**
**  Functions that allocate memory begin 
**  with "new". Others can be assumed
**  destructive.
*/

#include "Function+.h"
#include "Curry+.h"

/* More ways to construct Functions */
#define lambda_id(name, f) lambda(name, args) { return call_with(f, args); }

#define lambda_const(name, val) lambda(name, args) { return val; }

#define lambda_compose(name, f, g) \
  lambda(name, args) { \
    return call(f, call_with(g, args)); \
  }

#define lambda_flip(name, f) \
  lambda(name, args) { \
    var argflip = copy(args); reverse(args); \
    var ret = call_with(f, args); \
    return delete(argflip), ret; \
  }
  
#define lambda_partial(name, f, lhs) \
  lambda(name, args) { \
    var argfull = copy(args); push_front(argfull, lhs); \
    var ret = call_with(f, argfull); \
    return delete(argfull), ret; \
  }
  
/* Convert existing functions to Function */ 
#define lambda_void(name, func_ptr) lambda(name, args) { func_ptr(args); return None; }
#define lambda_uncurry(name, func_ptr, argc) lambda_uncurry##argc(name, func_ptr)
#define lambda_void_uncurry(name, func_ptr, argc) lambda_void_uncurry##argc(name, func_ptr)

/* Applies to collection, ignores return */
void map(var self, var func);

/* As above but returns new heap allocated data */
var new_map(var self, var func);
var new_filter(var self, var eq_func);
var new_foldl(var self, var func, var base);
var new_foldr(var self, var func, var base);
var new_sum(var self);
var new_product(var self);

