#include "Cello/Format.h"

#include "Cello/Type.h"
#include "Cello/Exception.h"
#include "Cello/File.h"
#include "Cello/Number.h"
#include "Cello/Char.h"
#include "Cello/Reference.h"

#include <string.h>

int format_to_va(var self, int pos, const char* fmt, va_list va) {
  return type_class_method(type_of(self), Format, format_to, self, pos, fmt, va);
}

int format_from_va(var self, int pos, const char* fmt, va_list va) {
  return type_class_method(type_of(self), Format, format_from, self, pos, fmt, va);
}

int format_to(var self, int pos, const char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  int ret = format_to_va(self, pos, fmt, va);
  va_end(va);
  return ret;
}

int format_from(var self, int pos, const char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  int ret = format_from_va(self, pos, fmt, va);
  va_end(va);
  return ret;
}

int show(var self) {
  return show_to(self, $(File, stdout), 0);
}

int show_to(var self, var out, int pos) {
  
  if (not type_implements(type_of(self), Show)) {
    return print_to(out, 0, "<'%s' At 0x%p>", type_of(self), self);
  } else {
    Show* ishow = type_class(type_of(self), Show);
    return ishow->show(self, out, pos);
  }
  
}

int print_vl(const char* fmt, var_list vl) {
  return print_to_vl($(File, stdout), 0, fmt, vl);
}

int println_vl(const char* fmt, var_list vl) {
  int ret = 0;
  ret = print_to_vl($(File, stdout), ret, fmt, vl);
  ret = print_to($(File, stdout), ret, "\n");
  return ret;
}

int print_to_vl(var out, int pos, const char* fmt, var_list vl) {

  char fmt_buf[strlen(fmt)+1]; 
  
  while(true) {
    
    if (*fmt == '\0') { break; }
    
    const char* start = fmt;
    
    /* Match String */
    while(!strchr("%\0", *fmt)) { fmt++; }
    
    if (start != fmt) {
      strncpy(fmt_buf, start, (fmt - start));
      fmt_buf[(fmt - start)] = '\0';
      int off = format_to(out, pos, fmt_buf);
      if (off < 0) { throw(FormatError, "Unable to output format!"); }
      pos += off;
      continue;
    }
    
    /* Match %% */
    if (*fmt == '%' && *(fmt+1) == '%') {
      int off = format_to(out, pos, "%%");
      if (off < 0) { throw(FormatError, "Unable to output '%%%%'!"); }
      pos += off;
      fmt += 2;
      continue;
    }
    
    /* Match Format Specifier */
    while(!strchr("diuoxXfFeEgGaAxcsp$\0", *fmt)) { fmt++; }
    
    if (start != fmt) {
    
      strncpy(fmt_buf, start, (fmt - start)+1);
      fmt_buf[(fmt - start)+1] = '\0';
      
      if (var_list_end(vl)) {
        throw(FormatError, "Not enough arguments to Format String!");
      }
      
      var a = var_list_get(vl);
      
      if (*fmt == '$') { pos = show_to(a, out, pos); }
      
      if (*fmt == 's') {      
        int off = format_to(out, pos, fmt_buf, as_str(a));
        if (off < 0) { throw(FormatError, "Unable to output String!"); }
        pos += off;
      }
      
      if (strchr("diouxX", *fmt)) {
        int off = format_to(out, pos, fmt_buf, as_long(a));
        if (off < 0) { throw(FormatError, "Unable to output Int!"); }
        pos += off;
      }
      
      if (strchr("fFeEgGaA", *fmt)) { 
        int off = format_to(out, pos, fmt_buf, as_double(a));
        if (off < 0) { throw(FormatError, "Unable to output Real!"); }
        pos += off;
      }
      
      if (*fmt == 'c') {
        int off = format_to(out, pos, fmt_buf, as_char(a));
        if (off < 0) { throw(FormatError, "Unable to output Char!"); }
        pos += off;
      }
      
      if (*fmt == 'p') {
        int off = format_to(out, pos, fmt_buf, a);
        if (off < 0) { throw(FormatError, "Unable to output Object!"); }
        pos += off;
      }

      fmt++;
      continue;
    }
    
    throw(FormatError, "Invalid Format String!");
  }
  
  return pos;
  
}

