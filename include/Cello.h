/*
**         $$===================================================$$
**         ||                                                   ||
**         ||          ,;,                ___     _ _           ||
**         ||         +($)+              / __|___| | |___       ||
**         ||         +($)+             | (__/ -_) | / _ \      ||
**         ||          |||           lib \___\___|_|_\___/      ||
**         ||          |||                                      ||
**         ||          |||    /#           High Level           ||
**         ||         _|||_  /'         Programming in C        ||
**         ||       .' ||| '/                                   ||
**         ||      /   ||| / \        http://libcello.org/      ||
**         ||      |   |||/  |                                  ||
**         ||      )_  ||/  _(         Licensed under BSD       ||
**         ||       _) |/| (_                                   ||
**         ||      )   /||   (                                  ||
**         ||     /  }/+++ {  \                                 ||
**         ||    |  {/ |||  }  |                                ||
**         ||    |  /  ~~~     |                                ||
**         ||     \/   \ /    /                                 ||
**         ||     /`-.__Y__.-`                                  ||
**         ||    '-     |              Daniel Holden            ||
**         ||           |                                       ||
**         ||           !      contact@theorangeduck.com        ||
**         ||                                                   ||
**         ||       https://github.com/orangeduck/libCello      ||
**         ||                                                   ||
**         $$===================================================$$
*/

#ifndef Cello_h
#define Cello_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <DbgHelp.h>
#endif

#if defined(__unix__) || defined(__APPLE__)
#include <pthread.h>
#include <execinfo.h>
#endif

/* Settings */

#ifndef CELLO_BOUNDS_CHECK
#define CELLO_BOUNDS_CHECK 1
#endif

/* Syntax */

typedef void* var;

#define is ==
#define isnt !=
#define not !
#define and &&
#define or ||
#define in ,

#define typedecl(T, ...) (var)((var[]){ \
  NULL, (var)CelloStaticAlloc, \
  NULL, "__Name",     #T, \
  NULL, "__Parent", NULL, \
  ##__VA_ARGS__, \
  NULL, NULL, NULL}) + sizeof(struct CelloHeader) + sizeof(T) - sizeof(T)

#define typeclass(I, ...) NULL, #I, &((struct I){__VA_ARGS__})

#define $(T, ...) new_stk(T, \
  ((char[sizeof(struct CelloHeader) + sizeof(struct T)]){}), \
  &((struct T){__VA_ARGS__}), sizeof(struct T))

#define try \
  exception_inc(); exception_deactivate(); if (!setjmp(exception_buffer()))   

#define catch_in(X, ...) else { exception_activate(); } exception_dec(); \
  for (var X = exception_catch(tuple(__VA_ARGS__)); \
    X isnt Terminal; X = Terminal)

#define catch(...) catch_in(__VA_ARGS__)
    
#define throw(E, F, ...) \
  exception_throw(E, F, tuple(__VA_ARGS__))
  
#if defined(__clang__)

#define function(N, A) \
  struct Function* N = $(Function, None); \
  N->func = ^ var (var A)
  
#else

