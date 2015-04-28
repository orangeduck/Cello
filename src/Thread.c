#include "Cello.h"

static const char* Current_Name(void) {
  return "Current";
}

static const char* Current_Brief(void) {
  return "Implicit Object";
}

static const char* Current_Description(void) {
  return
    "The `Current` class can be implemented by types which have implicit "
    "instances associated with them. For example it can be used to retrieve "
    "the _current_ `Thread`, or it could be used to get the _current_ Random "
    "Number generator."
    "\n\n"
    "This class may be implemented by types which express the [Singleton "
    "Design Pattern](http://en.wikipedia.org/wiki/Singleton_pattern)";
}

static struct DocExample* Current_Examples(void) {
  
  static struct DocExample examples[] = {
    {
      "Usage",
      "var gc = current(GC);\n"
      "show(gc);\n"
      "var thread = current(Thread);\n"
      "show(thread);\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct DocMethod* Current_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "current", 
      "var current(var type);",
      "Returns the current active object of the given `type`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Current = Cello(Current,
  Instance(Doc,
    Current_Name, Current_Brief, Current_Description,
    Current_Examples, Current_Methods));

var current(var type) {
  return type_method(type, Current, current);
}

/* TODO: Is this better named Wait? Could it become part of `Start`? */

static const char* Join_Name(void) {
  return "Join";
}

static const char* Join_Brief(void) {
  return "Wait for object";
}

static const char* Join_Description(void) {
  return
    "The `Join` class can be implemented by types which provide a mechanism "
    "for waiting for them to enter some same state. Primarily it is "
    "implemented by `Thread`.";
}

static struct DocMethod* Join_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "join", 
      "void join(var self);",
      "Wait for the object `self` to enter some state."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

/* TODO: Examples */

var Join = Cello(Join,
  Instance(Doc,
    Join_Name, Join_Brief, Join_Description,
    NULL, Join_Methods));

void join(var self) {
  method(self, Join, join);
}

enum {
  CELLO_EXC_MAX_DEPTH  = 2048,
  CELLO_EXC_MAX_STRACE = 25
};

struct Thread {
  
  var func;
  var args;
  
#if defined(__unix__) || defined(__APPLE__)
  pthread_t thread;
#elif defined(_WIN32)
  DWORD id;
  HANDLE thread;
#endif

  bool is_main;
  bool is_running;
  
  var      exc_obj;
  var      exc_msg;
  bool     exc_active;
  size_t   exc_depth;
  jmp_buf* exc_buffers[CELLO_EXC_MAX_DEPTH];
  
