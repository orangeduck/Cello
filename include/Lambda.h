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

#ifndef CelloLambda_h
#define CelloLambda_h

#include "Function.h"
#include "LambdaCurry.h"

#define __VA_ARGC__(...) (sizeof((var[]){ __VA_ARGS__ }) / sizeof(var))

/* More ways to construct Functions */
#define lambda_id(name, f) lambda(name, args) { return call_with(f, args); }

#define lambda_const(name, val) lambda(name, args) { return val; }

#define lambda_compose(name, f, g) \
  lambda(name, args) { \
    return call_with(f, call_with(g, args)); \
  }

#define lambda_flip(name, f) \
  lambda(name, args) { \
    var argflip = copy(args); reverse(args); \
    var ret = call_with(f, args); \
    return delete(argflip), ret; \
  }

/* Partial Evaluation */  
#define lambda_partial lambda_partial_l

#define lambda_partial_l(name, f, ...) \
  lambda(name, args) { \
    var partargs = new(List, __VA_ARGC__(__VA_ARGS__), __VA_ARGS__); \
    foreach(a in args) { push(partargs, a); } \
    var ret = call_with(f, partargs); \
    return delete(partargs), ret; \
  }
  
#define lambda_partial_r(name, f, ...) \
  lambda(name, args) { \
    var partargs = new(List, __VA_ARGC__(__VA_ARGS__), __VA_ARGS__); \
    for(int i = len(args)-1; i >= 0; i--) { \
      push_front(partargs, at(args,i) ); \
    } \
    var ret = call_with(f, partargs); \
    return delete(partargs), ret; \
  }


/* Convert existing c-functions to Function */ 
#define lambda_void(name, func_ptr) lambda(name, args) { func_ptr(args); return None; }
#define lambda_uncurry(name, func_ptr, argc) lambda_uncurry##argc(name, func_ptr)
#define lambda_void_uncurry(name, func_ptr, argc) lambda_void_uncurry##argc(name, func_ptr)

/* Compose several Function objects */
#define lambda_pipe(name, ...) \
  lambda(name, args) { \
    var funcs = new(List,  __VA_ARGC__(__VA_ARGS__), __VA_ARGS__); \
    foreach(f in funcs) { args = call_with(f, args); } \
    return delete(funcs), args; \
  }

#define lambda_method_pipe(name, ...) \
  lambda(name, args) { \
    var funcs = new(List,  __VA_ARGC__(__VA_ARGS__), __VA_ARGS__); \
    foreach(f in funcs) { call_with(f, args); } \
    return delete(funcs), None; \
  }

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
