#ifdef __unix__

#include "Cello/Thread.h"
#include "Cello/List.h"
#include "Cello/String.h"
#include "Cello/None.h"
#include "Cello/Exception.h"
#include "Cello/Bool.h"

#include <signal.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

var current(var type) {
  Process* iprocess = type_class(type, Process);
  assert(iprocess->current);
  return iprocess->current();  
}

void join(var self) {
  Process* iprocess = type_class(type_of(self), Process);
  assert(iprocess->join);
  return iprocess->join(self);
}

void terminate(var self) {
  Process* iprocess = type_class(type_of(self), Process);
  assert(iprocess->terminate);
  return iprocess->terminate(self);
}

var Thread = methods {
  methods_begin(Thread),
  method(Thread, New),
  method(Thread, Assign),
  method(Thread, Copy),
  method(Thread, Call),
  method(Thread, Process),
  method(Thread, AsLong),
  methods_end(Thread),
};

var Thread_New(var self, va_list* args) {
  
  ThreadData* td = cast(self, Thread);
  td->func = va_arg(*args, var);
  td->args = new(List, 0);
  td->is_main = false;
  td->running = false;
  
  td->exc_active = false;
  td->exc_depth = -1;
  memset(td->exc_buffers, 0, sizeof(jmp_buf) * EXC_MAX_DEPTH);
  
  td->exc_obj = Undefined;
  td->exc_msg = new(String, "");
  td->exc_func = NULL;
  td->exc_file = NULL;
  td->exc_lineno = 0;
  memset(td->exc_backtrace, 0, sizeof(void*) * 25);
  td->exc_backtrace_count = 0;
  
  return td;
}

var Thread_Delete(var self) {
  ThreadData* td = cast(self, Thread);
  delete(td->args);
  delete(td->exc_msg);
  return td;
}

void Thread_Assign(var self, var obj) {
  
  ThreadData* td = cast(self, Thread);
  ThreadData* to = cast(obj,  Thread);
  
  assign(td->func, to->func);
  assign(td->args, to->args);
  td->thread = to->thread;
  td->is_main = to->is_main;
  td->running = to->running;
  
  td->exc_active = to->exc_active;
  td->exc_depth = to->exc_depth;
  memmove(td->exc_buffers, to->exc_buffers, sizeof(jmp_buf) * EXC_MAX_DEPTH);
  
  assign(td->exc_obj, to->exc_obj);
  assign(td->exc_msg, to->exc_msg);
  
  td->exc_func = to->exc_func;
  td->exc_file = to->exc_file;
  td->exc_lineno = to->exc_lineno;
  memmove(td->exc_backtrace, to->exc_backtrace, sizeof(void*) * 25);
  td->exc_backtrace_count = to->exc_backtrace_count;
  
}

var Thread_Copy(var self) {
  var obj = new(Thread, None);
  assign(obj, self);
  return obj;
}

long Thread_Hash(var self) {
  return as_long(self);
}

long Thread_AsLong(var self) {
  ThreadData* td = cast(self, Thread);
  if (not td->running) { throw(ValueError, "Cannot get thread ID, thread not running!"); }
  return (long)td->thread;
}

var Thread_Eq(var self, var obj) {
  return (var)(intptr_t)(as_long(self) == as_long(obj));
}

var Thread_Gt(var self, var obj) {
  return (var)(intptr_t)(as_long(self) > as_long(obj));
}

var Thread_Lt(var self, var obj) {
  return (var)(intptr_t)(as_long(self) < as_long(obj));
}

local bool thread_keys_created = false;
local pthread_key_t key_thread_wrapper;

local void delete_pthread_keys(void) {
  pthread_key_delete(key_thread_wrapper);
}

local var Thread_Init_Run(var args) {
  
  var self = pop_front(args);
  pthread_setspecific(key_thread_wrapper, self);
  
  ThreadData* td = cast(self, Thread);
  td->running = true;
  return call_with(td->func, td->args);
  
}

var Thread_Call(var self, var args) {
  
  /* Setup Thread Local Storage */
  
  if (not thread_keys_created) {
    pthread_key_create(&key_thread_wrapper, NULL);
    thread_keys_created = true;
    atexit(delete_pthread_keys);
  }
  
  /* Copy Arguments & Push Thread Object */  
  
  ThreadData* td = cast(self, Thread);
  assign(td->args, args);
  push_front(td->args, self);
  
  /* Call Init Thread & Run */
  
  int err = pthread_create(&td->thread, NULL, Thread_Init_Run, td->args);
  if (err is EINVAL) { throw(ValueError, "Invalid Argument to Thread Creation"); }
  if (err is EAGAIN) { throw(OutOfMemoryError, "Not enough resources to create another Thread"); }
  if (err is EBUSY)  { throw(BusyError, "System is too busy to create thread"); }
  
  return self;
    
}

