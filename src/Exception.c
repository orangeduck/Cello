#include "Cello.h"

#define EXCEPTION_TLS_KEY "__Exception"

enum {
  EXCEPTION_MAX_DEPTH  = 2048,
  EXCEPTION_MAX_STRACE = 25
};

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

struct Exception {
  var      obj;
  var      msg;
  size_t   depth;
  bool     active;
  jmp_buf* buffers[EXCEPTION_MAX_DEPTH];  
};

static const char* Exception_Name(void) {
  return "Exception";
}

static const char* Exception_Brief(void) {
  return "Exception Object";
}

static const char* Exception_Description(void) {
  return
    "The `Exception` type provides an interface to the Cello Exception System. "
    "One instance of this type is created for each `Thread` and stores the "
    "various bits of data required for the exception system. It can be "
    "retrieved using the `current` function, although not much can be done "
    "with it."
    "\n\n"
    "Exceptions are available via the `try`, `catch` and `throw` macros. It is "
    "important that the `catch` part of the exception block is always "
    "evaluated otherwise the internal state of the exception system can go out "
    "of sync. For this reason please never use `return` inside a `try` block. "
    "\n\n"
    "The `exception_signals` method can be used to register some exception to "
    "be thrown for any of the "
    "[standard C signals](https://en.wikipedia.org/wiki/C_signal_handling)."
    "\n\n"
    "To get the current exception object or message use the "
    "`exception_message` or `exception_object` methods.";
}

static struct Method* Exception_Methods(void) {
  
  static struct Method methods[] = {
    {
      "try", 
      "#define try",
      "Start an exception `try` block."
    }, {
      "catch", 
      "#define catch(...)",
      "Start an exception `catch` block, catching any objects listed in `...` "
      "as the first name given. To catch any exception object leave argument "
      "list empty other than caught variable name."
    }, {
      "#define throw",
      "throw(E, F, ...)",
      "Throw exception object `E` with format string `F` and arguments `...`."
    }, {
      "exception_signals",
      "void exception_signals(void);",
      "Register the standard C signals to throw corresponding exceptions."
    }, {
      "exception_object",
      "void exception_object(void);\n",
      "Retrieve the current exception object."
    }, {
      "exception_message",
      "void exception_message(void);\n",
      "Retrieve the current exception message."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

static struct Example* Exception_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var x = new(Table, String, Int);\n"
      "set(x, $S(\"Hello\"), $I(1));\n"
      "set(x, $S(\"World\"), $I(2));\n"
      "\n"
      "try {\n"
      "  get(x, $S(\"Missing\"));\n"
      "} catch (e in KeyError) {\n"
      "  println(\"Got Exception: %$\", e);\n"
      "}\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static void Exception_New(var self, var args) {
  struct Exception* e = self;
  e->active = false;
  e->depth = 0;
  e->obj = NULL;
  e->msg = new_raw(String);
  memset(e->buffers, 0, sizeof(jmp_buf*) * EXCEPTION_MAX_DEPTH);
  set(current(Thread), $S(EXCEPTION_TLS_KEY), self);
}

static void Exception_Del(var self) {
  struct Exception* e = self;
  del_raw(e->msg);
  rem(current(Thread), $S(EXCEPTION_TLS_KEY));
}

static void Exception_Assign(var self, var obj) {
  struct Exception* e = self;
  struct Exception* o = cast(obj, Exception);
  e->obj = o->obj;
  assign(e->msg, o->msg);
  e->depth = o->depth;
  e->active = o->active;
  memcpy(e->buffers, o->buffers, sizeof(jmp_buf*) * EXCEPTION_MAX_DEPTH);
}

static var Exception_Current(void) {
  return get(current(Thread), $S(EXCEPTION_TLS_KEY));
}

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

static jmp_buf* Exception_Buffer(struct Exception* e) {
  if (e->depth == 0) {
    fprintf(stderr, "Cello Fatal Error: Exception Buffer Out of Bounds!\n");
    abort();
  }
  return e->buffers[e->depth-1];
}

static size_t Exception_Len(var self) {
  struct Exception* e = self;
  return e->depth;
}

static bool Exception_Running(var self) {
  struct Exception* e = self;
  return e->active;
}

#ifndef CELLO_NSTRACE
#if defined(CELLO_UNIX)

static void Exception_Backtrace(void) {
  
  var trace[EXCEPTION_MAX_STRACE];
  size_t trace_count = backtrace(trace, EXCEPTION_MAX_STRACE);
  char** symbols = backtrace_symbols(trace, trace_count);  
  
  print_to($(File, stderr), 0, "!!\tStack Trace: \n");
  print_to($(File, stderr), 0, "!!\t\n");

  for (size_t i = 0; i < trace_count; i++) {
    print_to($(File, stderr), 0, "!!\t\t[%i] %s\n", 
      $(Int, i), $(String, symbols[i]));
  }
  print_to($(File, stderr), 0, "!!\t\n");
  
  free(symbols);
  
}

#elif defined(CELLO_WINDOWS)

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

