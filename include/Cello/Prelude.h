#ifndef CelloPrelude_h
#define CelloPrelude_h

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <setjmp.h>

/*
** == Syntax ==
**
**  New reserved keywords:
**
**  var is not and or elif in
**  local global class data instance
**  foreach with try throw catch
**  $ lit
*/

typedef void* var;
#define is ==
#define isnt !=
#define not !
#define and &&
#define or ||
#define elif else if
#define in ,

#define local static
#define global extern
#define class typedef struct 
#define data typedef struct 
#define instance(T,C) local C T##C

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
** == Singleton ==
**
**  Create a empty type object.
*/

#define Singleton(T) methods { methods_begin(T), methods_end(T) }


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
** == Undefined ==
**
**  Undefined or errorous data value.
*/

global var Undefined;


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
**
**  == var_list ==
**
**  This is a set of macros that provide
**  a method for 'variable argument' like
**  behaviour where all arguments to a function
**  must be of type `var`.
**
**  Another advantage is that it provides a method
**  to tell when no arguments are left.
**
**  VAR_LIST must always be used like this:
**
**    VAR_LIST(_, ##__VA_ARGS__, Undefined)
**
**  Otherwise correct behaviour cannot be ensured
*/

typedef var* var_list;

#define var_list_parse(_, ...) (var[]){ __VA_ARGS__ }

#define var_list_new(...) var_list_parse(_, ##__VA_ARGS__, Undefined)
#define var_list_end(vl) (*vl is Undefined)
#define var_list_get(vl) (*vl++)


/*
** == Class ==
**
**  Sets of functions to act upon data objects.
**  Can be seen as interfaces or signatures.
*/


/** New - heap allocation & constructor/destructor */

class {
  var (*construct)(var, va_list*);
  var (*destruct)(var);
  size_t (*size)(void);
} New;

var new(var type, ...);
void delete(var obj);

var allocate(var type);
void deallocate(var obj);

size_t size(var type);
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
  var (*eq)(var,var);
} Eq;

var eq(var self, var obj);
var neq(var self, var obj);

#define if_eq(X,Y) if(eq(X,Y))
#define if_neq(X,Y) if(neq(X,Y))

/** Ord - ordering */

class {
  var (*gt)(var,var);
  var (*lt)(var,var);
} Ord;

var gt(var self, var obj);
var lt(var self, var obj);
var ge(var self, var obj);
var le(var self, var obj);

#define if_lt(X,Y) if(lt(X,Y))
#define if_gt(X,Y) if(gt(X,Y))
#define if_le(X,Y) if(le(X,Y))
#define if_ge(X,Y) if(ge(X,Y))

/** Hash - hashable */

class {
  long (*hash)(var);
} Hash;

long hash(var obj);

/** Collection - contains objects */

class {
  int (*len)(var);
  void (*clear)(var);
  var (*contains)(var, var);
  void (*discard)(var, var);
} Collection;

int len(var col);
void clear(var col);
var contains(var col, var obj);
void discard(var col, var obj);

var is_empty(var col);

var maximum(var self);
var minimum(var self);

/** Sort - can be sorted */

class {
  void (*sort)(var);
} Sort;

void sort(var self);

/** Reverse - can be revered */

class {
  void (*reverse)(var);
} Reverse;

void reverse(var self);

/** Append - can be appended to */

class {
  void (*append)(var, var);
} Append;

void append(var self, var obj);

/** Iter - iterable */

class {
  var (*iter_start)(var);
  var (*iter_end)(var);
  var (*iter_next)(var, var);
} Iter;

var iter_start(var col);
var iter_end(var col);
var iter_next(var col, var curr);

#define foreach(x) foreach_scanned(x)
#define foreach_scanned(x, xs) for(var x = iter_start(xs); x != iter_end(xs); x = iter_next(xs, x))

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


/** With - perform command on exit/enter of block */

class {
  void (*enter)(var);
  void (*exit)(var);
} With;

void enter_with(var self);
void exit_with(var self);

var enter_for(var self);
var exit_for(var self);

#define with(x) with_scanned(x)
#define with_scanned(x, ...) for(var x = enter_for(__VA_ARGS__); not (x is Undefined); x = exit_for(x))

/** Stream - File like object */

class {
  var (*stream_open)(var,const char*,const char*);
  void (*stream_close)(var);
  void (*stream_seek)(var,int,int);
  int (*stream_tell)(var);
  void (*stream_flush)(var);
  bool (*stream_eof)(var);
  int (*stream_read)(var,void*,int);
  int (*stream_write)(var,void*,int);
} Stream;

var stream_open(var self, const char* name, const char* access);
void stream_close(var self);
void stream_seek(var self, int pos, int origin);
int stream_tell(var self);
void stream_flush(var self);
bool stream_eof(var self);
int stream_read(var self, void* output, int size);
int stream_write(var self, void* input, int size);

/** Serialize - Serializable to Stream */

class {
  void (*serial_read)(var,var);
  void (*serial_write)(var,var);
} Serialize;

void serial_read(var self, var input);
void serial_write(var self, var output);

#endif