local ThreadData main_thread_wrapper = { is_main: true, exc_depth: -1 };

var Thread_Current(void) {
  
  var wrapper = pthread_getspecific(key_thread_wrapper);
  if (wrapper) {
    return wrapper;
  } else {
    main_thread_wrapper.thread = pthread_self();
    return &main_thread_wrapper;
  }
  
}

void Thread_Join(var self) {
  ThreadData* td = cast(self, Thread);
  if (not td->thread) { return; }
  
  int err = pthread_join(td->thread, NULL);
  if (err is EINVAL) { throw(ValueError, "Invalid Argument to Thread Join"); }
  if (err is ESRCH)  { throw(ValueError, "Invalid Thread"); }
}

void Thread_Terminate(var self) {
  ThreadData* td = cast(self, Thread);
  if (not td->thread) { return; }
  
  int err = pthread_kill(td->thread, SIGINT);
  if (err is EINVAL) { throw(ValueError, "Invalid Argument to Thread Kill"); }
  if (err is ESRCH)  { throw(ValueError, "Invalid Thread"); }
}

void lock(var self) {
  Lockable* ilockable = type_class(type_of(self), Lockable);
  assert(ilockable->lock);
  ilockable->lock(self);  
}

void unlock(var self) {
  Lockable* ilockable = type_class(type_of(self), Lockable);
  assert(ilockable->unlock);
  ilockable->unlock(self);
}

var lock_try(var self) {
  Lockable* ilockable = type_class(type_of(self), Lockable);
  assert(ilockable->lock_try);
  return ilockable->lock_try(self);  
}

int lock_status(var self) {
  Lockable* ilockable = type_class(type_of(self), Lockable);
  assert(ilockable->lock_status);
  return ilockable->lock_status(self);
}

var Mutex = methods {
  methods_begin(Mutex),
  method(Mutex, New),
  method(Mutex, Assign),
  method(Mutex, Copy),
  method(Mutex, With),
  method(Mutex, Lockable),
  methods_end(Mutex)
};

var Mutex_New(var self, va_list* args) {
  MutexData* md = cast(self, Mutex);
  pthread_mutex_init(&md->mutex, NULL);
  return md;
}

var Mutex_Delete(var self) {
  MutexData* md = cast(self, Mutex);
  pthread_mutex_destroy(&md->mutex);
  return md;
}

void Mutex_Assign(var self, var obj) {
  MutexData* md = cast(self, Mutex);
  MutexData* mo = cast(self, Mutex);
  md->mutex = mo->mutex;
  return md;
}

var Mutex_Copy(var self) {
  var obj = new(Mutex);
  assign(obj, self);
  return obj;
}

void Mutex_Lock(var self) {
  MutexData* md = cast(self, Mutex);
  int err = pthread_mutex_lock(&md->mutex);
  if (err is EINVAL)     { throw(ValueError, "Invalid Argument to Mutex Lock"); }
  //if (err is EDESTROYED) { throw(ResourceError, "Mutex has been Destroyed"); }
  //if (err is EOWNERTERM) { throw(ResourceError, "Thread Holding Mutex has been Terminated"); }
  if (err is EDEADLK)    { throw(ResourceError, "Attempt to relock already held mutex"); }
  //if (err is ERECURSE)   { throw(ResourceError, "Recursive Mutex cannot be held again"); }
}

var Mutex_Lock_Try(var self) {
  MutexData* md = cast(self, Mutex);
  int err = pthread_mutex_trylock(&md->mutex);
  if (err == EBUSY) { return False; }
  if (err is EINVAL) { throw(ValueError, "Invalid Argument to Mutex Lock Try"); }
  //if (err is ERECURSE)   { throw(ResourceError, "Recursive Mutex cannot be held again"); }
  return True;
}

void Mutex_Unlock(var self) {
  MutexData* md = cast(self, Mutex);
  int err = pthread_mutex_unlock(&md->mutex);
  if (err is EINVAL) { throw(ValueError, "Invalid Argument to Mutex Unlock"); }
  if (err is EPERM)  { throw(ResourceError, "Mutex cannot be held by caller"); }
}

var Semaphore_New(var self, va_list* args) {

}

var Semaphore_Delete(var self) {

}

void Semaphore_Lock(var self) {

}

void Semaphore_Unlock(var self) {

}

var  Semaphore_LockTimeout(var self, int timeout) {

}

int  Semaphore_LockValue(var self) {

}


#endif