  for (size_t i = 0; i < EXCEPTION_MAX_STRACE; i++) {
    
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

static void Exception_Error(struct Exception* e)  {
  
  print_to($(File, stderr), 0, "\n");
  print_to($(File, stderr), 0, "!!\t\n");
  print_to($(File, stderr), 0, "!!\tUncaught %$\n", e->obj);
  print_to($(File, stderr), 0, "!!\t\n");
  print_to($(File, stderr), 0, "!!\t\t %s\n", e->msg);
  print_to($(File, stderr), 0, "!!\t\n");
  
  Exception_Backtrace();
  
  exit(EXIT_FAILURE);
  
}

static int Exception_Show(var self, var out, int pos) {
  struct Exception* e = self;
  return print_to(out, pos, 
    "<'Exception' At 0x%p %$ - %$>", self, e->obj, e->msg);
}

var Exception = Cello(Exception,
  Instance(Doc,
    Exception_Name, Exception_Brief,    Exception_Description, 
    NULL,           Exception_Examples, Exception_Methods),
  Instance(New,     Exception_New, Exception_Del),
  Instance(Assign,  Exception_Assign),
  Instance(Len,     Exception_Len),
  Instance(Current, Exception_Current),
  Instance(Start,   NULL, NULL, NULL, Exception_Running),
  Instance(Show,    Exception_Show, NULL));

void exception_signals(void) {
  signal(SIGABRT, Exception_Signal);
  signal(SIGFPE,  Exception_Signal);
  signal(SIGILL,  Exception_Signal);
  signal(SIGINT,  Exception_Signal);
  signal(SIGSEGV, Exception_Signal);
  signal(SIGTERM, Exception_Signal);
}

void exception_try(jmp_buf* env) {
  struct Exception* e = current(Exception);
  if (e->depth is EXCEPTION_MAX_DEPTH) {
    fprintf(stderr, "Cello Fatal Error: Exception Buffer Overflow!\n");
    abort();
  }
  e->depth++;
  e->active = false; 
  e->buffers[e->depth-1] = env;
}

var exception_throw(var obj, const char* fmt, var args) {

  struct Exception* e = current(Exception);
  
  e->obj = obj;
  print_to_with(e->msg, 0, fmt, args);
  
  if (Exception_Len(e) >= 1) {
    longjmp(*Exception_Buffer(e), 1);
  } else {
    Exception_Error(e);
  }
  
  return NULL;
  
}

var exception_catch(var args) {
  
  struct Exception* e = current(Exception);
  
  if (not e->active) { return NULL; }
  
  /* If no Arguments catch all */
  if (len(args) is 0) {
    return e->obj;
  }
  
  /* Check Exception against Arguments */
  foreach(arg in args) {
    if (eq(arg, e->obj)) {
      return e->obj;
    }
  }
  
  /* No matches found. Propagate to outward block */
  if (e->depth >= 1) {
    longjmp(*Exception_Buffer(e), 1);
  } else {
    Exception_Error(e);
  }
  
  return NULL;
  
}

void exception_try_end(void) {
  struct Exception* e = current(Exception);
  if (e->depth == 0) {
    fprintf(stderr, "Cello Fatal Error: Exception Buffer Underflow!\n");
    abort();
  }
  e->depth--;
}

void exception_try_fail(void) {
  struct Exception* e = current(Exception);
  e->active = true;
}
