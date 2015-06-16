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
#define CELLO_NULL_CHECK   0
#define CELLO_METHOD_CHECK 0
#define CELLO_MEMORY_CHECK 0
#else
#define CELLO_BOUND_CHECK  1
#define CELLO_MAGIC_CHECK  1
#define CELLO_ALLOC_CHECK  1
#define CELLO_NULL_CHECK   1
#define CELLO_METHOD_CHECK 1
#define CELLO_MEMORY_CHECK 1
#endif

#if CELLO_ALLOC_CHECK == 1
#define CELLO_ALLOC_HEADER (var)AllocStatic,
#else
#define CELLO_ALLOC_HEADER
#endif

#if CELLO_MAGIC_CHECK == 1
#define CELLO_MAGIC_NUM 0xCe110
#define CELLO_MAGIC_HEADER ((var)CELLO_MAGIC_NUM),
#else
#define CELLO_MAGIC_HEADER
#endif

#ifndef CELLO_CACHE
#define CELLO_CACHE 1
#define CELLO_CACHE_HEADER \
  NULL, NULL, NULL, NULL, NULL, NULL, \
  NULL, NULL, NULL, NULL, NULL, NULL, \
  NULL, NULL, NULL, NULL, NULL, NULL,
#define CELLO_CACHE_NUM 18
#else
#define CELLO_CACHE 0
#define CELLO_CACHE_HEADER
#define CELLO_CACHE_NUM 0
#endif

#ifdef _WIN32
#define CELLO_WINDOWS
#endif

#ifdef __unix__
#define CELLO_UNIX
#define CELLO_LINUX
#endif

#ifdef __APPLE__
#define CELLO_UNIX
#define CELLO_MAC
#endif

#ifdef _MSC_VER
#define CELLO_MSC
#define popen _popen
#define pclose _pclose
#define __func__ __FUNCTION__
#ifndef CELLO_NSTRACE 
#pragma comment(lib, "DbgHelp.lib")
#endif
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
#ifndef CELLO_NSTRACE
#include <DbgHelp.h>
#endif
#endif

#ifdef CELLO_UNIX
#include <pthread.h>
#ifndef CELLO_NSTRACE
#include <execinfo.h>
#endif
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

#define Cello(T, ...) CelloStruct(T, ##__VA_ARGS__)
#define CelloStruct(T, ...) CelloObject(T, sizeof(struct T), ##__VA_ARGS__)
#define CelloEmpty(T, ...) CelloObject(T, 0, ##__VA_ARGS__)
#define CelloObject(T, S, ...) (var)((char*)((var[]){ NULL, \
  CELLO_ALLOC_HEADER       \
  CELLO_MAGIC_HEADER       \
  CELLO_CACHE_HEADER       \
  NULL, "__Name",     #T,  \
  NULL, "__Size", (var)S,  \
  ##__VA_ARGS__,           \
  NULL, NULL, NULL}) +     \
  sizeof(struct Header))

#define Instance(I, ...) NULL, #I, &((struct I){__VA_ARGS__})

/* Types */

extern var Type;
extern var Tuple;

extern var Ref;
extern var Box;
extern var Int;
extern var Float;
extern var String;

extern var Tree;
extern var List;
extern var Array;
extern var Table;
extern var Range;
extern var Slice;
extern var Zip;
extern var Filter;
extern var Map;
extern var Terminal;
extern var _;

extern var File;
extern var Mutex;
extern var Thread;
extern var Process;
extern var Function;

extern var Exception;
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
  AllocStatic = 0x01, AllocStack = 0x02,
  AllocHeap   = 0x03, AllocData  = 0x04
};

struct Header {
  var type;
#if CELLO_ALLOC_CHECK == 1
  var alloc;
#endif
#if CELLO_MAGIC_CHECK == 1
  var magic;
#endif
};

struct Type {
  var cls;
  var name;
  var inst;
};

struct Ref {
  var val;
};

struct Box {
  var val;
};

struct Int {
  int64_t val;
};

struct Float {
  double val;
};

struct String {
  char* val;
};

struct Tuple {
  var* items;
};

struct Range {
  var value;
  int64_t start;
  int64_t stop;
  int64_t step;
};

struct Slice {
  var iter;
  var range;
};

struct Zip {
  var iters;
  var values;
};

struct Filter {
  var iter;
  var func;
};

struct Map {
  var iter;
  var curr;
  var func;
};

struct File {
  FILE* file;
};

struct Process {
  FILE* proc;
};

struct Function {
  var (*func)(var);
};

/* Classes */

