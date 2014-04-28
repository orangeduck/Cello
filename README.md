libCello
========

Cello is a _library_ for higher level programming in C.

* __Interfaces__ allow for structured design
* __Duck Typing__ allows for generic functions
* __Exceptions__ control error handling
* __Constructors/Destructors__ aid memory management
* __Syntactic Sugar__ increases readability
* __C Library__ means excellent performance and integration

Example
-------

```c
/* Example libCello Program */

#include "Cello.h"

int main(int argc, char** argv) {

  /* Stack objects are created using "$" */
  var int_item = $(Int, 5);
  var float_item = $(Real, 2.4);
  var string_item = $(String, "Hello");

  /* Heap objects are created using "new" */
  var items = new(List, int_item, float_item, string_item);

  /* Collections can be looped over */
  foreach (item in items) {
    /* Types are also objects */
    var type = type_of(item);
    print("Object %$ has type %$\n", item, type);
  }

  /* Heap objects destroyed with "delete" */
  delete(items); 
}
```

```c
/* Another Example Cello Program */

#include "Cello.h"

int main(int argc, char** argv) {

  /* Tables require "Eq" and "Hash" on key type */
  var prices = new(Table, String, Int);
  put(prices, $(String, "Apple"),  $(Int, 12)); 
  put(prices, $(String, "Banana"), $(Int,  6)); 
  put(prices, $(String, "Pear"),   $(Int, 55));

  /* Tables also supports iteration */
  foreach (key in prices) {
    var price = get(prices, key);
    print("Price of %$ is %$\n", key, price);
  }

  /* "with" automatically closes file at end of scope. */
  with (file in stream_open($(File, NULL), "prices.bin", "wb")) {

    /* First class function object */
    lambda(write_pair, args) {

      /* Run time type-checking with "cast" */
      var key = cast(at(args, 0), String);
      var val = cast(get(prices, key), Int);

      try {
        print_to(file, 0, "%$ :: %$\n", key, val);
      } catch (e in IOError) {
        println("Could not write to file - got %$", e);
      }

      return None;
    };

    /* Higher order functions */
    map(prices, write_pair);
  }

  delete(prices);
}
```

Inspiration
-----------

The high level structure of Cello projects is inspired by Haskell, while the syntax and semantics are inspired by Python and Objective-C. Cello isn't about Object Orientation in C, but I hope that with Cello I've turned C into something of a dynamic and powerful functional language which it may have once been.

Although the syntax is pleasant, Cello isn't a library for beginners. It is for C power users, as manual memory management doesn't play nicely with many higher-order concepts. Most of all Cello is just a fun experiment to see what C would look like when hacked to its limits.

More Examples
-------------

Some functional tools using `lambda` statements.

```c
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
  var Welcome_Pair(var fst, var snd) {
    print("Hello %s and %s!\n", fst, snd);
    return None;
  }
  
  lambda_uncurry(welcome_uncurried, Welcome_Pair, 2);
  
  call(welcome_uncurried, $(String, "John"), $(String, "David"));
  
  return 0;
}
```

More More Examples
------------------

__Defining a new Class.__

A Class is a TypeClass, also known as an Interface. These are defined to allow overloaded or generic functions.

They are defined as follows.

In the header...

```c
/** Vector - vector operations */

class {
  float (*dot)(var, var);
  float (*length)(var);
} Vector;

float dot(var self, var obj);
float length(var self);
```

And in the source file...

```c
float dot(var self, var obj) {
  return type_class_method(type_of(self), Vector, dot, self, obj);
}

float length(var self) {
  return type_class_method(type_of(self), Vector, length, self);
}
```

__Defining a new Type.__

A Type is an structure which implements a number of Classes.

They typically have an associated data object. They can be defined as follows.

In the header...

