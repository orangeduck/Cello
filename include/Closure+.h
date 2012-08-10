/*
** == Closure ==
**
**  Higher Order Functional Tools
**
**  Functions that allocate memory begin 
**  with "new". Others can be assumed
**  destructive.
**
**  All "lambda" expressions must be
**  at the /statement/ level. This means
**  they cannot be used inside expressions.
*/

#include "Function+.h"

#define lambda_id(name, f) lambda(name, args) { return call_with(f, args); }

#define lambda_compose(name, f, g) \
  lambda(fnew, args) { \
    return call(f, call_with(g, args)); \
  }

#define lambda_partial(name, f, rhs) \
  lambda(name, args) { \
    var argfull = copy(args); push(argfull, rhs); \
    var ret = call_with(f, argfull); \
    return delete(argfull), ret; \
  }


/* Applies to collection, ignores return */
void map(var self, var func);

/* Applies to collection when func returns None */
void filter(var self, var eq_func);
void sort(var self, var gt_func);

/* Returns ref to smallest or largest in list */
var maximum(var self);
var minimum(var self);

/* As above but returns new heap allocated data */
var new_map(var self, var func);
var new_filter(var self, var eq_func);
var new_sort(var self, var gt_func);
var new_foldl(var self, var func, var base);
var new_foldr(var self, var func, var base);
var new_sum(var self);
var new_product(var self);