extern var Doc;
extern var Help;
extern var Cast;
extern var Size;
extern var Alloc;
extern var New;
extern var Copy;
extern var Assign;
extern var Swap;
extern var Cmp;
extern var Hash;
extern var Len;
extern var Iter;
extern var Push;
extern var Concat;
extern var Get;
extern var Sort;
extern var Resize;
extern var C_Str;
extern var C_Int;
extern var C_Float;
extern var Stream;
extern var Pointer;
extern var Call;
extern var Format;
extern var Show;
extern var Current;
extern var Start;
extern var Lock;
extern var Mark;

/* Signatures */

struct Example {
  const char* name;
  const char* body;
};

struct Method {
  const char* name;
  const char* definition;
  const char* description;
};

struct Doc {
  const char* (*name)(void);
  const char* (*brief)(void);
  const char* (*description)(void);
  const char* (*definition)(void);
  struct Example* (*examples)(void);
  struct Method* (*methods)(void);
};

struct Help {
  int (*help_to)(var, var, int);
};

struct Cast {
  var (*cast)(var, var);
};

struct Size {
  size_t (*size)(void);
};

struct Alloc {
  var (*alloc)(void);
  void (*dealloc)(var);
};

struct New {
  void (*construct_with)(var, var);
  void (*destruct)(var);
};

struct Copy {
  var (*copy)(var);
};

struct Assign {
  void (*assign)(var, var);
};

struct Swap {
  void (*swap)(var, var);
};

struct Cmp {
  int (*cmp)(var, var);
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
  bool (*mem)(var, var);
  void (*rem)(var, var);
  var (*key_type)(var);
  var (*val_type)(var);
};

struct Iter {
  var (*iter_init)(var);
  var (*iter_next)(var, var);
  var (*iter_last)(var);
  var (*iter_prev)(var, var);
  var (*iter_type)(var);  
};

struct Sort {
  void (*sort_by)(var,bool(*f)(var,var));
};

struct Resize {
  void (*resize)(var, size_t);
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

struct Stream {
  var  (*sopen)(var,var,var);
  void (*sclose)(var);
  void (*sseek)(var,int64_t,int);
  int64_t (*stell)(var);
  void (*sflush)(var);
  bool (*seof)(var);
  size_t (*sread)(var,void*,size_t);
  size_t (*swrite)(var,void*,size_t);
};

struct Pointer {
  void (*ref)(var, var);
  var (*deref)(var);
};

struct Call {
  var (*call_with)(var, var);
};

struct Format {
  int (*format_to)(var,int,const char*,va_list);
  int (*format_from)(var,int,const char*,va_list);
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
  void (*join)(var);
  bool (*running)(var);
};

struct Lock {
  void (*lock)(var);
  void (*unlock)(var);
  bool (*lock_try)(var);
};

struct Mark {
  void (*mark)(var, var, void(*)(var,void*));
};

/* Functions */

const char* name(var type);
const char* brief(var type);
const char* description(var type);
const char* definition(var type);

void help(var self);
int help_to(var out, int pos, var self);

var type_of(var self);
var cast(var self, var type);
var instance(var self, var cls);
bool implements(var self, var cls);
var type_instance(var type, var cls);
bool type_implements(var type, var cls);

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
bool implements_method_at_offset(var self, var cls, size_t offset);

var type_method_at_offset(var self, var cls, size_t offset, const char* method);
bool type_implements_method_at_offset(var self, var cls, size_t offset);

struct Header* header(var self);
var header_init(var head, var type, int alloc);

size_t size(var type);

var alloc(var type);
var alloc_raw(var type);
var alloc_root(var type);

#define alloc_stack(T) ((struct T*)header_init( \
  (char[sizeof(struct Header) + sizeof(struct T)]){0}, T, AllocStack))

void dealloc(var self);
void dealloc_raw(var self);
void dealloc_root(var self);

#define $(T, ...) ((struct T*)memcpy( \
  alloc_stack(T), &((struct T){__VA_ARGS__}), sizeof(struct T)))

#define $I(X) $(Int, X)
#define $F(X) $(Float, X)
#define $S(X) $(String, X)
#define $R(X) $(Ref, X)
#define $B(X) $(Box, X)

#define tuple(...) tuple_xp(tuple_in, (_, ##__VA_ARGS__, Terminal))
#define tuple_xp(X, A) X A
#define tuple_in(_, ...) $(Tuple, (var[]){ __VA_ARGS__ })

#define construct(self, ...) construct_with(self, tuple(__VA_ARGS__))
var construct_with(var self, var args);
var destruct(var self);

#define new(T, ...) ((struct T*)new_with(T, tuple(__VA_ARGS__)))
#define new_raw(T, ...) ((struct T*)new_raw_with(T, tuple(__VA_ARGS__)))
#define new_root(T, ...) ((struct T*)new_root_with(T, tuple(__VA_ARGS__)))