int look(var self) {
  return look_from(self, $(File, stdin), 0);
}

int look_from(var self, var input, int pos) {
  return type_class_method(type_of(self), Show, look, self, input, pos);
}

int scan_vl(const char* fmt, var_list vl) {
  return scan_from_vl($(File, stdin), 0, fmt, vl);
}

int scanln_vl(const char* fmt, var_list vl) {
  int ret = 0;
  ret = scan_from_vl($(File, stdin), ret, fmt, vl);
  ret = scan_from($(File, stdin), ret, "\n");
  return ret;
}

int scan_from_vl(var input, int pos, const char* fmt, var_list vl) {

  char fmt_buf[strlen(fmt)+1];
  
  while(true) {
    
    if (*fmt == '\0') { break; }
    
    const char* start = fmt;
    
    /* Match String */
    while(!strchr("%\0", *fmt)) { fmt++; }
    
    if (start != fmt) {
      
      strncpy(fmt_buf, start, (fmt - start));
      fmt_buf[(fmt - start)+0] = '\0';
      fmt_buf[(fmt - start)+1] = '%';
      fmt_buf[(fmt - start)+2] = 'n';
      
      int off = 0;
      int err = format_from(input, pos, fmt_buf, &off);
      if (err < 0) { throw(FormatError, "Unable to input format!"); }
      
      pos += off;
      continue;
    }
    
    /* Match %% */
    if (*fmt == '%' && *(fmt+1) == '%') {
      
      int err = format_from(input, pos, "%%");
      if (err < 0) { throw(FormatError, "Unable to input '%%%%'!"); }
      
      pos += 2;
      fmt += 2;
      continue;
    }
    
    /* Match Format Specifier */
    while(!strchr("diuoxXfFeEgGaAxcsp$\0", *fmt)) { fmt++; }
    
    if (start != fmt) {
    
      strncpy(fmt_buf, start, (fmt - start)+1);
      fmt_buf[(fmt - start)+1] = '\0';
      
      if (var_list_end(vl)) {
        throw(FormatError, "Not enough arguments to Format String!");
      }
      
      var a = var_list_get(vl);
      
      if (*fmt == '$') { pos = look_from(a, input, pos); }
      
      fmt_buf[(fmt - start)+1] = '%';
      fmt_buf[(fmt - start)+2] = 'n';
      fmt_buf[(fmt - start)+3] = '\0';
      
      int off = 0;
      
      if (*fmt == 's') {      
        int err = format_from(input, pos, fmt_buf, as_str(a), &off);
        if (err < 1) { throw(FormatError, "Unable to input String!"); }
        pos += off;
      }
      
      if (strchr("diouxX", *fmt)) {
        long tmp = 0;
        int err = format_from(input, pos, fmt_buf, &tmp, &off);
        if (err < 1) { throw(FormatError, "Unable to input Int!"); }
        pos += off;
        assign(a, $(Int, tmp));
      }
      
      if (strchr("fFeEgGaA", *fmt)) {
        if (strchr(fmt_buf, 'l')) {
          double tmp = 0;
          int err = format_from(input, pos, fmt_buf, &tmp, &off);
          if (err < 1) { throw(FormatError, "Unable to input Real!"); }
          pos += off;
          assign(a, $(Real, tmp));
        } else {
          float tmp = 0;
          int err = format_from(input, pos, fmt_buf, &tmp, &off);
          if (err < 1) { throw(FormatError, "Unable to input Real!"); }
          pos += off;
          assign(a, $(Real, tmp));
        }
      }
      
      if (*fmt == 'c') {
        char tmp = '\0';
        int err = format_from(input, pos, fmt_buf, &tmp, &off);
        if (err < 1) { throw(FormatError, "Unable to input Char!"); }
        pos += off;
        assign(a, $(Char, tmp));
      }
      
      if (*fmt == 'p') {
        void* tmp = NULL;
        int err = format_from(input, pos, fmt_buf, &tmp, &off);
        if (err < 1) { throw(FormatError, "Unable to input Reference!"); }
        pos += off;
        assign(a, $(Reference, tmp));
      }

      fmt++;
      continue;
    }
    
    throw(FormatError, "Invalid Format String!");
  }
  
  return pos;

}

