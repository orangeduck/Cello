#include "Cello.h"

static const char* Current_Name(void) {
  return "Current";
}

/* TODO */
static const char* Current_Brief(void) {
  return "";
}

/* TODO */
static const char* Current_Description(void) {
  return "";
}

/* TODO */
static const char* Current_Examples(void) {
  return "";
}

/* TODO */
static const char* Current_Methods(void) {
  return "";
}

var Current = Cello(Current,
  Member(Doc,
    Current_Name, Current_Brief, Current_Description,
    Current_Examples, Current_Methods));

var current(var type) {
  return type_method(type, Current, current);
}

static const char* Join_Name(void) {
  return "Join";
}

/* TODO */
static const char* Join_Brief(void) {
  return "";
}

/* TODO */
static const char* Join_Description(void) {
  return "";
}

/* TODO */
static const char* Join_Examples(void) {
  return "";
}

/* TODO */
static const char* Join_Methods(void) {
  return "";
}

var Join = Cello(Join,
  Member(Doc,
    Join_Name, Join_Brief, Join_Description,
    Join_Examples, Join_Methods));

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

  var is_main;
  var is_running;
  
  var     exc_obj;
  var     exc_msg;
  var     exc_active;
  size_t  exc_depth;
  jmp_buf exc_buffers[CELLO_EXC_MAX_DEPTH];
  void*   exc_backtrace[CELLO_EXC_MAX_STRACE];
  size_t  exc_backtrace_count;
  
};

static const char* Thread_Name(void) {
  return "Thread";
}

/* TODO */
static const char* Thread_Brief(void) {
  return "";
}

/* TODO */
static const char* Thread_Description(void) {
  return "";
}

/* TODO */
static const char* Thread_Examples(void) {
  return "";
}

/* TODO */
static const char* Thread_Methods(void) {
  return "";
}

static var Thread_New(var self, var args) {
  
  struct Thread* t = self;
  t->func = get(args, $(Int, 0));
  t->args = None;
  t->is_main = false;
  t->is_running = false;
  
  t->exc_active = false;
  t->exc_depth = 0;
  memset(t->exc_buffers, 0, sizeof(jmp_buf) * CELLO_EXC_MAX_DEPTH);
  
  t->exc_obj = Undefined;
  t->exc_msg = new(String, $(String, ""));
  memset(t->exc_backtrace, 0, sizeof(void*) * CELLO_EXC_MAX_STRACE);
  t->exc_backtrace_count = 0;
  
  return t;
}

static var Thread_Del(var self) {
  struct Thread* t = self;
  
#ifdef _WIN32
  CloseHandle(t->thread);
#endif
  
  if (t->args isnt None) { del(t->args); }
  if (t->exc_msg isnt None) { del(t->exc_msg); }
  return t;
}

static size_t Thread_Size(void) {
  return sizeof(struct Thread);
}

static var Thread_Assign(var self, var obj) {
  
  struct Thread* t0 = self;
  struct Thread* t1 = obj;
  
  t0->func = t1->func;
  t0->args = t1->args;
  
  t0->thread = t1->thread;
  t0->is_main = t1->is_main;
  t0->is_running = t1->is_running;
  
  t0->exc_active = t1->exc_active;
  t0->exc_depth = t1->exc_depth;
  memcpy(t0->exc_buffers, t1->exc_buffers,
    sizeof(jmp_buf) * CELLO_EXC_MAX_DEPTH);
  
  assign(t0->exc_obj, t1->exc_obj);
  assign(t0->exc_msg, t1->exc_msg);
  
  memcpy(t0->exc_backtrace, t1->exc_backtrace,
    sizeof(void*) * CELLO_EXC_MAX_STRACE);
  t0->exc_backtrace_count = t1->exc_backtrace_count;
  
  return self;
}

