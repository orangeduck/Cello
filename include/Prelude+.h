#ifndef PreludePlus_h
#define PreludePlus_h

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

/*
** Syntax
**
**  New reserved keywords:
**
**  var is not elif
**  module class data instance
**  lit $ foreach cast
*/

typedef void* var;
#define is ==
#define not !
#define elif else if

#define module extern var
#define class typedef struct 
#define data typedef struct 
#define instance(T,C) static const C T##C

/*
** Methods
**
**  These macros make the declaration 
**  of Types statically much easier
*/

#define methods (TypeData*)(const TypeData[])
#define methods_begin(T) {NULL, "__Type"}, {#T, "__Name"}
#define method(T,C) {&T##C, #C}
#define methods_end(T) {NULL, NULL}

/*
** Data
**
**  All data objects must start with "type" member.
**  If this is not respected functions taking "var" _will_ crash.
*/

data {
  var type;
} ObjectData;

var type_of(var);

#define cast(X, T) Type_Cast(X, T, __func__)

/*
** Classes
**
**  Sets of functions which work upon objects.
*/

/** New - heap allocation with constructor/destructor */

class {
  size_t size;
  var (*ctor)(var, va_list*);
  var (*dtor)(var);
} New;

var new(var, ...);
void delete(var);

/** Lit - Literatal value. 
*
* Can use "lit" macro or the "$" sign.
*
* var hello = $(String, "Hello");
*
* This is a way to declare rich data on the stack.
* Good for avoiding new/delete management & overhead.
*
*   !! Does not call constructors/destructors.
*   !! Simply copies arguments as if they were data members.
*   !! Assumes that the data object for a type is named as such:
*   !!   Int => IntData
*
*/

#define $ lit
#define lit(T, ...) (T##Data[]){{T, __VA_ARGS__}}


/** Copy - copyable */

class {
  var (*copy)(var);
} Copy;

var copy(var);

/** Eq - equality */

class {
  bool (*eq)(var,var);
} Eq;

bool eq(var, var);
bool neq(var, var);

/** Ord - ordering */

class {
  bool (*gt)(var,var);
  bool (*lt)(var,var);
} Ord;

bool gt(var, var);
bool lt(var, var);
bool ge(var, var);
bool le(var, var);

/** Hash - hashable */

class {
  long (*hash)(var);
} Hash;

long hash(var);

/** Collection - contains objects */

class {
  int (*len)(var);
  bool (*is_empty)(var);
  void (*clear)(var);
  bool (*contains)(var, var);
  void (*discard)(var, var);
} Collection;

int len(var);
bool is_empty(var);
void clear(var);
bool contains(var, var);
void discard(var, var);

/** Iter - iterable */

class {
  var (*iter_start)(var);
  var (*iter_end)(var);
  var (*iter_next)(var, var);
} Iter;

var iter_start(var);
var iter_end(var);
var iter_next(var,var);

#define foreach(xs, x) for(var x = iter_start(xs); x != iter_end(xs); x = iter_next(xs, x))

/** Push - insertion and removal abilities */

class {
  void (*push)(var, var);
  void (*push_at)(var, var, int);
  void (*push_back)(var, var);
  void (*push_front)(var, var);
  
  var (*pop)(var);
  var (*pop_at)(var, int);
  var (*pop_back)(var);
  var (*pop_front)(var);
} Push;

void push(var, var);
void push_at(var, var, int);
void push_back(var, var);
void push_front(var, var);

var pop(var);
var pop_at(var, int);
var pop_back(var);
var pop_front(var);

/** At - positional access */

class {
  var (*at)(var, int);
  void (*set)(var, int, var);
} At;

var at(var, int);
void set(var, int, var);

/** Dict - dictionary access */

class {
  var (*get)(var, var);
  void (*put)(var, var, var);
} Dict;

var get(var, var);
void put(var, var, var);

/** AsChar - as C char */

class {
  char (*as_char)(var);
} AsChar;

char as_char(var);

/** AsStr - as C string */

class {
  const char* (*as_str)(var);
} AsStr;

const char* as_str(var);

/** AsLong - as C long */

class {
  long (*as_long)(var);
} AsLong;

long as_long(var);

/** AsDouble - as C double */

class {
  double (*as_double)(var);
} AsDouble;

double as_double(var);



#endif