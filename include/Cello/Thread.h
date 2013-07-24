/*
** == Thread ==
**
*/

#ifndef CelloThread_h
#define CelloThread_h

#if defined(__unix__) || defined(__APPLE__)
  #include <pthread.h>
#elif defined(_WIN32)
  #undef data
  #undef in
  #include <windows.h>
  #define data typedef struct
  #define in ,
#else
  #error "Unsupported Threading Environment"
#endif

#include "Prelude.h"
#include "Function.h"

/* Process Like */

class {
  var  (*current)(void);
  void (*join)(var);
  void (*terminate)(var);
} Process;

var  current(var);
void join(var self);
void terminate(var self);

/* Thread Type */

global var Thread;
 
enum {
  EXC_MAX_DEPTH = 2048,
};

data {
  
  var type;
  
  /* Function Details */
  var func;
  var args;
  
#if defined(__unix__) || defined(__APPLE__)
  pthread_t thread;
#elif defined(_WIN32)
  HANDLE thread;
  DWORD id;
#endif

  bool is_main;
  bool running;
  
  /* Exception Details */
  bool exc_active;
  int exc_depth;
  jmp_buf exc_buffers[EXC_MAX_DEPTH];
  
  var exc_obj;
  var exc_msg;
  const char* exc_func;
  const char* exc_file;
  unsigned int exc_lineno;
  
  void* exc_backtrace[25];
  int exc_backtrace_count;
  
} ThreadData;

/** Thread_New(var self, var function) */
var Thread_New(var self, va_list* args);
var Thread_Delete(var self);
void Thread_Assign(var self, var obj);
var Thread_Copy(var self);

var Thread_Eq(var self, var obj);
var Thread_Gt(var self, var obj);
var Thread_Lt(var self, var obj);

long Thread_AsLong(var self);
long Thread_Hash(var self);

var Thread_Call(var self, var args);

var  Thread_Current(void);
void Thread_Join(var self);
void Thread_Terminate(var self);

instance(Thread, New) = { sizeof(ThreadData), Thread_New, Thread_Delete };
instance(Thread, Assign) = { Thread_Assign };
instance(Thread, Copy) = { Thread_Copy };
instance(Thread, Call) = { Thread_Call };
instance(Thread, Process) = { Thread_Current, Thread_Join, Thread_Terminate };
instance(Thread, AsLong) = { Thread_AsLong };

/* Lock */

class {
  void (*lock)(var);
  void (*unlock)(var);
  var  (*lock_try)(var);
} Lock;

void lock(var self);
void unlock(var self);
var lock_try(var self);

/* Mutex Type */

global var Mutex;
 
data {
  var type;
#if defined(__unix__) || defined(__APPLE__)
  pthread_mutex_t mutex;
#elif defined(_WIN32)
  HANDLE mutex;
#endif

} MutexData;

/** Mutex_New(var self) */
var Mutex_New(var self, va_list* args);
var Mutex_Delete(var self);
void Mutex_Assign(var self, var obj);
var Mutex_Copy(var self);

void Mutex_Lock(var self);
var Mutex_Lock_Try(var self);
void Mutex_Unlock(var self);

instance(Mutex, New) = { sizeof(MutexData), Mutex_New, Mutex_Delete };
instance(Mutex, Assign) = { Mutex_Assign };
instance(Mutex, Copy) = { Mutex_Copy };
instance(Mutex, Lock) = { Mutex_Lock, Mutex_Unlock, Mutex_Lock_Try };
instance(Mutex, With) = { Mutex_Lock, Mutex_Unlock };

#endif