static var Thread_Copy(var self) {
  return assign(new(Thread, None), self);
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

static var Thread_Eq(var self, var obj) {
  return bool_var(c_int(self) == c_int(obj));
}

static var Thread_Gt(var self, var obj) {
  return bool_var(c_int(self) > c_int(obj));
}

static var Thread_Lt(var self, var obj) {
  return bool_var(c_int(self) < c_int(obj));
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
  t->is_running = True;
  var x = call_with(t->func, t->args);
  del(t->args);
  t->args = None;
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
  t->is_running = True;
  call_with(t->func, t->args);
  del(t->args);
  t->args = None;
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
  
  t->args = copy(args);
  
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
  del(Thread_Main);
}

var Thread_Current(void) {
  
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
  if (pthread_main_np()) { wrapper = None; }
#endif
  
  if (wrapper is None) {
  
    if (Thread_Main is None) {
      Thread_Main = new(Thread, None);
      atexit(Thread_Main_Del);
    }
    
    struct Thread* t = Thread_Main;
    t->is_main = True;
    t->is_running = True;
    
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
  if (err is EINVAL) { throw(ValueError, "Invalid Argument to Thread Kill"); }
  if (err is ESRCH)  { throw(ValueError, "Invalid Thread"); }
#elif defined(_WIN32)
  TerminateThread(t->thread, FALSE);
#endif  
  
}

static var Thread_Running(var self) {
  struct Thread* t = self;
  return t->is_running;
}

var Thread = Cello(Thread,
  Member(Doc,
    Thread_Name, Thread_Brief, Thread_Description, 
    Thread_Examples, Thread_Methods),
  Member(New, Thread_New, Thread_Del, Thread_Size),
  Member(Assign,  Thread_Assign),
  Member(Copy,    Thread_Copy),
  Member(Call,    Thread_Call),
  Member(Current, Thread_Current),
  Member(Join,    Thread_Join),
  Member(Start,   Thread_Start, Thread_Stop, Thread_Running),
  Member(C_Int,   Thread_C_Int));

static const char* Lock_Name(void) {
  return "Lock";
}

/* TODO */
static const char* Lock_Brief(void) {
  return "";
}

/* TODO */
static const char* Lock_Description(void) {
  return "";
}

/* TODO */
static const char* Lock_Examples(void) {
  return "";
}

/* TODO */
static const char* Lock_Methods(void) {
  return "";
}

var Lock = Cello(Lock,
  Member(Doc,
    Lock_Name, Lock_Brief, Lock_Description,
    Lock_Examples, Lock_Methods));

void lock(var self) {
  method(self, Lock, lock);
}

void unlock(var self) {
  method(self, Lock, unlock);
}

var lock_try(var self) {
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

/* TODO */
static const char* Mutex_Brief(void) {
  return "";
}

/* TODO */
static const char* Mutex_Description(void) {
  return "";
}

/* TODO */
static const char* Mutex_Examples(void) {
  return "";
}

/* TODO */
static const char* Mutex_Methods(void) {
  return "";
}

static var Mutex_New(var self, var args) {
  struct Mutex* m = self;
#if defined(__unix__) || defined(__APPLE__)
  pthread_mutex_init(&m->mutex, NULL);
#elif defined(_WIN32)
  m->mutex = CreateMutex(NULL, false, NULL);
#endif
  return m;
}

static var Mutex_Del(var self) {
  struct Mutex* m = self;
#if defined(__unix__) || defined(__APPLE__)
  pthread_mutex_destroy(&m->mutex);
#elif defined(_WIN32)
  CloseHandle(m->mutex);
#endif
  return m;
}

static size_t Mutex_Size(void) {
  return sizeof(struct Mutex);
}

static var Mutex_Assign(var self, var obj) {
  struct Mutex* m0 = cast(self, Mutex);
  struct Mutex* m1 = cast(self, Mutex);
  m0->mutex = m1->mutex;
  return self;
}

static var Mutex_Copy(var self) {
  var obj = new(Mutex);
  assign(obj, self);
  return obj;
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

static var Mutex_Lock_Try(var self) {
  struct Mutex* m = self;
#if defined(__unix__) || defined(__APPLE__)
  int err = pthread_mutex_trylock(&m->mutex);
  if (err == EBUSY) { return False; }
  if (err is EINVAL) {
    throw(ValueError, "Invalid Argument to Mutex Lock Try");
  }
  return True;
#elif defined(_WIN32)
  return bool_var(not (WaitForSingleObject(m->mutex, 0) is WAIT_TIMEOUT));
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
  Member(Doc, 
    Mutex_Name, Mutex_Brief, Mutex_Description, Mutex_Examples, Mutex_Methods),
  Member(New,    Mutex_New, Mutex_Del, Mutex_Size),
  Member(Assign, Mutex_Assign),
  Member(Copy,   Mutex_Copy),
  Member(Lock,   Mutex_Lock, Mutex_Unlock, Mutex_Lock_Try),
  Member(Start,  Mutex_Lock, Mutex_Unlock, NULL));

var TypeError = Cello(TypeError);
var ValueError = Cello(ValueError);
var ClassError = Cello(ClassError);
var IndexOutOfBoundsError = Cello(IndexOutOfBoundsError);
var KeyError = Cello(KeyError);
var OutOfMemoryError = Cello(OutOfMemoryError);
var IOError = Cello(IOError);
var FormatError = Cello(FormatError);
var BusyError = Cello(BusyError);
var ResourceError = Cello(ResourceError);

var ProgramAbortedError = Cello(ProgramAbortedError);
var DivisionByZeroError = Cello(DivisionByZeroError);
var IllegalInstructionError = Cello(IllegalInstructionError);
var ProgramInterruptedError = Cello(ProgramInterruptedError);
var SegmentationError = Cello(SegmentationError);
var ProgramTerminationError = Cello(ProgramTerminationError);

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
  struct Thread* t = current(Thread);
  if (t->exc_depth is CELLO_EXC_MAX_DEPTH) {
    fprintf(stderr, "Cello Fatal Error: Exception Buffer Overflow!\n");
    abort();
  }
  t->exc_depth++;
}

void exception_dec(void) {
  struct Thread* t = current(Thread);
  if (t->exc_depth == 0) {
    fprintf(stderr, "Cello Fatal Error: Exception Buffer Underflow!\n");
    abort();
  }
  t->exc_depth--;
}

var exception_active(void) {
  struct Thread* t = current(Thread);
  return t->exc_active;
}

void exception_activate(void) {
  struct Thread* t = current(Thread);
  t->exc_active = True;
}

void exception_deactivate(void) {
  struct Thread* t = current(Thread);
  t->exc_active = False;  
}

var exception_object(void) {
  struct Thread* t = current(Thread);
  return t->exc_obj;  
}

var exception_message(void) {
  struct Thread* t = current(Thread);
  return t->exc_msg;
}

var exception_buffer(void) {
  struct Thread* t = current(Thread);
  if (t->exc_depth == 0) {
    fprintf(stderr, "Cello Fatal Error: Exception Buffer Out of Bounds!\n");
    abort();
  }
  return t->exc_buffers[t->exc_depth-1];
}

size_t exception_depth(void) {
  struct Thread* t = current(Thread);
  return t->exc_depth;
}

#if defined(__unix__) || defined(__APPLE__)

static void Exception_Backtrace(void) {
  
  struct Thread* t = current(Thread);
  
  t->exc_backtrace_count = backtrace(t->exc_backtrace, CELLO_EXC_MAX_STRACE);
  char** symbols = backtrace_symbols(t->exc_backtrace, t->exc_backtrace_count);  
  
  for (int i = 0; i < t->exc_backtrace_count; i++) {
    print_to($(File, stderr), 0, "!!\t\t[%i] %s\n", 
      $(Int, i), $(String, symbols[i]));
  }
  print_to($(File, stderr), 0, "!!\t\n");
  
  free(symbols);
  
}

#elif defined(_WIN32)

static void Exception_Backtrace(void) {
  
  /*
  HANDLE process = GetCurrentProcess();
  HANDLE thread = GetCurrentThread();

  CONTEXT context;
  context.ContextFlags = CONTEXT_CONTROL;
  GetThreadContext(thread, &context);

  DWORD image = IMAGE_FILE_MACHINE_AMD64;
  
  STACKFRAME64 stackframe;
  ZeroMemory(&stackframe, sizeof(STACKFRAME64));
  stackframe.AddrPC.Offset = context.Rip;
  stackframe.AddrPC.Mode = AddrModeFlat;
  stackframe.AddrFrame.Offset = context.Rsp;
  stackframe.AddrFrame.Mode = AddrModeFlat;
  stackframe.AddrStack.Offset = context.Rbp;
  stackframe.AddrStack.Mode = AddrModeFlat;

  SymInitialize(process, NULL, TRUE);

  for (size_t i = 0; i < 10; i++) {
    BOOL result = StackWalk64(
      image, process, thread,
      &stackframe, &context,
      NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL);
    
    if (!result) { break; }
    
    // SymFromAddr 
    
    unsigned char buffer[sizeof(IMAGEHLP_SYMBOL64) + 256];
    PIMAGEHLP_SYMBOL64 symbol = (PIMAGEHLP_SYMBOL64)&buffer;
    ZeroMemory(symbol, sizeof(IMAGEHLP_SYMBOL64) + 256);
    symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
    symbol->MaxNameLength = 256;
    
    //IMAGEHLP_LINE64 line;
    //line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    
    if (SymGetSymFromAddr64(process, stackframe.AddrPC.Offset, NULL, symbol)) {
      print_to($(File, stderr), 0, "!!\t\t[%i] %s\n", 
        $(Int, i), $(String, symbol->Name));
    } else {
      printf("Error from SymGetSymFromAddr64: %lu.\n", GetLastError());
      print_to($(File, stderr), 0, "!!\t\t[%i] ???\n", $(Int, i));
    }
    
    i++;
  }
  
  print_to($(File, stderr), 0, "!!\t\n");
  
  SymCleanup(process);
  */
  
}

#else

static void Exception_Backtrace(void) {}

#endif

static void Exception_Error(void)  {
  
  struct Thread* t = current(Thread);
  
  print_to($(File, stderr), 0, "\n");
  print_to($(File, stderr), 0, "!!\t\n");
  print_to($(File, stderr), 0, "!!\tUncaught %$\n", exception_object());
  print_to($(File, stderr), 0, "!!\t\n");
  print_to($(File, stderr), 0, "!!\t\t %s\n", exception_message());
  print_to($(File, stderr), 0, "!!\t\n");
  
  print_to($(File, stderr), 0, "!!\tStack Trace: \n");
  print_to($(File, stderr), 0, "!!\t\n");
  
  Exception_Backtrace();
  
  exit(EXIT_FAILURE);
  
}

var exception_throw(var obj, const char* fmt, var args) {

  struct Thread* t = current(Thread);
  
  t->exc_obj = obj;
  print_to_with(t->exc_msg, 0, fmt, args);
  
  if (exception_depth() >= 1) {
    longjmp(exception_buffer(), 1);
  } else {
    Exception_Error();
  }
  
  return Undefined;
  
}

var exception_catch(var args) {
  
  if (not exception_active()) { return Terminal; }
  
  /* If no Arguments catch all */
  if (len(args) is 0) {
    return exception_object();
  }
  
  /* Check Exception against Arguments */
  foreach(arg in args) {
    if_eq(arg, exception_object()) {
      return exception_object();
    }
  }
  
  /* No matches found. Propagate to outward block */
  if (exception_depth() >= 1) {
    longjmp(exception_buffer(), 1);
  } else {
    Exception_Error();
  }
  
  return Terminal;
  
}
