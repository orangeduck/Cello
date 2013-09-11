#include "Cello/Thread.h"
#include "Cello/List.h"
#include "Cello/String.h"
#include "Cello/None.h"
#include "Cello/Exception.h"
#include "Cello/Bool.h"

#include <signal.h>
#include <string.h>
#include <errno.h>

var current(var type) {
  return type_class_method(type, Process, current);
}

void join(var self) {
  type_class_method(type_of(self), Process, join, self);
}

void terminate(var self) {
  type_class_method(type_of(self), Process, terminate, self);
}

var Thread = type_data {
  type_begin(Thread),
  type_entry(Thread, New),
  type_entry(Thread, Assign),
  type_entry(Thread, Copy),
  type_entry(Thread, Call),
  type_entry(Thread, Process),
  type_entry(Thread, AsLong),
  type_end(Thread),
};

var Thread_New(var self, var_list vl) {
  
  ThreadData* td = cast(self, Thread);
  td->func = cast(var_list_get(vl), Function);
  td->args = new(List);
  td->is_main = false;
  td->running = false;
  
  td->exc_active = false;
  td->exc_depth = 0;
  memset(td->exc_buffers, 0, sizeof(jmp_buf) * EXC_MAX_DEPTH);
  
  td->exc_obj = Undefined;
  td->exc_msg = new(String, $(String, ""));
  td->exc_func = NULL;
  td->exc_file = NULL;
  td->exc_lineno = 0;
  memset(td->exc_backtrace, 0, sizeof(void*) * 25);
  td->exc_backtrace_count = 0;
  
  return td;
}

var Thread_Delete(var self) {
  ThreadData* td = cast(self, Thread);
  
#ifdef _WIN32
  CloseHandle(td->thread);
#endif
  
  delete(td->args);
  delete(td->exc_msg);
  return td;
}

size_t Thread_Size(void) {
  return sizeof(ThreadData);
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
#if defined(__unix__) || defined(__APPLE__)
  return (long)td->thread;
#elif defined(_WIN32)
  return (long)td->id;
#endif  
  
}

var Thread_Eq(var self, var obj) {
  return bool_var(as_long(self) == as_long(obj));
}

var Thread_Gt(var self, var obj) {
  return bool_var(as_long(self) > as_long(obj));
}

var Thread_Lt(var self, var obj) {
  return bool_var(as_long(self) < as_long(obj));
}

local bool tls_key_created = false;

#if defined(__unix__) || defined(__APPLE__)

local pthread_key_t key_thread_wrapper;
local void tls_key_create(void) {
  pthread_key_create(&key_thread_wrapper, NULL);
}
local void tls_key_delete(void) {
  pthread_key_delete(key_thread_wrapper);
}

local var Thread_Init_Run(var args) {
  
  var self = pop_front(args);
  pthread_setspecific(key_thread_wrapper, self);
  
  ThreadData* td = cast(self, Thread);
  td->running = true;
  return call_with(td->func, td->args);
  
}

#elif defined(_WIN32)

local DWORD key_thread_wrapper;
local void tls_key_create(void) {
  key_thread_wrapper = TlsAlloc();
}
local void tls_key_delete(void) {
  TlsFree(key_thread_wrapper);
}

local DWORD Thread_Init_Run(var args) {
  
  var self = pop_front(args);
  TlsSetValue(key_thread_wrapper, self);
  
  ThreadData* td = cast(self, Thread);
  td->running = true;
  call_with(td->func, td->args);
  return 0;
  
}

#endif

var Thread_Call(var self, var args) {
  
  /* Setup Thread Local Storage */
  
  if (not tls_key_created) {
    tls_key_create();
    tls_key_created = true;
    atexit(tls_key_delete);
  }
  
  /* Copy Arguments & Push Thread Object */  
  
  ThreadData* td = cast(self, Thread);
  assign(td->args, args);
  push_front(td->args, self);
  
  /* Call Init Thread & Run */
  
#if defined(__unix__) || defined(__APPLE__)
  int err = pthread_create(&td->thread, NULL, Thread_Init_Run, td->args);
  if (err is EINVAL) { throw(ValueError, "Invalid Argument to Thread Creation"); }
  if (err is EAGAIN) { throw(OutOfMemoryError, "Not enough resources to create another Thread"); }
  if (err is EBUSY)  { throw(BusyError, "System is too busy to create thread"); }
#elif defined(_WIN32)
  td->thread = CreateThread(NULL, 0, Thread_Init_Run, td->args, 0, &td->id);
  if (td->thread == NULL) {
    throw(ValueError, "Unable to Create WinThread");
  }
#endif
  
  return self;
  
}

local ThreadData main_thread_wrapper;

