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
  
  /* File-like objects can "open" and "close" */
  var file = open($(File, NULL), "prices.bin", "wb");
  
  /* First class function object */
  lambda(write_pair, args) {
    
    /* Run time type-checking with "cast" */
    var key = cast(at(args, 0), String);
    var val = cast(get(prices, key), Int);
    
    /* File implements "put/get" like Hashtable */
    put(file, String, key);
    put(file, Int, val);
    
    return None;
  };
  
  /* Higher order functions */
  map(prices, write_pair);
  
  close(file);
  
  delete(prices);
  
}
```

About
-----

C+ is a C GNU99 library which brings higher level programming tools to C. It takes inspiration from C++, Obj-C, Haskell and Python. Most closely C+ resembles C with interfaces, dynamic typing, and some syntactic sugar. There are a selection of new keywords, and many generically named functions in the namespace are taken, but other than that it should be fully compatible with normal C code.

Although I've made the syntax pleasant, this isn't a library for beginners. It is for C power users, as manual memory management doesn't play nicely with many higher-order concepts.

What I don't enjoy in Haskell is writing small detailed algorithms in a functional style. What I do love about Haskell (and C) is the way programs are structured at a higher level. With this project I never really aimed for Object Orientation in C, but I hope that with C+ I've turned it into something of a dynamic and powerful functional language which it might have once been. This also expains why some terminology is closer to Haskell than C++/Java.

More Examples
-------------

__Defining a new Class.__

In the header...

```c
/** Num - performs numeric operations */

class {
  void (*add)(var, var);
  void (*sub)(var, var);
  void (*mul)(var, var);
  void (*div)(var, var);
  void (*negate)(var);
  void (*absolute)(var);
} Num;

void add(var self, var obj);
void sub(var self, var obj);
void mul(var self, var obj);
void divide(var self, var obj);

void negate(var self);
void absolute(var self);
```

And in the object file...

```c
void add(var lhs, var rhs) {
  Num* inum = Type_Class(type_of(lhs), Num);
  assert(inum->add);
  inum->add(lhs, rhs);
}

...
```

__Defining a new Type.__

In the header...

```c
/*
** == Int ==
**
**  Basic integer wrapper
*/

module Int;

data {
  var type;
  long value;
} IntData;

/** Int_New(var self, long value); */
var Int_New(var self, va_list* args);
var Int_Delete(var self);
void Int_Assign(var self, var obj);
var Int_Copy(var self);

bool Int_Eq(var self, var obj);
bool Int_Gt(var self, var obj);
bool Int_Lt(var self, var obj);

long Int_Hash(var self);

void Int_Add(var self, var obj);
void Int_Sub(var self, var obj);
void Int_Mul(var self, var obj);
void Int_Div(var self, var obj);
void Int_Neg(var self);
void Int_Abs(var self);

void Int_Parse_Read(var self, var stream);
void Int_Parse_Write(var self, var stream);

long Int_AsLong(var self);
double Int_AsDouble(var self);

instance(Int, New) = { sizeof(IntData), Int_New, Int_Delete };
instance(Int, Assign) = { Int_Assign };
instance(Int, Copy) = { Int_Copy };
instance(Int, Eq) = { Int_Eq };
instance(Int, Ord) = { Int_Gt, Int_Lt };
instance(Int, Hash) = { Int_Hash };
instance(Int, Parse) = { Int_Parse_Read, Int_Parse_Write };
instance(Int, AsLong) = { Int_AsLong };
instance(Int, AsDouble) = { Int_AsDouble };
instance(Int, Num) = { Int_Add, Int_Sub, Int_Mul, Int_Div, Int_Neg, Int_Abs };
```

And in the object file...

```c
var Int = methods {
  methods_begin(Int),
  method(Int, New), 
  method(Int, Assign),
  method(Int, Copy),
  method(Int, Eq), 
  method(Int, Ord),
  method(Int, Hash),
  method(Int, Parse),
  method(Int, AsLong),
  method(Int, AsDouble),
  method(Int, Num),
  methods_end(Int)
};

var Int_New(var self, va_list* args) {
  IntData* intdata = cast(self, Int);
  intdata->value = va_arg(*args, int);
  return self;
}

var Int_Delete(var self) {
  return self;
}

void Int_Assign(var self, var obj) {
  IntData* intdata = cast(self, Int);
  intdata->value = as_long(obj);
}

...
```

More information and native class definitions (such as for ```New```, ```Eq```, ```Ord```) can be found in ```Prelude+.h```

Explaination
------------

The first thing that probably comes into your head viewing the above code is ```var```. This is a typedef of ```void*``` and is used via convention in C+ code to allow for overloaded functions. As you can see in the example code type checking/hinting can be done at runtime via the ```cast``` function.

This allows for a form of poor-man's duck-typing. If an object looks (or sounds) like it has a length, then you are more than free to use ```len``` upon it. One can test if a type implements a certain class with the function ```Type_Implements(type, Class)```. Calling a function on an object which does not implement the appropriate classes will throw a runtime error.

Another thing that may have jumped to your mind in the examples is ```new```, ```delete``` and the ```$``` symbol. These are ways to allocate memory for objects. ```new``` and ```delete``` are used for heap objects and call constructors/destructors. ```$``` is used to allocate objects on the stack. It __doesn't__ call a constructor or destructor, but will initialize the coorisponding types data structure with the arguments provided.

Other than these things there is not much suprising in the code which cannot be explained via syntactic sugar.

* ```is``` -> ```==```
* ```not``` -> ```!```
* ```elif``` -> ```else if```
* ```module``` -> ```extern var```
* ```class``` -> ```typedef struct```
* ```data``` -> ```typedef struct```

finally the ```instance``` and ```methods``` macros are helpers for defining Type objects statically. Because Type objects are somewhat complicated the syntax is very awkward otherwise.

Compiling
---------

To build the just run "make"

To build the tests run "make test" (requires CUnit)

To build the examples run "make examples"

More Examples
-------------

Some functional tools using "lambda" statements.

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

Returning Values
----------------

Having every data object as a reference makes returning data objects with manual memory management annoying. To return a full data object one has to allocate it on the heap and expect the user to delete it once they are done with it. There are a couple alternatives to this when such an approach is unacceptable:

* Embrace destructive operations - Although not as nice semantically as duplicative ones, consistency is the key issue. How often is it we mean not to act destructively anyway? Assume destructive functions by default, return void.

* Use output parameteres - Use arguments intended for output, passed into a function. Use the "assign" function or otherwise to assign them data.


Questions & Contributions
-------------------------

Questions, Contributions and Feedback are more than welcome.

You can e-mail me at:

contact@theorangeduck.com

Or get in contact via the IRC channel at #c+ on Freenode
