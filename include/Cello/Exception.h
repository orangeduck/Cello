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
global var BusyError;
global var ResourceError;

global var ProgramAbortedError;
global var DivisionByZeroError;
global var IllegalInstructionError;
global var ProgramInterruptedError;
global var SegmentationError;
global var ProgramTerminationError;

/* Register Signals as Exceptions */

void Exception_Register_Signals(void);

/* Internal Exception Stuff */

void Exception_Inc(void);
void Exception_Dec(void);
bool Exception_Active(void);
void Exception_Activate(void);
void Exception_Deactivate(void);

var Exception_Object(void);
var Exception_Message(void);
int  Exception_Depth(void);
void* Exception_Buffer(void);

var Exception_Throw(var obj, const char* fmt, const char* file, const char* func, int lineno, var_list vl);
var Exception_Catch(var_list vl);

/* Exception Macros */

#define try Exception_Inc(); Exception_Deactivate(); if (!setjmp(Exception_Buffer()))

#define catch(...) catch_scanned(__VA_ARGS__)
#define catch_scanned(x, ...) else { Exception_Activate(); } Exception_Dec(); for (var x = Exception_Catch(var_list_new(__VA_ARGS__)); x != Undefined; x = Undefined)
#define throw(e, fmt, ...) Exception_Throw(e, fmt, __FILE__, __func__, __LINE__, var_list_new(__VA_ARGS__))


#endif
