/*
** == Thread ==
**
*/

#ifndef CelloThread_h
#define CelloThread_h

#ifdef __unix__
  #include <pthread.h>
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

#ifdef __unix__
 
  enum {
    EXC_MAX_DEPTH = 2048,
  };
 
  data {
    
    var type;
    
    /* Function Details */
    var func;
    var args;
    pthread_t thread;
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
  
#endif

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

/* Lockable */

class {
  void (*lock)(var);
  void (*unlock)(var);
  var  (*lock_try)(var);
  int  (*lock_status)(var);
} Lockable;

void lock(var self);
void unlock(var self);
var lock_try(var self);
int lock_status(var self);

/* Mutex Type */

global var Mutex;

#ifdef __unix__
 
  data {
    var type;
    pthread_mutex_t mutex;
  } MutexData;
    
#endif

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
instance(Mutex, Lockable) = { Mutex_Lock, Mutex_Unlock, Mutex_Lock_Try, NULL };
instance(Mutex, With) = { Mutex_Lock, Mutex_Unlock };

/* Semaphor Type */

global var Semaphore;

#ifdef __unix__
 
  data {
    var type;

  } SemaphoreData;
  
#endif

/** Semaphore_New(var self, int val) */
var Semaphore_New(var self, va_list* args);
var Semaphore_Delete(var self);

void Semaphore_Lock(var self);
void Semaphore_Unlock(var self);
var  Semaphore_LockTimeout(var self, int timeout);
int  Semaphore_LockValue(var self);

instance(Semaphore, New) = { sizeof(SemaphoreData), Semaphore_New, Semaphore_Delete };
instance(Semaphore, Lockable) = { Semaphore_Lock, Semaphore_Unlock, Semaphore_LockTimeout, Semaphore_LockValue };

#endif