```c
global var Vec2;

data {
  var type;
  float x, y;
} Vec2Data;

var Vec2_New(var self, var_list vl);
var Vec2_Delete(var self);
size_t Vec2_Size(void);
void Vec2_Assign(var self, var obj);
var Vec2_Copy(var self);

var Vec2_Eq(var self, var obj);

float Vec2_Dot(var self, var obj);
float Vec2_Length(var self);

instance(Vec2, New) = { Vec2_New, Vec2_Delete, Vec2_Size };
instance(Vec2, Assign) = { Vec2_Assign };
instance(Vec2, Copy) = { Vec2_Copy };
instance(Vec2, Eq) = { Vec2_Eq };
instance(Vec2, Vector) = { Vec2_Dot, Vec2_Length };
```

And in the source file...

```c
var Vec2 = type_data {
  type_begin(Vec2),
  type_entry(Vec2, New),
  type_entry(Vec2, Assign),
  type_entry(Vec2, Copy),
  type_entry(Vec2, Eq),
  type_entry(Vec2, Vector),
  type_end(Vec2)
};

var Vec2_New(var self, var_list vl) {
  Vec2Data* v = cast(self, Vec2);
  v->x = as_double(var_list_get(vl));
  v->y = as_double(var_list_get(vl));
  return self;
}

var Vec2_Delete(var self) {
  return self;
}

size_t Vec2_Size(void) {
  return sizeof(Vec2Data);
}

void Vec2_Assign(var self, var obj) {
  Vec2Data* v1 = cast(self, Vec2);
  Vec2Data* v2 = cast(obj, Vec2);
  v1->x = v2->x;
  v1->y = v2->y;
}

var Vec2_Copy(var self) {
  Vec2Data* v = cast(self, Vec2);
  return new(Vec2, $(Real, v->x), $(Real, v->y));
}

var Vec2_Eq(var self, var obj) {
  Vec2Data* v1 = cast(self, Vec2);
  Vec2Data* v2 = cast(obj, Vec2);
  return bool_var(v1->x is v2->x and v1->y is v2->y);
}

float Vec2_Dot(var self, var obj) {
  Vec2Data* v1 = cast(self, Vec2);
  Vec2Data* v2 = cast(obj, Vec2);
  return (v1->x * v2->x + v2->y * v2->y);
}

float Vec2_Length(var self) {
  Vec2Data* v = cast(self, Vec2);
  return sqrt(v->x * v->x + v->y * v->y);
}
```

More More More Examples
-----------------------

Memory management is hard. Very hard when combined with a lack of rich stack types. Very very hard when combined with a whole load of high level concepts. libCello gives you a few options, which where possible, the standard library is agnostic too. You can use what you think is best.

* __Destructive Operations__ - Most of the standard library uses destructive operations and expects the user to make a copy if they explicitly want one.
* __Output Parameters__ - In some places it is more appropriate to use output parameters and in which case `assign` is used to move the data around. 
* __Reference Objects__ - References wrap standard objects, where `with` can be used to declare their lifetime. And `at` can be used to dereference.

```c
local void object_lifetime_example(void) {
  
  var life = $(String, "Life is long")
  
  with(liferef in $(Reference, new(String, life))) {
    print("This string is alive: %$\n", at(liferef,0));
  }

  print("Now it has been cleared up!\n");
  
}

/* They can also be stacked */

local void many_object_lifetimes(void) {
  
  var life0 = $(String, "Life is long")
  var life1 = $(String, "Life is Beautiful")
  var life2 = $(String, "Life is Grand")
  
  with(liferef0 in $(Reference, new(String, life0)))
  with(liferef1 in $(Reference, new(String, life1)))
  with(liferef2 in $(Reference, new(String, life2))) {
    print("%s :: %s :: %s\n", at(liferef0,0), at(liferef1,0), at(liferef2,0));
  }

}
```

* __Reference Pools__ - Reference pools are also avaliable which use `retain` and `release` to providing a reference counting mechanism.

```c
#include "Cello.h"

local var g_pool;

local void table_fill(var x) {
  put(x, $(String, "First"),  $(Real, 0.0));
  put(x, $(String, "Second"), $(Real, 0.1));
  put(x, $(String, "Third"),  $(Real, 5.7));
  release(g_pool, x);
}

local void table_process(var x) {
  put(x, $(String, "First"), $(Real, -0.65));
  release(g_pool, x);
}

int main(int argc, char** argv) {
  
  g_pool = new(Pool);
  
  var x = retain(g_pool, new(Table, String, Real));
  
  table_fill(retain(g_pool, x));
  table_process(retain(g_pool, x));
  
  release(g_pool, x);
  
  delete(g_pool);
  
}
```

