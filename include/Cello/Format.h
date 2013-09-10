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
**  The format specifier for rich objects is '%$'. The
**  "Show" functions will only accept rich objects.
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

#define print(fmt, ...) print_vl(fmt, var_list_new(__VA_ARGS__))
#define println(fmt, ...) println_vl(fmt, var_list_new(__VA_ARGS__))
#define print_to(out, pos, fmt, ...) print_to_vl(out, pos, fmt, var_list_new(__VA_ARGS__))

int print_vl(const char* fmt, var_list vl);
int println_vl(const char* fmt, var_list vl);
int print_to_vl(var out, int pos, const char* fmt, var_list vl);

int look(var self);
int look_from(var self, var input, int pos);

#define scan(fmt, ...) scan_vl(fmt, var_list_new(__VA_ARGS__))
#define scanln(fmt, ...) scanln_vl(fmt, var_list_new(__VA_ARGS__))
#define scan_from(input, pos, fmt, ...) scan_from_vl(input, pos, fmt, var_list_new(__VA_ARGS__))

int scan_vl(const char* fmt, var_list vl);
int scanln_vl(const char* fmt, var_list vl);
int scan_from_vl(var input, int pos, const char* fmt, var_list vl);

#endif
