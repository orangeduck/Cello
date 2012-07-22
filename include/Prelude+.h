#ifndef PreludePlus_h
#define PreludePlus_h

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>


/*
** == Syntax ==
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
** == Methods ==
**
**  These macros make the declaration 
**  of Types statically much easier
*/

#define methods (TypeData*)(const TypeData[])
#define methods_begin(T) {NULL, "__Type"}, {#T, "__Name"}
#define method(T,C) {&T##C, #C}
#define methods_end(T) {NULL, NULL}


/*
** == Data ==
**
**  All data definitions must start with "type" member.
**  If this is not respected functions taking data objects will crash.
*/

data {
  var type;
} ObjectData;

var type_of(var obj);


/*
** == Cast ==
**
**  Perform run-time type check for object.
**  Ensure object X is of certain type T.
*/

#define cast(X, T) Type_Cast(X, T, __func__)


/* 
** == Lit ==
**
**  Declare data objects on the stack.
**  Good for avoiding new/delete management.
**  Can use "lit" macro or the "$" sign.
**
**    var hello = $(String, "Hello");
**
**  !! Does not call constructors/destructors.
**  !! Simply copies arguments as if they were data members.
**  !! Assumes that the data object for a type is named as such:
**  !!   Int => IntData
**
*/

#define $ lit
#define lit(T, ...) (T##Data[]){{T, __VA_ARGS__}}


/*
** == Class ==
**
**  Sets of functions to act upon data objects.
**  Can be seen as interfaces or signatures.
*/


/** New - heap allocation with constructor/destructor */

class {
  size_t size;
  var (*construct)(var, va_list*);
  var (*destruct)(var);
} New;

var new(var type, ...);
void delete(var obj);

var construct(var obj, ...);
var destruct(var obj);

/** Assign - assignment (copy constructor) */

class {
  void (*assign)(var, var);
} Assign;

void assign(var self, var obj);

/** Copy - copyable */

class {
  var (*copy)(var);
} Copy;

var copy(var obj);

/** Eq - equality */

class {
  bool (*eq)(var,var);
} Eq;

bool eq(var self, var obj);
bool neq(var self, var obj);

/** Ord - ordering */

class {
  bool (*gt)(var,var);
  bool (*lt)(var,var);
} Ord;

bool gt(var self, var obj);
bool lt(var self, var obj);
bool ge(var self, var obj);
bool le(var self, var obj);

/** Hash - hashable */

class {
  long (*hash)(var);
} Hash;

long hash(var obj);

/** Collection - contains objects */

class {
  int (*len)(var);
  bool (*is_empty)(var);
  void (*clear)(var);
  bool (*contains)(var, var);
  void (*discard)(var, var);
} Collection;

int len(var col);
bool is_empty(var col);
void clear(var col);
bool contains(var col, var obj);
void discard(var col, var obj);

/** Iter - iterable */

class {
  var (*iter_start)(var);
  var (*iter_end)(var);
  var (*iter_next)(var, var);
} Iter;

var iter_start(var col);
var iter_end(var col);
var iter_next(var col, var curr);

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

void push(var col, var obj);
void push_at(var col, var obj, int i);
void push_back(var col, var obj);
void push_front(var col, var obj);

var pop(var col);
var pop_at(var col, int i);
var pop_back(var col);
var pop_front(var col);

/** At - positional access */

class {
  var (*at)(var, int);
  void (*set)(var, int, var);
} At;

var at(var col, int i);
void set(var col, int i, var obj);

/** Dict - dictionary access */

class {
  var (*get)(var, var);
  void (*put)(var, var, var);
} Dict;

var get(var col, var key);
void put(var col, var key, var val);

/** AsChar - as C char */

class {
  char (*as_char)(var);
} AsChar;

char as_char(var obj);

/** AsStr - as C string */

class {
  const char* (*as_str)(var);
} AsStr;

const char* as_str(var obj);

/** AsLong - as C long */

class {
  long (*as_long)(var);
} AsLong;

long as_long(var obj);

/** AsDouble - as C double */

class {
  double (*as_double)(var);
} AsDouble;

double as_double(var obj);



#endif