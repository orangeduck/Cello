C+
==

Example
-------

```c
/*
** Example C+ Program
*/

#include "C+.h"

int main(int argc, char** argv) {

  /* Stack objects are created using "$" */
  var int_item = $(Int, 5);
  var float_item = $(Real, 2.4);
  var string_item = $(String, "Hello");

  /* Heap objects are created using "new" */
  var items = new(List, 3, int_item, float_item, string_item);
	
  /* Collections can be looped over */
  foreach(items, item) {
    /* Types are also objects */
    var type = type_of(item);
    printf("Type: '%s'\n", as_str(type));
  }
  
  /* Heap objects destroyed with "delete" */
  delete(items);
  
  /* Tables require "Eq" and "Hash" on key type */
  var prices = new(Table, String, Int);
  put(prices, $(String, "Apple"),  $(Int, 12)); 
  put(prices, $(String, "Banana"), $(Int,  6)); 
  put(prices, $(String, "Pear"),   $(Int, 55)); 
  
  var pear_price = get(prices, $(String, "Pear"));
  printf("Price of a 'Pear' is '%li'\n", as_long(pear_price));

  /* Tables also supports iteration */
  foreach(prices, key) {
    var price = get(prices, key);
    printf("Price of '%s' is '%li'\n", as_str(key), as_long(price));
  }
  
  /* "with" automatically closes File at end of scope. */
  with(open($(File, NULL), "prices.bin", "wb"), file) {
  
    /* First class function object */
    lambda(write_pair, args) {
      
      /* Run time type-checking with "cast" */
      var key = cast(at(args, 0), String);
      var val = cast(get(prices, key), Int);
      
      /* File implements "put/get" like Table */
      put(file, String, key);
      put(file, Int, val);
      
      return None;
    };
    
    /* Higher order functions */
    map(prices, write_pair);
  
  }
  
  delete(prices);
  
}
```

About
-----

C+ is a C GNU99 _library_ which brings higher level programming tools to C. It takes inspiration from C++, Obj-C, Haskell and Python. Most closely C+ resembles C with interfaces, dynamic/duck typing, and some syntactic sugar. There are a selection of new keywords, and many generically named functions in the namespace are taken, but other than that it should be fully compatible with normal C code.

Although I've made the syntax pleasant, this isn't a library for beginners. It is for C power users, as manual memory management doesn't play nicely with many higher-order concepts.

What I don't enjoy in Haskell is writing small detailed algorithms in a functional style. What I do love about Haskell (and C) is the way programs are structured at a higher level. With this project I never really aimed for Object Orientation in C, but I hope that with C+ I've turned made something of a dynamic and powerful functional language which C might have once been. This also explains why some terminology is closer to Haskell than C++/Java.

More Examples
-------------

Some functional tools using `lambda` statements.

```c
#include "C+.h"

int main(int argc, char** argv) {
  
  /*
  ** Basic method of lambda construction .
  ** Must be at the statement level.
  ** Cannot be used in/as expression.
  */
  lambda(hello_name, args) {
  
    /* Input is a list of arguments */
    var name = cast(at(args, 0), String);
    printf("Hello %s!\n", as_str(name));
    
    /* Always must return */
    return None; 
  }
  
  /* Functions called with "call" */
  call(hello_name, $(String, "Bob"));
  
  /* Higher order functions supported */
  var names = new(List, 3, $(String, "Dan"), $(String, "Robert"), $(String, "Chris"));
  map(names, hello_name);
  delete(names);
  
  /* Here is an example with two arguments */
  lambda(add_print, args) {
    int fst = as_long(cast(at(args, 0), Int));
    int snd = as_long(cast(at(args, 1), Int));
    printf("%i + %i = %i\n", fst, snd, fst+snd);
    return None;
  }
  
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
    printf("Hello %s and %s!\n", as_str(fst), as_str(snd));
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
  Vector* ivector = type_class(type_of(self), Vector);
  assert(ivector->dot);
  return ivector->dot(self, obj);
}

float length(var self) {
  Vector* ivector = type_class(type_of(self), Vector);
  assert(ivector->length);
  return ivector->length(self);
}
```

__Defining a new Type.__

In the header...

```c
module Vec2;

data {
  var type;
  float x, y;
} Vec2Data;

/** Vec2_New(var self, float x, float y); */
var Vec2_New(var self, va_list* args);
var Vec2_Delete(var self);
void Vec2_Assign(var self, var obj);
var Vec2_Copy(var self);

var Vec2_Eq(var self, var obj);

float Vec2_Dot(var self, var obj);
float Vec2_Length(var self);

instance(Vec2, New) = { sizeof(Vec2Data), Vec2_New, Vec2_Delete };
instance(Vec2, Assign) = { Vec2_Assign };
instance(Vec2, Copy) = { Vec2_Copy };
instance(Vec2, Eq) = { Vec2_Eq };
instance(Vec2, Vector) = { Vec2_Dot, Vec2_Length };
```

And in the source file...

