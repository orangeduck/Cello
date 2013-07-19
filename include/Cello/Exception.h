/*
** == Exceptions ==
**
**  Exceptions in libCello follow fairly normal
**  semantics with one main difference.
**
**  Any object can be thrown and caught.
**
*/
#ifndef CelloException_H
#define CelloException_H

#include "Prelude.h"

global var TypeError;
global var ValueError;
global var ClassError;
global var IndexOutOfBoundsError;
global var KeyError;
global var OutOfMemoryError;
global var IOError;
global var FormatError;

global var ProgramAbortedError;
global var DivisionByZeroError;
global var IllegalInstructionError;
global var ProgramInterruptedError;
global var SegmentationError;
global var ProgramTerminationError;

/* Register Signals as Exceptions */

void Exception_Register_Signals(void);

/* Internal Exception Stuff */

enum {
  __EXC_MAX_DEPTH = 2048,
};

extern __thread bool __exc_active;
extern __thread int __exc_depth;
extern __thread jmp_buf __exc_buffers[__EXC_MAX_DEPTH];

var __exc_throw(var obj, const char* fmt, const char* file, const char* func, int lineno, ...);
var __exc_catch(void* unused, ...);

/* Exception Macros */

#define try \
  __exc_depth++; __exc_active = false; \
  if (__exc_depth == __EXC_MAX_DEPTH) { fprintf(stderr, "Maximum Exception Depth Exceeded!\n"); abort(); } \
  if (!setjmp(__exc_buffers[__exc_depth]))   

#define catch(...) catch_scanned(__VA_ARGS__)
#define catch_scanned(x, ...) else { __exc_active = true; } __exc_depth--; for (var x = __exc_catch(NULL, ##__VA_ARGS__, Undefined); x != Undefined; x = Undefined)
#define throw(e, fmt, ...) __exc_throw(e, fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)


#endif