While useless in such a trivial example, because the pool always cleans up references on delete, it can be very useful to avoid memory leaks in things such as event loops or with general pooled memory. It can be used as a poor man's sweep garbage collector.


More More More More Examples
----------------------------

libCello provides a kind of exception handling to deal with errors.

```c
local var DivideByZeroError = Singleton(DivideByZeroError);

local int try_divide(int x, int y) {
  if (y == 0) {
    throw(DivideByZeroError, "Division By Zero (%i / %i)", $(Int, x), $(Int, y));
  } else {
    return x / y;
  }
}

int main(int argc, char** argv) {

  try {
    int result = try_divide(2, 0);
  } catch (e in DivideByZeroError) {
    // Panic!
    return 1;
  }
  
  return 0;
}
```

One can also catch multiple objects and then write conditional code based on each. Or one can catch all exceptions or any thrown object by leaving the specifier list empty.

```c
try {
  do_some_work();
} catch (e in TypeError, ClassError) {
  if (e is TypeError) { print("Got TypeError!\n"); }
  if (e is ClassError) { print("Got ClassError!\n"); }
}

try {
  do_some_other_word();
} catch (e) {
  print("Got Exception: %$\n", e);
}
```

Throwing an exception will jump the program control to the innermost `catch` block. If it is not handled here it is passed on to an outer block. To catch an exception one must put a reference to the thrown object. Any object can be thrown and caught as an Exception in libCello so users can create their own Exception types or find other applications for the semantics. The thrown message will be preserved internally, but be careful of throwing stack memory which may become invalidated when jumping to the new location. The `catch` part of a exception block must always be evaluated at risk of breaking the exception stack. This means it must always be included and one should never return from inside a `try` block. To retrieve the exception message thrown one can use `Exception_Message`.

More More More More More Examples
---------------------------------

There are some more examples in the demos folder as well as a large amount of reference material under tests, documentation and source code. Native class definitions (such as for `New`, `Eq`, `Ord`) can be found in `Prelude.h`


Explanation
------------

The first thing that probably comes into your head viewing the above code is `var`. This is a typedef of `void*` and is used via convention in libCello code to allow for overloaded functions. As you can see in the example code type checking/hinting can be done at runtime via the `cast` function.

This allows for a form of poor-man's duck-typing. If an object looks (or sounds) like it has a length, then you are more than free to use `len` upon it. One can test if a type implements a certain class with the function `type_implements(type, Class)`. Calling a function on an object which does not implement the appropriate classes will throw a `ClassError`.

Another thing that may have jumped to your mind in the examples is `new`, `delete` and the `$` symbol. These are ways to allocate memory for objects. `new` and `delete` are used for heap objects and call constructors/destructors. `$` (which I like to think of as a clef) is used to allocate objects on the stack. It __doesn't__ call a constructor or destructor, but will initialize the corresponding types data structure with the arguments provided. This makes it useful for basic or boxed types.

The idea of `with` is stolen from Python and will execute `enter_with` and `exit_with` on an object on entrance and exit to the block. The behaviour of these can be defined by implementing the `With` class. Currently it does not handle exceptions inside the block.

Other than these things there is not much surprising in the code which cannot be explained via syntactic sugar.

* `is`, `isnt`, `not`, `and`, `or` -> `==`, `!=`, `!`, `&&`, `||`
* `elif` -> `else if`
* `local` -> `static`
* `global` -> `extern`
* `class` -> `typedef struct`
* `data` -> `typedef struct`

finally the `instance` and `methods` macros are helpers for defining Type objects statically. Because Type objects are somewhat complicated the syntax is very awkward otherwise.


Questions & Contributions
-------------------------

Questions, Contributions and Feedback are more than welcome.

You can e-mail me at:

`contact@theorangeduck.com`

Or get in contact via the IRC channel at `#libCello` on Freenode
