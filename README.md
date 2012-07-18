C+
==

About
-----

__What is C+?__

C+ is a C library which brings higher level programming tools to C. It implements various ideas and takes inspiration from C++, Haskell and Python. Most closely C+ resembles an implementation of C with interfaces, dynamic typing, and some syntactic sugar.

__Why would you write this?__

Primarily for fun and learning. I wouldn't seriously recommend it's use in production code, although I'd love to say it would make a cute little scripting language for C/C++ based applications.

__Why the weird terminology?__

Terminology is taken from Haskell. What I don't enjoy in Haskell is writing small detailed algorithms in a functional style. But I do love the way it (and C) programs are structured at a higher level. C will never be object oriented but I hope that with C+ I've turned it into something of a dynamic and powerful functional language.

__Does this break anything?__

There are a selection of new keywords and many generically named functions in the namespace are taken but other than that it should be fully compatible with normal C99 code.

Examples
--------

Using a list

```c
var numbers = new(List, 3, $(Int, 1), $(Real, 10.12), $(Int, 6));

foreach(numbers, num) {
  if (type_of(num) is Int) {
    printf("Int Number: %li\n", as_long(num));
  } elif (type_of(num) is Real) {
    printf("Real Number: %f\n", as_double(num));
  }
}

delete(numbers);
```

Using a hashtable

```c
var prices = new(HashTable);

put(prices, $(String, "Apple"),  $(Int, 12)); 
put(prices, $(String, "Banana"), $(Int,  6)); 
put(prices, $(String, "Pear"),   $(Int, 55)); 

var pear_price = get(prices, $(String, "Pear"));

printf("Price of a 'Pear' is '%li'\n", as_long(pear_price));

foreach(prices, key) {
  var price = get(prices, key);
  printf("Price of '%s' is '%li'\n", as_str(key), as_long(price));
}

delete(prices);
```

Defining a new class

```c
/** Num */

class {
  void (*add)(var, var);
  void (*sub)(var, var);
  void (*mul)(var, var);
  void (*div)(var, var);
  void (*negate)(var);
  void (*absolute)(var);
} Num;

void add(var, var);
void sub(var, var);
void mul(var, var);
void divide(var, var);

void negate(var);
void absolute(var);
```

Defining a new type

```
module Int;

data {
  var type;
  long value;
} IntData;

var Int_New(var, va_list*);
var Int_Delete(var);
var Int_Copy(var);

bool Int_Eq(var, var);
bool Int_Gt(var, var);
bool Int_Lt(var, var);

long Int_Hash(var);

void Int_Add(var, var);
void Int_Sub(var, var);
void Int_Mul(var, var);
void Int_Div(var, var);
void Int_Neg(var);
void Int_Abs(var);

long Int_AsLong(var);
double Int_AsDouble(var);

instance(Int, New) = { sizeof(IntData), Int_New, Int_Delete };
instance(Int, Copy) = { Int_Copy };
instance(Int, Eq) = { Int_Eq };
instance(Int, Ord) = { Int_Gt, Int_Lt };
instance(Int, Hash) = { Int_Hash };
instance(Int, AsLong) = { Int_AsLong };
instance(Int, AsDouble) = { Int_AsDouble };
instance(Int, Num) = { Int_Add, Int_Sub, Int_Mul, Int_Div, Int_Neg, Int_Abs };
```

Then in the .c file

```c
var Int = methods {
  methods_begin(Int),
  method(Int, New), 
  method(Int, Copy),
  method(Int, Eq), 
  method(Int, Ord),
  method(Int, Hash),
  method(Int, AsLong),
  method(Int, AsDouble),
  method(Int, Num),
  methods_end(Int)
};

var Int_New(var self, va_list* args) {
  IntData* io = cast(self, Int);
  io->value = va_arg(*args, int);
  return self;
}

var Int_Delete(var self) {
  return self;
}

var Int_Copy(var self) {
  IntData* io = cast(self, Int);
  return new(Int, io->value);
}

...
```

More info and builtin definitions in "Prelude+.h"