var Thread_Current(void) {
  
#if defined(__unix__) || defined(__APPLE__)
  var wrapper = pthread_getspecific(key_thread_wrapper);
#elif defined(_WIN32)
  var wrapper = TlsGetValue(key_thread_wrapper);
#endif
  
  /*
  ** Here is a nasty one. On OSX instead of
  ** returning NULL for an unset key it
  ** decides to return uninitialized rubbish
  ** (even though the spec says otherwise).
  **
  ** Luckily we can test directly for the main
  ** thread on OSX using this non-portable method
  */
#if defined(__APPLE__)
  if (pthread_main_np()) { wrapper = NULL; }
#endif
  
  if (wrapper) {
    return wrapper;
  } else {
    main_thread_wrapper.is_main = true;
#if defined(__unix__) || defined(__APPLE__)
    main_thread_wrapper.thread = pthread_self();
#elif defined(_WIN32)
    main_thread_wrapper.thread = GetCurrentThread();
#endif
    return &main_thread_wrapper;
  }
  
}

void Thread_Join(var self) {
  ThreadData* td = cast(self, Thread);
  if (not td->thread) { return; }
  
#if defined(__unix__) || defined(__APPLE__)
  int err = pthread_join(td->thread, NULL);
  if (err is EINVAL) { throw(ValueError, "Invalid Argument to Thread Join"); }
  if (err is ESRCH)  { throw(ValueError, "Invalid Thread"); }
#elif defined(_WIN32)
  WaitForSingleObject(td->thread, INFINITE);
#endif
  
}

void Thread_Terminate(var self) {
  ThreadData* td = cast(self, Thread);
  if (not td->thread) { return; }
  
#if defined(__unix__) || defined(__APPLE__)
  int err = pthread_kill(td->thread, SIGINT);
  if (err is EINVAL) { throw(ValueError, "Invalid Argument to Thread Kill"); }
  if (err is ESRCH)  { throw(ValueError, "Invalid Thread"); }
#elif defined(_WIN32)
  TerminateThread(td->thread, FALSE);
#endif  
  
}

void lock(var self) {
  type_class_method(type_of(self), Lock, lock, self);
}

void unlock(var self) {
  type_class_method(type_of(self), Lock, unlock, self);
}

data {
  var type;
#if defined(__unix__) || defined(__APPLE__)
  pthread_mutex_t mutex;
#elif defined(_WIN32)
  HANDLE mutex;
#endif

} MutexData;

var Mutex = type_data {
  type_begin(Mutex),
  type_entry(Mutex, New),
  type_entry(Mutex, Assign),
  type_entry(Mutex, Copy),
  type_entry(Mutex, With),
  type_entry(Mutex, Lock),
  type_end(Mutex)
};

var Mutex_New(var self, var_list vl) {
  MutexData* md = cast(self, Mutex);
#if defined(__unix__) || defined(__APPLE__)
  pthread_mutex_init(&md->mutex, NULL);
#elif defined(_WIN32)
  md->mutex = CreateMutex(NULL, false, NULL);
#endif
  return md;
}

var Mutex_Delete(var self) {
  MutexData* md = cast(self, Mutex);
#if defined(__unix__) || defined(__APPLE__)
  pthread_mutex_destroy(&md->mutex);
#elif defined(_WIN32)
  CloseHandle(md->mutex);
#endif
  return md;
}

size_t Mutex_Size(void) {
  return sizeof(MutexData);
}

void Mutex_Assign(var self, var obj) {
  MutexData* md = cast(self, Mutex);
  MutexData* mo = cast(self, Mutex);
  md->mutex = mo->mutex;
}

var Mutex_Copy(var self) {
  var obj = new(Mutex);
  assign(obj, self);
  return obj;
}

void Mutex_Lock(var self) {
  MutexData* md = cast(self, Mutex);
#if defined(__unix__) || defined(__APPLE__)
  int err = pthread_mutex_lock(&md->mutex);
  if (err is EINVAL)     { throw(ValueError, "Invalid Argument to Mutex Lock"); }
  if (err is EDEADLK)    { throw(ResourceError, "Attempt to relock already held mutex"); }
#elif defined(_WIN32)
  WaitForSingleObject(md->mutex, INFINITE);
#endif
  
}

var Mutex_Lock_Try(var self) {
  MutexData* md = cast(self, Mutex);
#if defined(__unix__) || defined(__APPLE__)
  int err = pthread_mutex_trylock(&md->mutex);
  if (err == EBUSY) { return False; }
  if (err is EINVAL) { throw(ValueError, "Invalid Argument to Mutex Lock Try"); }
  return True;
#elif defined(_WIN32)
  return bool_var(not (WaitForSingleObject(md->mutex, 0) is WAIT_TIMEOUT));
#endif
  
}

void Mutex_Unlock(var self) {
  MutexData* md = cast(self, Mutex);
#if defined(__unix__) || defined(__APPLE__)
  int err = pthread_mutex_unlock(&md->mutex);
  if (err is EINVAL) { throw(ValueError, "Invalid Argument to Mutex Unlock"); }
  if (err is EPERM)  { throw(ResourceError, "Mutex cannot be held by caller"); }
#elif defined(_WIN32)
  ReleaseMutex(md->mutex);
#endif
  
}
