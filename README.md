C+
==

About
-----

__What is C+?__

C+ is a C library which brings higher level programming tools to C. It takes inspiration from C++, Haskell and Python. Most closely C+ resembles C with interfaces, dynamic typing, and some syntactic sugar. This isn't a new language, just a library for C99. There are a selection of new keywords and many generically named functions in the namespace are taken but other than that it should be fully compatible with normal code.

Primarily I made this for fun and learning. I wouldn't seriously recommend it's use in production code, although I'd love to say it would make a cute little scripting language for C/C++ based applications.

I am a huge fan of C, Python and Haskell. What I don't enjoy in Haskell is writing small detailed algorithms in a functional style. This I feel is best in C/Python. What I do love about Haskell (and C) is the way programs are structured at a higher level. C will never be object oriented but I hope that with C+ I've turned it into something of a dynamic and powerful functional language which it might have once been. This also expains why some of the terminology is closer to Haskell than C++.

Examples
--------

Using a list.

```c
var numbers = new(List, 3, $(Int, 1), $(Real, 10.12), $(Int, 6));

foreach(numbers, num) {
  Type t = type_of(num);
  if (t is Int) {
    printf("%s Number: %li\n", as_str(t), as_long(num));
  } elif (t is Real) {
    printf("%s Number: %f\n", as_str(t), as_double(num));
  }
}

delete(numbers);
```

Using a hashtable.

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

Defining a new class.

In the header...

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

And in the object file...

```c
void add(var lhs, var rhs) {
  Num* inum = Type_Class(type_of(lhs), Num);
  assert(inum->add);
  inum->add(lhs, rhs);
}

...
```

Defining a new type.

In the header...

```c
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

And in the object file...

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
  IntData* intdata = cast(self, Int);
  intdata->value = va_arg(*args, int);
  return self;
}

var Int_Delete(var self) {
  return self;
}

var Int_Copy(var self) {
  IntData* intdata = cast(self, Int);
  return new(Int, intdata->value);
}

...
```

More information and native class definitions (such as New, Eq, Ord) can be found in "Prelude+.h"

Explaination
------------

The first thing that probably comes into your head viewing the above code is "var". This is a typedef to a void* and is used via convention in C+ code to allow for overloaded functions. As you can see in the example code type checking/hinting can be done at runtime via the "cast" function.

This allows for a form of poor-man's duck-typing. If an object looks like it has a length, and it implements "Len", then you are more than free to use "len" upon it. One can test if an object implements a certain class with "Type_Implmenets(type_of(obj), Class)". Calling a function on an object which does not implement the appropriate classes will throw an error.

Another thing that may have jumped to your mind in the examples is "new", "delete" and the "$" symbol. These are ways to allocate memory for objects. "new" and "delete" are used for heap objects and call constructors/destructors. "$" is used to allocate objects on the stack. It _doesn't_ call a constructor or destructor, but will initialize the coorisponding types data structure with the arguments provided.

A final thing that might have popped into your head is that Types of objects are rich objects themselves. The type "Type" implement various classes and can be used with functions such as "as_str". It even implements "New" meaning new Types can be constructed at runtime.

"foreach" is a new addition which allows for iteration over objects which implement the "Iter" interface. Also shown are various generic containers which implement this. The Hashtable can use as a key any object that implements "Hash" and can store any object as a value.

Other than these things there is not much suprising in the code which cannot be explained via syntactic sugar.

* "is" -> "=="
* "not" -> "!"
* "elif" -> "else if"
* "module" -> "extern var"
* "class" -> "typedef struct"
* "data" -> "typedef struct"

"instance" and the "methods" macros being helpers for defining Type objects statically.