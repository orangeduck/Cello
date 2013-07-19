/*
** == Format ==
**
**  Provides an interface for "printing" and "scanning"
**  which works with rich objects.
**
**  The semantics of the "Format" class are similar to
**  "printf" and "scanf". The 'pos' variable is meant to 
**  represent the position at which the formatting takes
**  place but can be ignored by objects which maintain
**  their own state such as "File".
**
**  On the other hand it is essential for objects such
**  as strings.
**
**  The format specifier for rich objects is '%$'. It
**  is important you _only_ pass rich objects to the
**  "Show" functions. Otherwise they will segfault.
**
**  Another important thing to note is that the return
**  value of the "Format" functions and the "Show"
**  functions differs.
**
**  "format_to" returns the number of characters written
**  "format_from" returns the number of arguments scanned
**
**  "show", "look", "print", "scan" all return the updated
**  version of the 'pos' variable, with added to it the
**  number of character either written or scanned.
*/
#ifndef CelloFormat_h
#define CelloFormat_h

#include "Prelude.h"

/** Format - Writable using formatted data */

class {
  int (*format_to)(var,int,const char*,va_list);
  int (*format_from)(var,int,const char*,va_list);
} Format;

int format_to_va(var self, int pos, const char* fmt, va_list va);
int format_from_va(var self, int pos, const char* fmt, va_list va);

int format_to(var self, int pos, const char* fmt, ...);
int format_from(var self, int pos, const char* fmt, ...);

/** Show - Conversion to/from string */

class {
  int (*show)(var,var,int);
  int (*look)(var,var,int);
} Show;

int show(var self);
int show_to(var self, var out, int pos);

int print(const char* fmt, ...);
int println(const char* fmt, ...);
int print_to(var out, int pos, const char* fmt, ...);

int print_va(const char* fmt, va_list va);
int print_to_va(var out, int pos, const char* fmt, va_list va);

int look(var self);
int look_from(var self, var input, int pos);

int scan(const char* fmt, ...);
int scanln(const char* fmt, ...);
int scan_from(var input, int pos, const char* fmt, ...);

int scan_va(const char* fmt, va_list va);
int scan_from_va(var input, int pos, const char* fmt, va_list va);

#endif
