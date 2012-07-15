#ifndef PlusPrelude_h
#define PlusPrelude_h

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

/*
** Syntax
**
**    New reserved keywords:
**
**    var, is, not, elif, foreach, class, object, instance, 
**    methods, method, method_begin, method_end
*/

typedef void* var;

#define is ==
#define not !
#define elif else if

#define class typedef struct 
#define object typedef struct 

#define instance(T,C) static const C T##C

#define methods(T) static const TypeMethod T##Methods[] 
#define methods_begin(T) {"__TypeName", #T}
#define method(T,C) {#C, &T##C}
#define methods_end(T) {NULL, NULL}

#define methods_table(T) T##Methods

/*
** Types
**    
**    A list of class implementations.
*/

typedef struct {
  const char* class_name;
  const void* class_object;
} TypeMethod;

typedef const TypeMethod* Type;

#define type_implements(T, C) type_implements_name(T, #C)
#define type_class(T, C) type_class_name(T, #C)

const char* type_name(Type);
bool type_implements_name(Type, const char* class_name);
var type_class_name(Type, const char* class_name);

/*
** Object
**
**    All objects must start with "Type" member.
*/

object {
  Type type;
} ObjectObject;

Type type_of(var);

#define cast(X, T) type_cast(X, T, __func__)
var type_cast(var, Type t, const char*);

/*
** Classes
**
**    Sets of functions which work on Objects.
*/

/** New - constructor/destructor */

class {
  size_t size;
  void (*ctor)(var, va_list*);
  void (*dtor)(var);
} New;

var new(Type, ...);
void delete(var);

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


/** Len - countable length */

class {
  size_t (*len)(var);
} Len ;

size_t len(var);

/** Contains - collection contains items */

class {
  bool (*contains)(var, var);
} Contains;

bool contains(var, var);

/** Iter - iterable collection */

class {
  var (*iter_start)(var);
  var (*iter_end)(var);
  var (*iter_next)(var, var);
} Iter;

var iter_start(var);
var iter_end(var);
var iter_next(var,var);

#define foreach(xs, x) for(var x = iter_start(xs); x != iter_end(xs); x = iter_next(xs, x))

var sum(var);
var product(var);
var sorted(var);

var map(var, void f(var));
var filter(var, bool f(var));

/** At - access as a list */

class {
  var (*at)(var, int);
  void (*set)(var, int, var);
} At;

var at(var, int);
void set(var, int, var);

/** Empty - can be empty collection */

class {
  bool (*is_empty)(var);
  void (*clear)(var);
} Empty;

bool is_empty(var);
void clear(var);

/** Push - push and pop access */

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


/** Hash - convert to a hash */

class {
  int (*hash)(var);
} Hash;

int hash(var);

/** Dict - access as a dict */

class {
  var (*get)(var, const char*);
  void (*put)(var, const char*, var);
} Dict;

var get(var, const char*);
void put(var, const char*, var);


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

/** Show - formatting & printing */

void show(var);
void showf(const char*, ...);

#endif