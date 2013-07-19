#include "Cello/Exception.h"

#include "Cello/Type.h"
#include "Cello/None.h"
#include "Cello/File.h"
#include "Cello/String.h"
#include "Cello/Number.h"

#include <signal.h>

var TypeError = Singleton(TypeError);
var ValueError = Singleton(ValueError);
var ClassError = Singleton(ClassError);
var IndexOutOfBoundsError = Singleton(IndexOutOfBoundsError);
var KeyError = Singleton(KeyError);
var OutOfMemoryError = Singleton(OutOfMemoryError);
var IOError = Singleton(IOError);
var FormatError = Singleton(FormatError);

var ProgramAbortedError = Singleton(ProgramAbortedError);
var DivisionByZeroError = Singleton(DivisionByZeroError);
var IllegalInstructionError = Singleton(IllegalInstructionError);
var ProgramInterruptedError = Singleton(ProgramInterruptedError);
var SegmentationError = Singleton(SegmentationError);
var ProgramTerminationError = Singleton(ProgramTerminationError);

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

void Exception_Register_Signals(void) {
  signal(SIGABRT, Exception_Signal);
  signal(SIGFPE,  Exception_Signal);
  signal(SIGILL,  Exception_Signal);
  signal(SIGINT,  Exception_Signal);
  signal(SIGSEGV, Exception_Signal);
  signal(SIGTERM, Exception_Signal);
}

__thread bool __exc_active = false;
__thread int __exc_depth = -1;
__thread jmp_buf __exc_buffers[__EXC_MAX_DEPTH];

local var __exc_obj = NULL;
local bool __exc_msg_hook = false;
local void* __exc_msg = NULL;
local const char* __exc_file = "";
local const char* __exc_func = "";
local unsigned int __exc_lineno = 0;

#ifdef __unix__
#include <execinfo.h>

local void* __exc_backtrace[25];
local int __exc_backtrace_count;

#endif

local void __exc_error(void)  {
  
  print_to($(File, stderr), 0, "\n");
  print_to($(File, stderr), 0, "!!\t\n");
  print_to($(File, stderr), 0, "!!\tUncaught %$ at (%s:%s:%i) \n", __exc_obj, $(String, (char*)__exc_file), $(String, (char*)__exc_func), $(Int, __exc_lineno));
  print_to($(File, stderr), 0, "!!\t\n");
  print_to($(File, stderr), 0, "!!\t\t %s\n", $(String, __exc_msg));
  print_to($(File, stderr), 0, "!!\t\n");
  
#ifdef __unix__
  
  print_to($(File, stderr), 0, "!!\tStack Trace: \n");
  print_to($(File, stderr), 0, "!!\t\n");
  
  char** symbols = backtrace_symbols(__exc_backtrace, __exc_backtrace_count);
  
  for (int i = 0; i < __exc_backtrace_count; i++){
    print_to($(File, stderr), 0, "!!\t\t[%i] %s\n", $(Int, i), $(String, symbols[i]));
  }
  
  print_to($(File, stderr), 0, "!!\t\n");
  
  free(symbols);
  
#endif
  
  exit(EXIT_FAILURE);
  
}

local void __exc_free_msg(void) {
  free(__exc_msg);
}

var __exc_throw(var obj, const char* fmt, const char* file, const char* func, int lineno, ...) {
  
  __exc_obj = obj;
  __exc_file = file;
  __exc_func = func;
  __exc_lineno = lineno;
  
#ifdef __unix__
  __exc_backtrace_count = backtrace(__exc_backtrace, 25);
#endif
  
  var exc_msg = $(String, __exc_msg);

  va_list va;
  va_start(va, lineno);
  print_to_va(exc_msg, 0, fmt, va);
  va_end(va);
  
  __exc_msg = (char*)as_str(exc_msg);
  if (!__exc_msg_hook) {
    __exc_msg_hook = true;
    atexit(__exc_free_msg);
  }
  
  if (__exc_depth >= 0) {
    longjmp(__exc_buffers[__exc_depth], 1);
  } else {
    __exc_error();
  }
  
  return Undefined;
  
}

var __exc_catch(void* unused, ...) {
  
  if (!__exc_active) { return Undefined; }
  
  va_list va;
  va_start(va, unused);
  var e = va_arg(va, var);
  
  /* If no Arguments catch all */
  if (e is Undefined) {
    va_end(va);
    return __exc_obj;
  }
  
  /* Check Exception against Arguments */
  while (e != Undefined) {
    if_eq(e, __exc_obj) {
      va_end(va);
      return __exc_obj;
    }
    e = va_arg(va, var);
  }
  
  va_end(va);
  
  /* No matches found. Propagate to outward block */
  if (__exc_depth >= 0) {
    longjmp(__exc_buffers[__exc_depth], 1);
  } else {  
    __exc_error();
  }
  
  return Undefined;
  
}
