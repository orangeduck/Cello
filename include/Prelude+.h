#ifndef PreludePlus_h
#define PreludePlus_h

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

/*
** Syntax
**
**    New reserved keywords:
**
**    var, is, not, elif, module, class, data, instance, 
**    methods, method, methods_begin, methods_end,
**    lit, $, foreach, 
*/

typedef void* var;

#define is ==
#define not !
#define elif else if

#define module extern Type

#define class typedef struct 
#define data typedef struct 

#define instance(T,C) static const C T##C

#define methods (Type)(TypeMethod[])
#define methods_begin(T) {"__TypeName", #T}
#define method(T,C) {#C, &T##C}
#define methods_end(T) {NULL, NULL}

#define methods_table(T) T##Methods

/*
** Types
**    
**    A list of named class instances ending in NULL entries.
*/

typedef struct {
  const char* class_name;
  const void* class_object;
} TypeMethod;

typedef const TypeMethod* Type;

#define type_implements(T, C) type_implements_name(T, #C)
#define type_class(T, C) type_class_name(T, #C)

bool type_implements_name(Type, const char* class_name);
var type_class_name(Type, const char* class_name);

const char* type_name(Type);

/*
** Data
**
**    All data objects must start with "Type" member.
*/

data {
  Type type;
} ObjectData;

Type type_of(var);

#define cast(X, T) type_cast(X, T, __func__)
var type_cast(var, Type t, const char*);

/*
** Classes
**
**    Sets of functions which work upon objects.
*/

/** New - heap allocation with constructor/destructor */

class {
  size_t size;
  void (*ctor)(var, va_list*);
  void (*dtor)(var);
} New;

var new(Type, ...);
void delete(var);

/** Lit - Literatal value. 
*
* Can use "lit" macro or the "$" sign.
*
* This is a way to declare rich data on the stack.
* Huge benefit in avoiding new/delete management & overhead.
*
* Warning: does not call constructors/destructors.
*   Simply copies arguments as if they were data members.
*   Assumes that the data object for a type is named as such:
*   
*   Int <=> IntData
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


/** Len - countable length */

class {
  size_t (*len)(var);
} Len ;

size_t len(var);

/** Contains - contains object */

class {
  bool (*contains)(var, var);
  void (*erase)(var, var);
} Contains;

bool contains(var, var);
void erase(var, var);

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

var sum(var);
var product(var);
var sorted(var);

var map(var, void f(var));
var filter(var, bool f(var));

/** At - positional access */

class {
  var (*at)(var, int);
  void (*set)(var, int, var);
} At;

var at(var, int);
void set(var, int, var);

/** Empty - can be empty and cleared */

class {
  bool (*is_empty)(var);
  void (*clear)(var);
} Empty;

bool is_empty(var);
void clear(var);

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


/** Hash - convert to a hash */

class {
  long (*hash)(var);
} Hash;

long hash(var);

/** Dict - access as a dictionary */

class {
  var (*get)(var, var);
  void (*put)(var, var, var);
} Dict;

var get(var, var);
void put(var, var, var);

/** AsChar - as C char */

class {
  char (*as_char)(var);
} AsChr;

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

/** Show - uses the above to print formatted strings */

void show(var);
void showf(const char*, ...);

void fshow(FILE*, var);
void fshowf(FILE*, const char*, ...);

void sshow(char*, var);
void sshowf(char*, const char*, ...);

void vfshow(FILE*, var);
void vfshowf(FILE*, const char*, va_list);

void vsshow(char*, var);
void vsshowf(char*, const char*, va_list);

#endif