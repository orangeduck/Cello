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

#ifndef CELLO_H
#define CELLO_H

/* Settings */

#ifdef  CELLO_NDEBUG
#define CELLO_BOUND_CHECK  0
#define CELLO_MAGIC_CHECK  0
#define CELLO_ALLOC_CHECK  0
#define CELLO_UNDEF_CHECK  0
#define CELLO_METHOD_CHECK 0
#define CELLO_MEMORY_CHECK 0
#else
#define CELLO_BOUND_CHECK  1
#define CELLO_MAGIC_CHECK  1
#define CELLO_ALLOC_CHECK  1
#define CELLO_UNDEF_CHECK  1
#define CELLO_METHOD_CHECK 1
#define CELLO_MEMORY_CHECK 1
#endif

#ifndef CELLO_GC
#define CELLO_GC 1
#define CELLO_GC_HEADER NULL, NULL, NULL,
#else
#define CELLO_GC 0
#define CELLO_GC_HEADER
#endif

#ifndef CELLO_MAGIC
#define CELLO_MAGIC 1
#define CELLO_MAGIC_HEADER ((var)0xCe110),
#else
#define CELLO_MAGIC 0
#define CELLO_MAGIC_HEADER
#endif

#if defined(_WIN32)
#define CELLO_WINDOWS
#endif

#if defined(__unix__)
#define CELLO_UNIX
#define CELLO_LINUX
#endif

#if defined(__APPLE__)
#define CELLO_UNIX
#define CELLO_MAC
#endif

#if defined(__clang__)
#define CELLO_CLANG
#endif

#if defined(__GNUC__)
#define CELLO_GCC
#endif

/* Includes */

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

#ifdef CELLO_WINDOWS
#include <windows.h>
#include <DbgHelp.h>
#endif

#ifdef CELLO_UNIX
#include <pthread.h>
#include <execinfo.h>
#endif

/* Syntax */

typedef void* var;

#define is ==
#define isnt !=
#define not !
#define and &&
#define or ||
#define in ,

/* Declaration */

#define Cello(T, ...) (var)((var[]){ \
  NULL, (var)CelloStaticAlloc, \
  CELLO_GC_HEADER \
  CELLO_MAGIC_HEADER \
  NULL, "__Name",     #T, \
  NULL, "__Parent", NULL, \
  ##__VA_ARGS__, \
  NULL, NULL, NULL}) + \
  sizeof(struct CelloHeader) + (sizeof(T) - sizeof(T))

#define Member(I, ...) NULL, #I, &((struct I){__VA_ARGS__})
  
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
extern var List;
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
  CelloDataAlloc   = 0x08,
  CelloMarked      = 0x10,
  CelloRed         = 0x20
};

struct CelloHeader {
  var type;
  var flags;
#if CELLO_GC == 1
  var prev, next;
  var gcnext;
#endif
#if CELLO_MAGIC == 1
  var magic;
#endif
};

struct Type {
  var cls;
  var name;
  var inst;
};

static const var True  = (var)1;
static const var False = (var)0;
static const var Okay  = (var)1;
static const var None  = (var)0;

struct Ref { var val; };
struct Box { var val; };
struct Int { int64_t val; };
struct Char { char val; };
struct Float { double val; };
struct String { char* val; };
struct Tuple { var* items; };
struct Range { var iter; int64_t start; int64_t stop; int64_t step; };
struct Slice { var iter; var range; };
struct File { FILE* file; };

#ifdef CELLO_CLANG
struct Function { var (^func)(var); };
#else
struct Function { var (*func)(var); };
#endif
  
/* Classes */

extern var Doc;
extern var Help;
extern var Cast;
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
extern var Reserve;
extern var C_Char;
extern var C_Str;
extern var C_Int;
extern var C_Float;
extern var Begin;
extern var Stream;
extern var Pointer;
extern var Call;
extern var Format;
extern var Show;
extern var Math;
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

struct Cast {
  var (*cast)(var, var);
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
  void (*sort_with)(var,var);
};

struct Clear {
  void (*clear)(var);  
};

