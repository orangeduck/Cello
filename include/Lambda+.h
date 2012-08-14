/*
** == Lambda ==
**
**  Higher Order Functional Tools
**
**  All "lambda" expressions must be
**  at the /statement/ level. This means
**  they cannot be used inside expressions.
**
*/

#ifndef LambdaPlus_h
#define LambdaPlus_h

#include "Function+.h"
#include "LambdaCurry+.h"
#include "LambdaThread+.h"

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

#define lambda_partial_l lambda_partial
#define lambda_partial_r(name, f, rhs) \
  lambda(name, args) { \
    var argfull = copy(args); push(argfull, rhs); \
    var ret = call_with(f, argfull); \
    return delete(argfull), ret; \
  }
  
/* Convert existing c-functions to Function */ 
#define lambda_void(name, func_ptr) lambda(name, args) { func_ptr(args); return None; }
#define lambda_uncurry(name, func_ptr, argc) lambda_uncurry##argc(name, func_ptr)
#define lambda_void_uncurry(name, func_ptr, argc) lambda_void_uncurry##argc(name, func_ptr)

/* Compose several Function objects */
#define lambda_pipe_thread(name, count, ...) lambda_pipe_thread##count(name, ##__VA_ARGS__)
#define lambda_method_thread(name, count, ...) lambda_method_thread##count(name, ##__VA_ARGS__)

/* Applies to collection, ignores return */
void map(var self, var func);

/* As above but returns new heap allocated data */
var new_map(var self, var func);
var new_filter(var self, var eq_func);
var new_foldl(var self, var func, var base);
var new_foldr(var self, var func, var base);
var new_sum(var self);
var new_product(var self);

#endif