var new_with(var type, var args);
var new_raw_with(var type, var args);
var new_root_with(var type, var args);

void del(var self);
void del_raw(var self);
void del_root(var self);

var assign(var self, var obj);
var copy(var self);
void swap(var self, var obj);

int cmp(var self, var obj);
bool eq(var self, var obj);
bool neq(var self, var obj);
bool gt(var self, var obj);
bool lt(var self, var obj);
bool ge(var self, var obj);
bool le(var self, var obj);

uint64_t hash(var self);
uint64_t hash_data(const void* data, size_t num);

var iter_init(var self);
var iter_next(var self, var curr);
var iter_prev(var self, var curr);
var iter_last(var self);
var iter_type(var self);

#define foreach(...) foreach_xp(foreach_in, (__VA_ARGS__))
#define foreach_xp(X, A) X A
#define foreach_in(X, S) for(var \
  __##X = (S), \
  __Iter##X = instance(__##X, Iter), \
  X = ((struct Iter*)(__Iter##X))->iter_init(__##X); \
  X isnt Terminal; \
  X = ((struct Iter*)(__Iter##X))->iter_next(__##X, X))

void push(var self, var obj);
void pop(var self);
void push_at(var self, var obj, var key);
void pop_at(var self, var key);

void sort(var self);
void sort_by(var self, bool(*f)(var,var));

void append(var self, var obj);
void concat(var self, var obj);

var  get(var self, var key);
void set(var self, var key, var val);
bool mem(var self, var key);
void rem(var self, var key);
var key_type(var self);
var val_type(var self);

void resize(var self, size_t n);
size_t len(var self);
bool empty(var self);

char* c_str(var self);
int64_t c_int(var self);
double c_float(var self);

#define range(...) range_stack($(Range, $I(0), 0, 0, 0), tuple(__VA_ARGS__))

#define slice(I, ...) slice_xp(slice_in, (I, ##__VA_ARGS__))
#define slice_xp(X, A) X A
#define slice_in(...) slice_stack( \
  $(Slice, NULL, $(Range, $I(0), 0, 0, 0)), tuple(__VA_ARGS__))

#define reverse(I) slice(I, _, _, $I(-1))
#define filter(I, F) $(Filter, I, F)
#define map(I, F) $(Map, I, NULL, F)

#define zip(...) zip_stack( \
  $(Zip, tuple(__VA_ARGS__), \
  $(Tuple, (var[(sizeof((var[]){__VA_ARGS__})/sizeof(var))+1]){0})))

#define enumerate(I) enumerate_stack(zip(range(), I))

var range_stack(var self, var args);
var slice_stack(var self, var args);
var zip_stack(var self);
var enumerate_stack(var self);

var sopen(var self, var resource, var options);
void sclose(var self);
void sseek(var self, int64_t pos, int origin);
int64_t stell(var self);
void sflush(var self);
bool seof(var self);
size_t sread(var self, void* output, size_t size);
size_t swrite(var self, void* input, size_t size);

void ref(var self, var item);
var deref(var self);

#define call(self, ...) call_with(self, tuple(__VA_ARGS__))
var call_with(var self, var args);

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
void join(var self);
bool running(var self);

var start_in(var self);
var stop_in(var self);

#define with(...) with_xp(with_in, (__VA_ARGS__))
#define with_xp(X, A) X A
#define with_in(X, S) for(var X = start_in(S); X isnt NULL; X = stop_in(X))

void lock(var self);
bool lock_try(var self);
void unlock(var self);

#define try { jmp_buf __env; exception_try(&__env); if (!setjmp(__env))

#define catch(...) catch_xp(catch_in, (__VA_ARGS__))
#define catch_xp(X, A) X A
#define catch_in(X, ...) else { exception_try_fail(); } exception_try_end(); } \
  for (var X = exception_catch(tuple(__VA_ARGS__)); \
    X isnt NULL; X = NULL)

#define throw(E, F, ...) exception_throw(E, F, tuple(__VA_ARGS__))

void exception_try(jmp_buf* env);
void exception_try_end(void);
void exception_try_fail(void);
var exception_throw(var obj, const char* fmt, var args);
var exception_catch(var args);

void exception_signals(void);

var exception_object(void);
var exception_message(void);

void mark(var self, var gc, void(*f)(var,void*));

#ifndef CELLO_NGC

extern var GC;

int Cello_Main(int argc, char** argv);
void Cello_Exit(void);

#define main(...) \
  main(int argc, char** argv) { \
    var bottom = NULL; \
    new_raw(GC, $R(&bottom)); \
    atexit(Cello_Exit); \
    return Cello_Main(argc, argv); \
  }; \
  int Cello_Main(__VA_ARGS__)

#endif
  
#endif
