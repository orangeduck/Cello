/*
** == Exceptions ==
**
**  Exceptions in C+ are a little weird.
**   
**
*/
#ifndef ExceptionPlus_H
#define ExceptionPlus_H

#include "Prelude+.h"

module Exception;

data {
  var type;
  const char* name;
} ExceptionData;

const char* Exception_AsStr(var e);

instance(Exception, AsStr) = { Exception_AsStr };

/*
**  == Exception Records ==
**
**  Exception Macros and Records
**
*/

typedef struct __exc_record {
  struct __exc_record* prev;
  jmp_buf jb;
} __exc_record;

extern __exc_record  __exc_root;
extern __exc_record* __exc_top;

extern var __exc_obj;
extern const char* __exc_msg;
extern const char* __exc_file;
extern const char* __exc_func;
extern unsigned int __exc_lineno;

void __exc_record_push(__exc_record* er);
bool __exc_record_pop(__exc_record* er, var* exc, ...);

#define try \
  __exc_record __exc_r; \
  __exc_record_push(&__exc_r); \
  int __exc_res = setjmp(__exc_r.jb); \
  if (__exc_res == 0) 
  
#define catch(E, ...) \
  var E = Undefined; \
  if (__exc_res && __exc_record_pop(&__exc_r, &E, __VA_ARGS__, Undefined))
  
/* TODO: Format string */
#define throw(E, FMT, ...) \
  __exc_obj = E; \
  __exc_msg = FMT; \
  __exc_file = __FILE__; \
  __exc_func = __func__; \
  __exc_lineno = __LINE__; \
  longjmp(__exc_top->jb, 1);


#endif