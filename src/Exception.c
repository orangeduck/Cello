#include "Cello/Exception.h"

#include "Cello/Type.h"
#include "Cello/None.h"
#include "Cello/File.h"
#include "Cello/String.h"
#include "Cello/Number.h"
#include "Cello/Thread.h"

#include <signal.h>

var TypeError = Singleton(TypeError);
var ValueError = Singleton(ValueError);
var ClassError = Singleton(ClassError);
var IndexOutOfBoundsError = Singleton(IndexOutOfBoundsError);
var KeyError = Singleton(KeyError);
var OutOfMemoryError = Singleton(OutOfMemoryError);
var IOError = Singleton(IOError);
var FormatError = Singleton(FormatError);
var BusyError = Singleton(BusyError);
var ResourceError = Singleton(ResourceError);

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

void Exception_Inc(void) {
  ThreadData* td = current(Thread);
  if (td->exc_depth == EXC_MAX_DEPTH) {
    fprintf(stderr, "Cello Fatal Error: Exception Buffer Overflow!\n"); abort();
  }
  td->exc_depth++;
}

void Exception_Dec(void) {
  ThreadData* td = current(Thread);
  if (td->exc_depth == 0) {
    fprintf(stderr, "Cello Fatal Error: Exception Buffer Underflow!\n"); abort();
  }
  td->exc_depth--;
}

bool Exception_Active(void) {
  ThreadData* td = current(Thread);
  return td->exc_active;
}

void Exception_Activate(void) {
  ThreadData* td = current(Thread);
  td->exc_active = true;
}

void Exception_Deactivate(void) {
  ThreadData* td = current(Thread);
  td->exc_active = false;  
}

var Exception_Object(void) {
  ThreadData* td = current(Thread);
  return td->exc_obj;  
}

var Exception_Message(void) {
  ThreadData* td = current(Thread);
  return td->exc_msg;
}

void* Exception_Buffer(void) {
  ThreadData* td = current(Thread);
  if (td->exc_depth == 0) {
    fprintf(stderr, "Cello Fatal Error: Exception Buffer Out of Bounds!\n"); abort();
  }
  return td->exc_buffers[td->exc_depth-1];
}

int Exception_Depth(void) {
  ThreadData* td = current(Thread);
  return td->exc_depth;
}

#if defined(__unix__) || defined(__APPLE__)
#include <execinfo.h>
#endif

local void Exception_Error(void)  {
  
  ThreadData* td = current(Thread);
  
  print_to($(File, stderr), 0, "\n");
  print_to($(File, stderr), 0, "!!\t\n");
  print_to($(File, stderr), 0, "!!\tUncaught %$ at (%s:%s:%i) \n", Exception_Object(), $(String, (char*)td->exc_file), $(String, (char*)td->exc_func), $(Int, td->exc_lineno));
  print_to($(File, stderr), 0, "!!\t\n");
  print_to($(File, stderr), 0, "!!\t\t %s\n", Exception_Message());
  print_to($(File, stderr), 0, "!!\t\n");
  
  print_to($(File, stderr), 0, "!!\tStack Trace: \n");
  print_to($(File, stderr), 0, "!!\t\n");

#if defined(__unix__) || defined(__APPLE__)
  char** symbols = backtrace_symbols(td->exc_backtrace, td->exc_backtrace_count);  
  
  for (int i = 0; i < td->exc_backtrace_count; i++) {
    print_to($(File, stderr), 0, "!!\t\t[%i] %s\n", $(Int, i), $(String, symbols[i]));
  }
  print_to($(File, stderr), 0, "!!\t\n");
  
  free(symbols);
#endif
  
  exit(EXIT_FAILURE);
  
}

local var main_exc_msg = NULL;
local void main_exc_msg_free(void) {
  delete(main_exc_msg);
}

var Exception_Throw(var obj, const char* fmt, const char* file, const char* func, int lineno, var_list vl) {

  ThreadData* td = current(Thread);
#if defined(__unix__) || defined(__APPLE__)
  td->exc_backtrace_count = backtrace(td->exc_backtrace, 25);
#endif
  td->exc_obj = obj;
  td->exc_file = file;
  td->exc_func = func;
  td->exc_lineno = lineno;
  
  if ((td->is_main) and (main_exc_msg == NULL)) {
    main_exc_msg = new(String, $(String, ""));
    td->exc_msg = main_exc_msg;
    atexit(main_exc_msg_free);
  }
  
  print_to_vl(td->exc_msg, 0, fmt, vl);
  
  if (Exception_Depth() >= 1) {
    longjmp(Exception_Buffer(), 1);
  } else {
    Exception_Error();
  }
  
  return Undefined;
  
}

var Exception_Catch(var_list vl) {
  
  if (not Exception_Active()) { return Undefined; }
  
  /* If no Arguments catch all */
  if (var_list_end(vl)) {
    return Exception_Object();
  }
  
  /* Check Exception against Arguments */
  while (not var_list_end(vl)) {
    if_eq(var_list_get(vl), Exception_Object()) {
      return Exception_Object();
    }
  }
  
  /* No matches found. Propagate to outward block */
  if (Exception_Depth() >= 1) {
    longjmp(Exception_Buffer(), 1);
  } else {  
    Exception_Error();
  }
  
  return Undefined;
  
}
