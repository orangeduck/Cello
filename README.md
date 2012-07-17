C+
==

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

Defining a string type

```c
#ifndef StringPlus_h
#define StringPlus_h

#include "Prelude+.h"

module String;

data {
  Type type;
  char* value;
} StringData;

void String_New(var, va_list*);
void String_Delete(var);
var String_Copy(var);

bool String_Eq(var, var);

size_t String_Len(var);
bool String_Contains(var, var);

long String_Hash(var);

const char* String_AsStr(var);

instance(String, New) = { sizeof(StringData), String_New, String_Delete };
instance(String, Copy) = { String_Copy };
instance(String, Eq) = { String_Eq };
instance(String, Len) = { String_Len  };
instance(String, Hash) = { String_Hash  };
instance(String, AsStr) = { String_AsStr  };

#endif
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

FAQ
---

__What is C+?__

C+ is a C library which brings higher level programming tools to C. It is created using various ideas and inspiration from C++, Haskell and Python. Most closely C+ resembles an implementation of C with interfaces, dynamic typing, and some syntactic sugar.

__Why would you write this?__

Primarily for fun and learning. I wouldn't seriously recommend it's use in production code, although I'd love to say it would make a cute little scripting language for C/C++ based applications.

__Why the weird terminology?__

Most of the terminology is taken from Haskell. What I don't enjoy in Haskell is writing small detailed algorithms in a functional style. But I do love the way it (and C) programs are structured at a higher level. C will never be object oriented but I hope that with C+ I've turned it into something of a dynamic and powerful functional language.

__Does this break anything?__

There are a selection of new keywords and many functions in the namespace are taken but other than that it should be fully compatible with normal C99 code.