#define function(N, A) \
  auto var __CelloFunction_##N(var); \
  var N = $(Function, __CelloFunction_##N); \
  var __CelloFunction_##N(var A)
  
#endif
  
/* Types */

extern var Type;
extern var Bool;
extern var Tuple;
extern var Terminal;
extern var Undefined;

extern var Ref;
extern var Box;
extern var Int;
extern var Char;
extern var Float;
extern var String;

extern var Map;
extern var Array;
extern var Table;
extern var Range;
extern var Slice;

extern var File;
extern var Mutex;
extern var Thread;
extern var Function;

extern var IOError;
extern var KeyError;
extern var BusyError;
extern var TypeError;
extern var ValueError;
extern var ClassError;
extern var FormatError;
extern var ResourceError;
extern var OutOfMemoryError;
extern var IndexOutOfBoundsError;

extern var SegmentationError;
extern var ProgramAbortedError;
extern var DivisionByZeroError;
extern var IllegalInstructionError;
extern var ProgramInterruptedError;
extern var ProgramTerminationError;

/* Data */

enum {
  CelloStaticAlloc = 0x01,
  CelloStackAlloc  = 0x02,
  CelloHeapAlloc   = 0x04,
  CelloDataAlloc   = 0x08
};

struct CelloHeader {
  var type;
  var flags;
};

struct Type {
  var cls;
  var name;
  var inst;
};

static var True  = (var)1;
static var False = (var)0;
static var Okay  = (var)1;
static var None  = (var)0;

struct Ref { var val; };
struct Box { var val; };
struct Int { int64_t val; };
struct Char { char val; };
struct Float { double val; };
struct String { char* val; };
struct Tuple { var* items; };
struct Range { var start; var stop; var step; };
struct Slice { var iter; var range; };
struct File { FILE* file; };

#if defined(__clang__)
struct Function { var (^func)(var); };
#else
struct Function { var (*func)(var); };
#endif
  
/* Classes */

extern var Doc;
extern var Help;
extern var New;
extern var Assign;
extern var Copy;
extern var Traverse;
extern var Eq;
extern var Ord;
extern var Hash;
extern var Len;
extern var Iter;
extern var Push;
extern var Concat;
extern var Get;
extern var Reverse;
extern var Sort;
extern var Clear;
extern var Find;
extern var C_Char;
extern var C_Str;
extern var C_Int;
extern var C_Float;
extern var Begin;
extern var Stream;
extern var Pointer;
extern var Math;
extern var Call;
extern var Format;
extern var Show;
extern var Current;
extern var Start;
extern var Join;
extern var Lock;

/* Signatures */

struct Doc {
  const char* (*name)(void);
  const char* (*brief)(void);
  const char* (*description)(void);
  const char* (*examples)(void);
  const char* (*methods)(void);
};

struct Help {
  void (*help)(var);
};

struct New {
  var (*construct_with)(var, var);
  var (*destruct)(var);
  size_t (*size)(void);
};

struct Assign {
  var (*assign)(var, var);
};

struct Copy {
  var (*copy)(var);
};

struct Traverse {
  void (*traverse)(var, var);
};

struct Eq {
  var (*eq)(var, var);
};

struct Ord {
  var (*gt)(var, var);
  var (*lt)(var, var);
};

struct Hash {
  uint64_t (*hash)(var);
};

struct Len {
  size_t (*len)(var);
};

struct Push {
  void (*push)(var, var);
  void (*pop)(var);
  void (*push_at)(var, var, var);
  void (*pop_at)(var, var);
};

struct Concat {
  void (*concat)(var, var);
  void (*append)(var, var);
};

struct Get {
  var  (*get)(var, var);
  void (*set)(var, var, var);
  var  (*mem)(var, var);
  void (*rem)(var, var);
};

struct Iter {
  var (*iter_init)(var);
  var (*iter_next)(var, var);
  var (*iter_prev)(var, var);
};

struct Reverse {
  void (*reverse)(var);
};

struct Sort {
  void (*sort)(var);
};

struct Clear {
  void (*clear)(var);  
};

struct Find {
  var (*find)(var, var);
};

struct C_Char {
  char (*c_char)(var);
};

struct C_Str {
  char* (*c_str)(var);
};

struct C_Int {
  int64_t (*c_int)(var);
};

struct C_Float {
  double (*c_float)(var);
};

struct Begin {
  void (*begin)(var);
  void (*end)(var);
};

struct Stream {
  var  (*sopen)(var, var, var);
  void (*sclose)(var);
  void (*sseek)(var, var, var);
  int  (*stell)(var);
  void (*sflush)(var);
  var  (*seof)(var);
  int  (*sread)(var, var, var);
  int  (*swrite)(var, var, var);
};

struct Pointer {
  void (*ref)(var, var);
  var (*deref)(var);
};

struct Math {
  void (*madd)(var, var);
  void (*msub)(var, var);
  void (*mmul)(var, var);
  void (*mdiv)(var, var);
  void (*mpow)(var, var);
  void (*mmod)(var, var);
  void (*mneg)(var);
  void (*mabs)(var);
  void (*mexp)(var);
};

struct Call {
  var (*call_with)(var, var);
};

struct Format {
  int (*format_to)(var, int, const char*, va_list);
  int (*format_from)(var, int, const char*, va_list);
};

struct Show {
  int (*show)(var, var, int);
  int (*look)(var, var, int);
};

struct Current {
  var (*current)(void);
};

struct Start {
  void (*start)(var);
  void (*stop)(var);
  var (*running)(var);
};

struct Join {
  void (*join)(var);
};

struct Lock {
  void (*lock)(var);
  void (*unlock)(var);
  var  (*lock_try)(var);
};

/* Functions */

void help(var self);

var type_of(var obj);
var cast(var self, var type);
var cast_exact(var self, var type);
var instance(var self, var cls);
var implements(var self, var cls);
var type_instance(var self, var cls);
var type_implements(var self, var cls);

#define method(X, C, M, ...) \
  ((struct C*)method_at_offset(X, C, \
  offsetof(struct C, M), #M))->M(X, ##__VA_ARGS__)

#define implements_method(X, C, M) \
  implements_method_at_offset(X, C, offsetof(struct C, M), #M)
  
#define type_method(T, C, M, ...) \
  ((struct C*)type_method_at_offset(T, C, \
  offsetof(struct C, M), #M))->M(__VA_ARGS__)
  
#define type_implements_method(T, C, M) \
  type_implements_method_at_offset(T, C, offsetof(struct C, M), #M)
  
var method_at_offset(var self, var cls, size_t offset, const char* method);
var implements_method_at_offset(
  var self, var cls, size_t offset, const char* method);

var type_method_at_offset(var self, var cls, size_t offset, const char* method);
var type_implements_method_at_offset(
  var self, var cls, size_t offset, const char* method);

#define new(T, ...) new_with(T, tuple(__VA_ARGS__))
#define new_$(T, ...) new(T, $(T, ##__VA_ARGS__))
var new_with(var type, var args);
void del(var self);
var new_stk(var type, var mem, var data, size_t size);

var alloc(var type);
void dealloc(var self);
size_t size(var type);

#define construct(self, ...) construct_with(self, tuple(__VA_ARGS__))
var construct_with(var self, var args);
var destruct(var self);

#define tuple_new(_, ...) $(Tuple, (var[]){ __VA_ARGS__ })
#define tuple(...) tuple_new(_, ##__VA_ARGS__, Terminal)

var assign(var self, var obj);
var copy(var obj);

void traverse(var self, var f);

var eq(var self, var obj);
var neq(var self, var obj);

#define if_eq(X,Y) if(eq(X,Y))
#define if_neq(X,Y) if(neq(X,Y))

var gt(var self, var obj);
var lt(var self, var obj);
var ge(var self, var obj);
var le(var self, var obj);

#define if_lt(X,Y) if(lt(X,Y))
#define if_gt(X,Y) if(gt(X,Y))
#define if_le(X,Y) if(le(X,Y))
#define if_ge(X,Y) if(ge(X,Y))

var bool_var(intptr_t x);

void hash_init(uint64_t seed);
uint64_t hash_seed(void);
uint64_t hash(var obj);

var iter_init(var self);
var iter_next(var self, var curr);

#define foreach(X) foreach_in(X)
#define foreach_in(X, S) \
  for(var X = iter_init(S); X isnt Terminal; X = iter_next(S, X))

void push(var self, var obj);
void pop(var self);
void push_at(var self, var obj, var key);
void pop_at(var self, var key);

void reverse(var self);
void sort(var self);

var get(var self, var key);
void set(var self, var key, var val);
var mem(var self, var key);
void rem(var self, var key);

var find(var self, var val);
void clear(var self);

size_t len(var self);
var empty(var self);

var maximum(var self);
var minimum(var self);

char c_char(var self);
char* c_str(var self);
int64_t c_int(var self);
double c_float(var self);

void begin(var self);
void end(var self);

var begin_with(var self);
var end_with(var self);

#define range(X) $(Range, $(Int, 0), $(Int, X), $(Int, 1))
#define range_from(X, Y) $(Range, $(Int, X), $(Int, Y), $(Int, 1))
#define range_step(X, Y, Z) $(Range, $(Int, X), $(Int, Y), $(Int, Z))

#define with(X) with_in(X)
#define with_in(X, S) \
  for(var X = begin_with(S); X isnt Terminal; X = end_with(X))

#define file() $(File, None)
  
var sopen(var self, var filename, var access);
void sclose(var self);
void sseek(var self, var pos, var origin);
int stell(var self);
void sflush(var self);
var seof(var self);
int sread(var self, var output, var size);
int swrite(var self, var input, var size);

void ref(var self, var item);
var deref(var self);

var forward(var self);

void minc(var self);
void mdec(var self);
void mneg(var self);
void mabs(var self);
void mexp(var self);
void madd(var self, var obj);
void msub(var self, var obj);
void mmul(var self, var obj);
void mdiv(var self, var obj);
void mpow(var self, var obj);
void mmod(var self, var obj);
void minc(var self);
void mdec(var self);

#define call(x, ...) call_with(x, tuple(__VA_ARGS__))
var call_with(var self, var args);

void map(var self, var func);

int format_to_va(var self, int pos, const char* fmt, va_list va);
int format_from_va(var self, int pos, const char* fmt, va_list va);

int format_to(var self, int pos, const char* fmt, ...);
int format_from(var self, int pos, const char* fmt, ...);

int show(var self);
int show_to(var self, var out, int pos);

#define print(fmt, ...) print_with(fmt, tuple(__VA_ARGS__))
#define println(fmt, ...) println_with(fmt, tuple(__VA_ARGS__))
#define print_to(out, pos, fmt, ...) print_to_with(out, pos, fmt, tuple(__VA_ARGS__))

int print_with(const char* fmt, var args);
int println_with(const char* fmt, var args);
int print_to_with(var out, int pos, const char* fmt, var args);

int look(var self);
int look_from(var self, var input, int pos);

#define scan(fmt, ...) scan_with(fmt, tuple(__VA_ARGS__))
#define scanln(fmt, ...) scanln_with(fmt, tuple(__VA_ARGS__))
#define scan_from(input, pos, fmt, ...) \
  scan_from_with(input, pos, fmt, tuple(__VA_ARGS__))

int scan_with(const char* fmt, var args);
int scanln_with(const char* fmt, var args);
int scan_from_with(var input, int pos, const char* fmt, var args);

var current(var type);

void start(var self);
void stop(var self);
var running(var self);

void join(var self);

void lock(var self);
void unlock(var self);
var lock_try(var self);

void exception_register_signals(void);
void exception_inc(void);
void exception_dec(void);
var exception_active(void);
void exception_activate(void);
void exception_deactivate(void);

var exception_object(void);
var exception_message(void);
size_t exception_depth(void);
var exception_buffer(void);

var exception_throw(var obj, const char* fmt, var args);
var exception_catch(var args);

#endif