  var tls;
  
};

static const char* Thread_Name(void) {
  return "Thread";
}

static const char* Thread_Brief(void) {
  return "Concurrent Execution";
}

static const char* Thread_Description(void) {
  return
    "The `Thread` type provides a basic primitive for concurrent "
    "execution. It acts as a basic wrapper around operating system threads, "
    "using WinThreads on Windows and pthreads otherwise.";
}

/* TODO: Examples Methods */

static void Thread_New(var self, var args) {
  
  struct Thread* t = self;
  
  t->func = empty(args) ? NULL : get(args, $I(0));
  t->args = NULL;
  t->is_main = false;
  t->is_running = false;
  
  t->exc_active = false;
  t->exc_depth = 0;
  memset(t->exc_buffers, 0, sizeof(jmp_buf*) * CELLO_EXC_MAX_DEPTH);
  t->exc_obj = NULL;
  t->exc_msg = NULL;
  
  t->tls = new_raw(Table, String, Ref);
  
}

static void Thread_Del(var self) {
  struct Thread* t = self;

#ifdef _WIN32
  CloseHandle(t->thread);
#endif
  
  if (t->args    isnt NULL) { del_raw(t->args); }
  if (t->exc_msg isnt NULL) { del_raw(t->exc_msg); }
  
  del_raw(t->tls);
}

static uint64_t Thread_Hash(var self) {
  return c_int(self);
}

static int64_t Thread_C_Int(var self) {
  struct Thread* t = self;
  
  if (not t->is_running) {
    throw(ValueError, "Cannot get thread ID, thread not running!");
  }
  
#if defined(__unix__) || defined(__APPLE__)
  return t->thread;
#elif defined(_WIN32)
  return t->id;
#endif
  
}

static bool Thread_Eq(var self, var obj) {
  return Thread_C_Int(self) == c_int(obj);
}

static bool Thread_Gt(var self, var obj) {
  return Thread_C_Int(self) > c_int(obj);
}

static bool Thread_Lt(var self, var obj) {
  return Thread_C_Int(self) < c_int(obj);
}

static bool Thread_TLS_Key_Created = false;

#if defined(__unix__) || defined(__APPLE__)

static pthread_key_t Thread_Key_Wrapper;

static void Thread_TLS_Key_Create(void) {
  pthread_key_create(&Thread_Key_Wrapper, NULL);
}
static void Thread_TLS_Key_Delete(void) {
  pthread_key_delete(Thread_Key_Wrapper);
}

static var Thread_Init_Run(var self) {

  struct Thread* t = self;  
  pthread_setspecific(Thread_Key_Wrapper, t);
  t->is_running = true;
  
#if CELLO_GC == 1
  var bottom = NULL;
  var gc = new_raw(GC, $R(&bottom));
#endif
  
  var x = call_with(t->func, t->args);
  del_raw(t->args);
  t->args = NULL;
  
#if CELLO_GC == 1
  del_raw(gc);
#endif
  
  return x;
}

#elif defined(_WIN32)

static DWORD Thread_Key_Wrapper;

static void Thread_TLS_Key_Create(void) {
  Thread_Key_Wrapper = TlsAlloc();
}
static void Thread_TLS_Key_Delete(void) {
  TlsFree(Thread_Key_Wrapper);
}

static DWORD Thread_Init_Run(var self) {
  
  struct Thread* t = self;
  TlsSetValue(Thread_Key_Wrapper, t);
  t->is_running = true;
  
#if CELLO_GC == 1
  var bottom = NULL;
  var gc = new_raw(GC, $R(&bottom));
#endif
  
  call_with(t->func, t->args);
  del_raw(t->args);
  t->args = NULL;
  
#if CELLO_GC == 1
  del_raw(gc);
#endif
  
  return 0;
}

#endif

static var Thread_Call(var self, var args) {
  
  struct Thread* t = self;
  
  /* Setup Thread Local Storage */
  
  if (not Thread_TLS_Key_Created) {
    Thread_TLS_Key_Create();
    Thread_TLS_Key_Created = true;
    atexit(Thread_TLS_Key_Delete);
  }
  
  t->args = assign(alloc_raw(type_of(args)), args);  
  
  /* Call Init Thread & Run */
  
#if defined(__unix__) || defined(__APPLE__)
  
  int err = pthread_create(&t->thread, NULL, Thread_Init_Run, t);
  
  if (err is EINVAL) {
    throw(ValueError, "Invalid Argument to Thread Creation");
  }
  
  if (err is EAGAIN) {
    throw(OutOfMemoryError, "Not enough resources to create another Thread");
  }
  
  if (err is EBUSY)  {
    throw(BusyError, "System is too busy to create thread");
  }
  
#elif defined(_WIN32)
  
  t->thread = CreateThread(NULL, 0,
    (LPTHREAD_START_ROUTINE)Thread_Init_Run, t, 0, &t->id);
  
  if (t->thread is NULL) {
    throw(ValueError, "Unable to Create WinThread");
  }
  
#endif
  
  return self;
  
}

static var Thread_Main = NULL;

static void Thread_Main_Del(void) {
  del_raw(Thread_Main);
}

static var Thread_Current(void) {
  
#if defined(__unix__) || defined(__APPLE__)
  var wrapper = pthread_getspecific(Thread_Key_Wrapper);
#elif defined(_WIN32)
  var wrapper = TlsGetValue(Thread_Key_Wrapper);
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
  
  if (wrapper is NULL) {
  
    if (Thread_Main is NULL) {
      Thread_Main = new_raw(Thread);
      atexit(Thread_Main_Del);
    }
    
    struct Thread* t = Thread_Main;
    t->is_main = true;
    t->is_running = true;
    
#if defined(__unix__) || defined(__APPLE__)
    t->thread = pthread_self();
#elif defined(_WIN32)
    t->thread = GetCurrentThread();
#endif

    return Thread_Main;
  }
  
  return wrapper;
  
}

static void Thread_Join(var self) {
  struct Thread* t = self;
  if (not t->thread) { return; }
  
#if defined(__unix__) || defined(__APPLE__)
  int err = pthread_join(t->thread, NULL);
  if (err is EINVAL) { throw(ValueError, "Invalid Argument to Thread Join"); }
  if (err is ESRCH)  { throw(ValueError, "Invalid Thread"); }
#elif defined(_WIN32)
  WaitForSingleObject(t->thread, INFINITE);
#endif
  
}

static void Thread_Start(var self) {
  call(self);
}

static void Thread_Stop(var self) {
  struct Thread* t = self;
  if (not t->thread) { return; }
  
#if defined(__unix__) || defined(__APPLE__)
  int err = pthread_kill(t->thread, SIGINT);
  if (err is EINVAL) { throw(ValueError, "Invalid Argument to Thread Stop"); }
  if (err is ESRCH)  { throw(ValueError, "Invalid Thread"); }
#elif defined(_WIN32)
  TerminateThread(t->thread, FALSE);
#endif  
  
}

static bool Thread_Running(var self) {
  struct Thread* t = self;
  return t->is_running;
}

static var Thread_Get(var self, var key) {
  struct Thread* t = self;
  return deref(get(t->tls, key));
}

static void Thread_Set(var self, var key, var val) {
  struct Thread* t = self;
  set(t->tls, key, val);
}

static bool Thread_Mem(var self, var key) {
  struct Thread* t = self;
  return mem(t->tls, key);
}

static void Thread_Mark(var self, var gc, void(*f)(var,void*)) {
  struct Thread* t = self;
  mark(t->tls, gc, f);
}

var Thread = Cello(Thread,
  Instance(Doc,
    Thread_Name, Thread_Brief, Thread_Description, 
    NULL, NULL),
  Instance(New,     Thread_New, Thread_Del),
  Instance(Call,    Thread_Call),
  Instance(Current, Thread_Current),
  Instance(Join,    Thread_Join),
  Instance(Mark,    Thread_Mark),
  Instance(Start,   Thread_Start, Thread_Stop, Thread_Running),
  Instance(C_Int,   Thread_C_Int),
  Instance(Get,     Thread_Get, Thread_Set, Thread_Mem));

static const char* Lock_Name(void) {
  return "Lock";
}

static const char* Lock_Brief(void) {
  return "Exclusive Resource";
}

static const char* Lock_Description(void) {
  return
    "The `Lock` class can be implemented by types to limit the access to them. "
    "For example this class is implemented by the `Mutex` type to provide "
    "mutual exclusion across Threads.";
}

static struct DocMethod* Lock_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "lock", 
      "void lock(var self);",
      "Wait until a lock can be aquired on object `self`."
    }, {
      "lock_try", 
      "bool lock_try(var self);",
      "Try to acquire a lock on object `self`. Returns `true` on success and "
      "`false` if the resource is busy."
    }, {
      "unlock", 
      "void unlock(var self);",
      "Release lock on object `self`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

/* TODO: Examples */

var Lock = Cello(Lock,
  Instance(Doc,
    Lock_Name, Lock_Brief, Lock_Description,
    NULL, Lock_Methods));

void lock(var self) {
  method(self, Lock, lock);
}

void unlock(var self) {
  method(self, Lock, unlock);
}

bool lock_try(var self) {
  return method(self, Lock, lock_try);
}

struct Mutex {
#if defined(__unix__) || defined(__APPLE__)
  pthread_mutex_t mutex;
#elif defined(_WIN32)
  HANDLE mutex;
#endif
};

static const char* Mutex_Name(void) {
  return "Mutex";
}

static const char* Mutex_Brief(void) {
  return "Mutual Exclusion Lock";
}

static const char* Mutex_Description(void) {
  return
    "The `Mutex` type can be used to gain mutual exclusion across Threads for "
    "access of some resource.";
}

/* TODO: Examples Methods */

static void Mutex_New(var self, var args) {
  struct Mutex* m = self;
#if defined(__unix__) || defined(__APPLE__)
  pthread_mutex_init(&m->mutex, NULL);
#elif defined(_WIN32)
  m->mutex = CreateMutex(NULL, false, NULL);
#endif
}

static void Mutex_Del(var self) {
  struct Mutex* m = self;
#if defined(__unix__) || defined(__APPLE__)
  pthread_mutex_destroy(&m->mutex);
#elif defined(_WIN32)
  CloseHandle(m->mutex);
#endif
}

static void Mutex_Lock(var self) {
  struct Mutex* m = self;
#if defined(__unix__) || defined(__APPLE__)
  int err = pthread_mutex_lock(&m->mutex);
  
  if (err is EINVAL)  {
    throw(ValueError, "Invalid Argument to Mutex Lock");
  }
  
  if (err is EDEADLK) {
    throw(ResourceError, "Attempt to relock already held mutex");
  }
#elif defined(_WIN32)
  WaitForSingleObject(m->mutex, INFINITE);
#endif
  
}

static bool Mutex_Lock_Try(var self) {
  struct Mutex* m = self;
#if defined(__unix__) || defined(__APPLE__)
  int err = pthread_mutex_trylock(&m->mutex);
  if (err == EBUSY) { return false; }
  if (err is EINVAL) {
    throw(ValueError, "Invalid Argument to Mutex Lock Try");
  }
  return true;
#elif defined(_WIN32)
  return not (WaitForSingleObject(m->mutex, 0) is WAIT_TIMEOUT);
#endif
  
}

static void Mutex_Unlock(var self) {
  struct Mutex* m = cast(self, Mutex);
#if defined(__unix__) || defined(__APPLE__)
  int err = pthread_mutex_unlock(&m->mutex);
  if (err is EINVAL) { throw(ValueError, "Invalid Argument to Mutex Unlock"); }
  if (err is EPERM)  { throw(ResourceError, "Mutex cannot be held by caller"); }
#elif defined(_WIN32)
  ReleaseMutex(m->mutex);
#endif
  
}

var Mutex = Cello(Mutex,
  Instance(Doc, 
    Mutex_Name, Mutex_Brief, Mutex_Description, NULL, NULL),
  Instance(New,    Mutex_New, Mutex_Del),
  Instance(Lock,   Mutex_Lock, Mutex_Unlock, Mutex_Lock_Try),
  Instance(Start,  Mutex_Lock, Mutex_Unlock, NULL));

var TypeError = CelloEmpty(TypeError);
var ValueError = CelloEmpty(ValueError);
var ClassError = CelloEmpty(ClassError);
var IndexOutOfBoundsError = CelloEmpty(IndexOutOfBoundsError);
var KeyError = CelloEmpty(KeyError);
var OutOfMemoryError = CelloEmpty(OutOfMemoryError);
var IOError = CelloEmpty(IOError);
var FormatError = CelloEmpty(FormatError);
var BusyError = CelloEmpty(BusyError);
var ResourceError = CelloEmpty(ResourceError);

var ProgramAbortedError = CelloEmpty(ProgramAbortedError);
var DivisionByZeroError = CelloEmpty(DivisionByZeroError);
var IllegalInstructionError = CelloEmpty(IllegalInstructionError);
var ProgramInterruptedError = CelloEmpty(ProgramInterruptedError);
var SegmentationError = CelloEmpty(SegmentationError);
var ProgramTerminationError = CelloEmpty(ProgramTerminationError);

static void Exception_Signal(int sig) {
  switch(sig) {
    case SIGABRT: throw(ProgramAbortedError, "Program Aborted");
    case SIGFPE:  throw(DivisionByZeroError, "Division by Zero");
    case SIGILL:  throw(IllegalInstructionError, "Illegal Instruction");
    case SIGINT:  throw(ProgramInterruptedError, "Program Interrupted");
    case SIGSEGV: throw(SegmentationError, "Segmentation fault");
    case SIGTERM: throw(ProgramTerminationError, "Program Terminated");
  }
}

void exception_register_signals(void) {
  signal(SIGABRT, Exception_Signal);
  signal(SIGFPE,  Exception_Signal);
  signal(SIGILL,  Exception_Signal);
  signal(SIGINT,  Exception_Signal);
  signal(SIGSEGV, Exception_Signal);
  signal(SIGTERM, Exception_Signal);
}

void exception_inc(void) {
  struct Thread* t = Thread_Current();
  if (t->exc_depth is CELLO_EXC_MAX_DEPTH) {
    fprintf(stderr, "Cello Fatal Error: Exception Buffer Overflow!\n");
    abort();
  }
  t->exc_depth++;
}

void exception_dec(void) {
  struct Thread* t = Thread_Current();
  if (t->exc_depth == 0) {
    fprintf(stderr, "Cello Fatal Error: Exception Buffer Underflow!\n");
    abort();
  }
  t->exc_depth--;
}

bool exception_active(void) {
  struct Thread* t = Thread_Current();
  return t->exc_active;
}

void exception_activate(void) {
  struct Thread* t = Thread_Current();
  t->exc_active = true;
}

void exception_deactivate(void) {
  struct Thread* t = Thread_Current();
  t->exc_active = false;  
}

var exception_object(void) {
  struct Thread* t = Thread_Current();
  return t->exc_obj;  
}

var exception_message(void) {
  struct Thread* t = Thread_Current();
  return t->exc_msg;
}

void exception_try(jmp_buf* env) {
  struct Thread* t = Thread_Current();
  exception_inc();
  exception_deactivate();
  t->exc_buffers[t->exc_depth-1] = env;
}

jmp_buf* exception_buffer(void) {
  struct Thread* t = Thread_Current();
  if (t->exc_depth == 0) {
    fprintf(stderr, "Cello Fatal Error: Exception Buffer Out of Bounds!\n");
    abort();
  }
  return t->exc_buffers[t->exc_depth-1];
}

size_t exception_depth(void) {
  struct Thread* t = Thread_Current();
  return t->exc_depth;
}

#ifndef CELLO_NSTRACE
#if defined(__unix__) || defined(__APPLE__)

static void Exception_Backtrace(void) {
  
  struct Thread* t = Thread_Current();
  
  var exc_backtrace[CELLO_EXC_MAX_STRACE];
  size_t exc_backtrace_count = backtrace(exc_backtrace, CELLO_EXC_MAX_STRACE);
  char** symbols = backtrace_symbols(exc_backtrace, exc_backtrace_count);  
  
  print_to($(File, stderr), 0, "!!\tStack Trace: \n");
  print_to($(File, stderr), 0, "!!\t\n");

  for (size_t i = 0; i < exc_backtrace_count; i++) {
    print_to($(File, stderr), 0, "!!\t\t[%i] %s\n", 
      $(Int, i), $(String, symbols[i]));
  }
  print_to($(File, stderr), 0, "!!\t\n");
  
  free(symbols);
  
}

#elif defined(_WIN32)

static void Exception_Backtrace(void) {
  
  HANDLE process = GetCurrentProcess();
  HANDLE thread = GetCurrentThread();
  
  CONTEXT context;
  memset(&context, 0, sizeof(CONTEXT));
  context.ContextFlags = CONTEXT_FULL;
  RtlCaptureContext(&context);
  
  SymSetOptions(SYMOPT_UNDNAME|SYMOPT_LOAD_LINES);
  SymInitialize(process, NULL, TRUE);
  
  DWORD image;
  STACKFRAME64 stackframe;
  ZeroMemory(&stackframe, sizeof(STACKFRAME64));
  
#ifdef _M_IX86
  image = IMAGE_FILE_MACHINE_I386;
  stackframe.AddrPC.Offset = context.Eip;
  stackframe.AddrPC.Mode = AddrModeFlat;
  stackframe.AddrFrame.Offset = context.Ebp;
  stackframe.AddrFrame.Mode = AddrModeFlat;
  stackframe.AddrStack.Offset = context.Esp;
  stackframe.AddrStack.Mode = AddrModeFlat;
#elif _M_X64
  image = IMAGE_FILE_MACHINE_AMD64;
  stackframe.AddrPC.Offset = context.Rip;
  stackframe.AddrPC.Mode = AddrModeFlat;
  stackframe.AddrFrame.Offset = context.Rsp;
  stackframe.AddrFrame.Mode = AddrModeFlat;
  stackframe.AddrStack.Offset = context.Rsp;
  stackframe.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64
  image = IMAGE_FILE_MACHINE_IA64;
  stackframe.AddrPC.Offset = context.StIIP;
  stackframe.AddrPC.Mode = AddrModeFlat;
  stackframe.AddrFrame.Offset = context.IntSp;
  stackframe.AddrFrame.Mode = AddrModeFlat;
  stackframe.AddrBStore.Offset = context.RsBSP;
  stackframe.AddrBStore.Mode = AddrModeFlat;
  stackframe.AddrStack.Offset = context.IntSp;
  stackframe.AddrStack.Mode = AddrModeFlat;
#endif

  print_to($(File, stderr), 0, "!!\tStack Trace: \n");
  print_to($(File, stderr), 0, "!!\t\n");

  for (size_t i = 0; i < CELLO_EXC_MAX_STRACE; i++) {
    
    BOOL result = StackWalk64(
      image, process, thread,
      &stackframe, &context, NULL, 
      SymFunctionTableAccess64, SymGetModuleBase64, NULL);
    
    if (!result) { break; }
    
    char* filename = "";
    char* symbolname = "???";
    int lineno = 0;
    
    char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
    PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = MAX_SYM_NAME;
    
    DWORD64 displacement = 0;
    if (SymFromAddr(process, stackframe.AddrPC.Offset, &displacement, symbol)) {
      symbolname = symbol->Name;
    } else {
      symbolname = "???";
    }
      
    IMAGEHLP_LINE64 line;
    line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    
    DWORD displacementline = 0;
    if (SymGetLineFromAddr64(process, 
      stackframe.AddrPC.Offset, &displacementline, &line)) {
      lineno = line.LineNumber;
      filename = line.FileName;
    } else {
      lineno = 0;
      filename = "";
    }
    
    if (strcmp(filename, "") == 0) {
      print_to($(File, stderr), 0, "!!\t\t[%i] %s\n",
        $I(i), $S(symbolname));      
    } else {
      print_to($(File, stderr), 0, "!!\t\t[%i] %s:%i %s\n",
        $I(i), $S(filename), $I(lineno), $S(symbolname));
    }
    
  }
  
  print_to($(File, stderr), 0, "!!\t\n");
  
  SymCleanup(process);
  
}

#else

static void Exception_Backtrace(void) {}

#endif

#else

static void Exception_Backtrace(void) {}
  
#endif

static void Exception_Error(void)  {
  
  struct Thread* t = Thread_Current();
  
  print_to($(File, stderr), 0, "\n");
  print_to($(File, stderr), 0, "!!\t\n");
  print_to($(File, stderr), 0, "!!\tUncaught %$\n", exception_object());
  print_to($(File, stderr), 0, "!!\t\n");
  print_to($(File, stderr), 0, "!!\t\t %s\n", exception_message());
  print_to($(File, stderr), 0, "!!\t\n");
  
  Exception_Backtrace();
  
  exit(EXIT_FAILURE);
  
}

var exception_throw(var obj, const char* fmt, var args) {

  struct Thread* t = Thread_Current();
  
  t->exc_obj = obj;
  
  if (t->exc_msg is NULL) {
    t->exc_msg = new_raw(String);
  }
  
  print_to_with(t->exc_msg, 0, fmt, args);
  
  if (exception_depth() >= 1) {
    longjmp(*exception_buffer(), 1);
  } else {
    Exception_Error();
  }
  
  return NULL;
  
}

var exception_catch(var args) {
  
  if (not exception_active()) { return NULL; }
  
  /* If no Arguments catch all */
  if (len(args) is 0) {
    return exception_object();
  }
  
  /* Check Exception against Arguments */
  foreach(arg in args) {
    if (eq(arg, exception_object())) {
      return exception_object();
    }
  }
  
  /* No matches found. Propagate to outward block */
  if (exception_depth() >= 1) {
    longjmp(*exception_buffer(), 1);
  } else {
    Exception_Error();
  }
  
  return NULL;
  
}