```c
var Vec2 = methods {
  methods_begin(Vec2),
  method(Vec2, New),
  method(Vec2, Assign),
  method(Vec2, Copy),
  method(Vec2, Eq),
  method(Vec2, Vector),
  methods_end(Vec2)
};

var Vec2_New(var self, va_list* args) {
  Vec2Data* v = cast(self, Vec2);
  v->x = va_arg(*args, double);
  v->y = va_arg(*args, double);
  return self;
}

var Vec2_Delete(var self) {
  return self;
}

void Vec2_Assign(var self, var obj) {
  Vec2Data* v1 = cast(self, Vec2);
  Vec2Data* v2 = cast(obj, Vec2);
  v1->x = v2->x;
  v1->y = v2->y;
}

var Vec2_Copy(var self) {
  Vec2Data* v = cast(self, Vec2);
  return new(Vec2, v->x, v->y);
}

var Vec2_Eq(var self, var obj) {
  Vec2Data* v1 = cast(self, Vec2);
  Vec2Data* v2 = cast(obj, Vec2);
  return (var)(v1->x is v2->x and v1->y is v2->y);
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

Memory management is hard. Very hard when combined with a lack of rich stack types. Very very hard when combined with a whole load of high level concepts. C+ gives you a few options, which where possible, the standard library is agnostic too. You can use what you think is best.

* __Destructive Operations__ - Most of the standard library uses destructive operations and expects the user to make a copy if they exlicity want one.
* __Output Parameters__ - In some places it is more appropriate to use output parameters and in which case "assign" is used to move the data around. 
* __Reference Objects__ - These can be used to wrap and declare lifetimes for normal objects.

```c
local void object_lifetime_example(void) {
  
  with($(Reference, new(String, "Life is long")), liferef) {
    printf("This string is alive: '%s'\n", as_str(at(liferef,0)));
  }

  printf("Now it has been cleared up!\n");
  
}

/* They can also be stacked */

local void many_object_lifetimes(void) {
  
  with($(Reference, new(String, "Life is long")), liferef0)
  with($(Reference, new(String, "Life is Beautiful")), liferef1)
  with($(Reference, new(String, "Life is Grand")), liferef2) {
    printf("%s :: %s :: %s\n", as_str(at(liferef0,0)), as_str(at(liferef1,0)), as_str(at(liferef2,0)));  
  }

}
```

* __Reference Pools__ - Reference pools are also avaliable which use `retain` and `release` to providing a reference counting mechanism.

```c
#include "C+.h"

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

While useless in such a trivial example, because the pool always cleans up references on delete, it can be very useful to avoid memory leaks in things such as event loops or with general pooled memory. It can be used as a poor mans sweep garbage collector.


More More More More Examples
----------------------------

C+ provides a kind of exception handling to deal with errors.

```c
local var DivideByZeroError = Singleton(DivideByZeroError);

local int try_divide(int x, int y) {
  if (y == 0) {
    throw(DivideByZeroError, "Division By Zero (%i / %i)", x, y);
  } else {
    return x / y;
  }
}

int main(int argc, char** argv) {

  try {
    int result = try_divide(2, 0);
  } catch (e, DivideByZeroError) {
    // Panic!
    return 1;
  }
  
  return 0;
}
```

One can also catch multiple objects and then write conditional code based on each.

```c
try {
  do_some_work();
} catch (e, TypeError, ClassError) {
  if (e is TypeError) { printf("Got TypeError!\n"); }
  if (e is ClassError) { printf("Got ClassError!\n"); }
}
```

Throwing an exception will jump the program control to the innermost `catch` block where it must be handled (exceptions to not propagate outward). To catch an exception one must put a reference to the thrown Object. Any Object can be thrown and caught as an Exception in C+ so users can create their own Exception types or find other applications for the semanitcs. The thrown message will be preserved internally, but be careful of throwing stack memory which may become invalidated when jumping to the new location.

More More More More More Examples
---------------------------------

There are some more examples in the demos folder as well as a large amount of reference material under tests and via the source code. Native class definitions (such as for `New`, `Eq`, `Ord`) can be found in `Prelude+.h`


Explanation
------------

The first thing that probably comes into your head viewing the above code is `var`. This is a typedef of `void*` and is used via convention in C+ code to allow for overloaded functions. As you can see in the example code type checking/hinting can be done at runtime via the `cast` function.

This allows for a form of poor-man's duck-typing. If an object looks (or sounds) like it has a length, then you are more than free to use `len` upon it. One can test if a type implements a certain class with the function `type_implements(type, Class)`. Calling a function on an object which does not implement the appropriate classes will throw a ClassError.

Another thing that may have jumped to your mind in the examples is `new`, `delete` and the `$` symbol. These are ways to allocate memory for objects. `new` and `delete` are used for heap objects and call constructors/destructors. `$` is used to allocate objects on the stack. It __doesn't__ call a constructor or destructor, but will initialize the corresponding types data structure with the arguments provided.

The idea of `with` is stolen from Python and will execute `enter_with` and `exit_with` on an object on entrance and exit to the block. The behaviour of these can be defined by implementing the `With` class.

Other than these things there is not much surprising in the code which cannot be explained via syntactic sugar.

* `is`, `not`, `and`, `or` -> `==`, `!`, `&&`, `||`
* `elif` -> `else if`
* `local` -> `static`
* `module` -> `extern var`
* `class` -> `typedef struct`
* `data` -> `typedef struct`

finally the `instance` and `methods` macros are helpers for defining Type objects statically. Because Type objects are somewhat complicated the syntax is very awkward otherwise.

Compiling
---------

To build the just run `make`

To build the tests run `make tests`

To build the examples run `make demos`


Questions & Contributions
-------------------------

Questions, Contributions and Feedback are more than welcome.

You can e-mail me at:

`contact@theorangeduck.com`

Or get in contact via the IRC channel at #c+ on Freenode