struct Reserve {
  void (*reserve)(var, var);
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

const char* name(var type);
const char* brief(var type);
const char* description(var type);
const char* examples(var type);
const char* methods(var type);

void help(var self);

var type_of(var obj);
var cast(var self, var type);
var instance(var self, var cls);
var implements(var self, var cls);
var type_instance(var self, var cls);
var type_implements(var self, var cls);

#define method(X, C, M, ...) \
  ((struct C*)method_at_offset(X, C, \
  offsetof(struct C, M), #M))->M(X, ##__VA_ARGS__)

#define implements_method(X, C, M) \
  implements_method_at_offset(X, C, offsetof(struct C, M))
  
#define type_method(T, C, M, ...) \
  ((struct C*)type_method_at_offset(T, C, \
  offsetof(struct C, M), #M))->M(__VA_ARGS__)
  
#define type_implements_method(T, C, M) \
  type_implements_method_at_offset(T, C, offsetof(struct C, M))
  
var method_at_offset(var self, var cls, size_t offset, const char* method);
var implements_method_at_offset(var self, var cls, size_t offset);

var type_method_at_offset(var self, var cls, size_t offset, const char* method);
var type_implements_method_at_offset(var self, var cls, size_t offset);

var CelloHeader_Init(struct CelloHeader* head, var type, int flags);
var CelloHeader_GetFlag(struct CelloHeader* head, int flag);
void CelloHeader_SetFlag(struct CelloHeader* head, int flag);
void CelloHeader_RemFlag(struct CelloHeader* head, int flag);

#define $(T, ...) alloc_stk(T, \
  ((char[sizeof(struct CelloHeader) + sizeof(struct T)]){}), \
  &((struct T){__VA_ARGS__}), sizeof(struct T))

#define $I(X) $(Int, X)
#define $F(X) $(Float, X)
#define $C(X) $(Char, X)
#define $S(X) $(String, X)
#define $R(X) $(Ref, X)
#define $B(X) $(Box, X)
#define $T(...) tuple(__VA_ARGS__)

#define new(T, ...) new_with(T, tuple(__VA_ARGS__))
#define new_$(T, ...) new(T, $(T, ##__VA_ARGS__))
var new_with(var type, var args);
void del(var self);

#define new_$I(X) new_$(Int, X)
#define new_$F(X) new_$(Float, X)
#define new_$C(X) new_$(Char, X)
#define new_$S(X) new_$(String, X)
#define new_$R(X) new_$(Ref, X)
#define new_$B(X) new_$(Box, X)
#define new_$T(...) new_with(Tuple, tuple(__VA_ARGS__))

#define tuple_new(_, ...) $(Tuple, (var[]){ __VA_ARGS__ })
#define tuple(...) tuple_new(_, ##__VA_ARGS__, Terminal)

var alloc(var type);
var alloc_stk(var type, var mem, var data, size_t size);
void dealloc(var self);
size_t size(var type);

#define construct(self, ...) construct_with(self, tuple(__VA_ARGS__))
var construct_with(var self, var args);
var destruct(var self);

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
#define foreach_in(X, S) for( var \
  __##X = (S), \
  __Iter##X = instance(__##X, Iter), \
  X = ((struct Iter*)(__Iter##X))->iter_init(__##X); \
  X isnt Terminal; \
  X = ((struct Iter*)(__Iter##X))->iter_next(__##X, X))

void push(var self, var obj);
void pop(var self);
void push_at(var self, var obj, var key);
void pop_at(var self, var key);

void reverse(var self);
void sort(var self);
void sort_with(var self, var func);

var get(var self, var key);
void set(var self, var key, var val);
var mem(var self, var key);
void rem(var self, var key);

void reserve(var self, var amount);
void clear(var self);

size_t len(var self);
var empty(var self);

char c_char(var self);
char* c_str(var self);
int64_t c_int(var self);
double c_float(var self);

void begin(var self);
void end(var self);

var begin_with(var self);
var end_with(var self);

#define range(X) $(Range, $(Int, 0), 0, X, 1)
#define range_from(X, Y) $(Range, $(Int, 0), X, Y, 1)
#define range_step(X, Y, Z) $(Range, $(Int, 0), X, Y, Z)

#define with(X) with_in(X)
#define with_in(X, S) \
  for(var X = begin_with(S); X isnt Terminal; X = end_with(X))

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

#ifdef CELLO_CLANG

#define fun(X, A) \
  struct Function* X = $(Function, None); \
  X->func = ^ var (var A)
  
#else

#define fun(X, A) \
  auto var __CelloFunction_##X(var); \
  var X = $(Function, __CelloFunction_##X); \
  var __CelloFunction_##X(var A)
  
#endif

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
#define print_to(out, pos, fmt, ...) \
  print_to_with(out, pos, fmt, tuple(__VA_ARGS__))

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

#define try \
  exception_inc(); exception_deactivate(); if (!setjmp(exception_buffer()))   

#define catch_in(X, ...) else { exception_activate(); } exception_dec(); \
  for (var X = exception_catch(tuple(__VA_ARGS__)); \
    X isnt Terminal; X = Terminal)

#define catch(...) catch_in(__VA_ARGS__)
    
#define throw(E, F, ...) \
  exception_throw(E, F, tuple(__VA_ARGS__))

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
