#include "Cello.h"

static const char* Format_Name(void) {
  return "Format";
}

static const char* Format_Brief(void) {
  return "Read or Write with Format String";
}

static const char* Format_Description(void) {
  return
    "Format abstracts the class of operations such as `scanf`, `sprintf` and "
    "`fprintf` with matching semantics. It provides general `printf` and "
    "`scanf` functionality for several different types objects in a "
    "uniform way. This class is essentially an in-between class, used by the "
    "`Show` class to read and write output."
    "\n\n"
    "It is important to note that the semantics of these operations match "
    "`printf` and not the newly defined `Show` class. For example it is "
    "perfectly valid to pass a C `int` to these functions, while the `println` "
    "function from `Show` must be passed only `var` objects.";
}

static const char* Format_Definition(void) {
  return 
    "struct Format {\n"
    "  int (*format_to)(var,int,const char*,va_list);\n"
    "  int (*format_from)(var,int,const char*,va_list);\n"
    "};\n";
}

static struct Example* Format_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "/* printf(\"Hello my name is %s, I'm %i\\n\", \"Dan\", 23); */\n"
      "format_to($(File, stdout), 0, \n"
      "  \"Hello my name is %s, I'm %i\\n\", \"Dan\", 23);\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* Format_Methods(void) {
  
  static struct Method methods[] = {
    {
      "format_to", 
      "int format_to(var self, int pos, const char* fmt, ...);\n"
      "int format_to_va(var self, int pos, const char* fmt, va_list va);",
      "Write a formatted string `fmt` to the object `self` at position `pos`."
    }, {
      "format_from", 
      "int format_from(var self, int pos, const char* fmt, ...);\n"
      "int format_from_va(var self, int pos, const char* fmt, va_list va);",
      "Read a formatted string `fmt` from the object `self` at position `pos`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Format = Cello(Format,
  Instance(Doc, 
    Format_Name,       Format_Brief,    Format_Description,
    Format_Definition, Format_Examples, Format_Methods));

int format_to_va(var self, int pos, const char* fmt, va_list va) {
  return method(self, Format, format_to, pos, fmt, va);
}

int format_from_va(var self, int pos, const char* fmt, va_list va) {
  return method(self, Format, format_from, pos, fmt, va);
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

static const char* Show_Name(void) {
  return "Show";
}

static const char* Show_Brief(void) {
  return "Convert To or From String";
}

static const char* Show_Description(void) {
  return
    "The `Show` class is used to convert objects to, and from, a `String` "
    "representation. Objects which implement `Show` should expect the "
    "input/output object to be one that support the `Format` class, such as "
    "`File` or `String`."
    "\n\n"
    "The `print`, `println` and `print_to` functions provide a mechanism for "
    "writing formatted strings with Cello objects. To do this they provide a "
    "new format specifier `%$` which uses an object's `Show` functionality to "
    "write that part of the string. All objects which don't support `Show` can "
    "still be shown via a default implementation."
    "\n\n"
    "All the Show methods which are variable arguments only take `var` objects "
    "as input. To print native C types wrap them in Cello types using `$`."
    "\n\n"
    "Standard format specifiers such as `%f` and `%d` will call functions such "
    "as `c_float` and `c_int` on their passed arguments to convert objects to "
    "C types before performing the standard C formatting behaviour."
    "\n\n"
    "See [printf](http://www.cplusplus.com/reference/cstdio/printf/) for more "
    "information on format specifiers.";
}

static const char* Show_Definition(void) {
  return
    "struct Show {\n"
    "  int (*show)(var, var, int);\n"
    "  int (*look)(var, var, int);\n"
    "};\n";
}

static struct Example* Show_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Hello World",
      "println(\"Hello %s!\", $S(\"World\"));\n"
    }, {
      "File Writing",
      "with (f in new(File, $S(\"prices.txt\"), $S(\"wb\"))) {\n"
      "  print_to(f, 0, \"%$ :: %$\\n\", $S(\"Banana\"), $I(57));\n"
      "  print_to(f, 0, \"%$ :: %$\\n\", $S(\"Apple\"),  $I(22));\n"
      "  print_to(f, 0, \"%$ :: %$\\n\", $S(\"Pear\"),   $I(16));\n"
      "}\n"
    }, {
      "String Scanning",
      "var input = $S(\"1 and 52 then 78\");\n"
      "\n"
      "var i0 = $I(0), i1 = $I(0), i2 = $I(0);\n"
      "scan_from(input, 0, \"%i and %i then %i\", i0, i1, i2);\n"
      "\n"
      "/* i0: 1, i1: 52, i2: 78 */\n"
      "println(\"i0: %$, i1: %$, i2: %$\", i0, i1, i2);\n"
    }, {
      "String Printing",
      "var greeting = new(String);\n"
      "print_to(greeting, 0, \"Hello %s %s, %s?\", \n"
      "  $S(\"Mr\"), $S(\"Johnson\"), $S(\"how are you?\"));\n"
      "\n"
      "/* Hello Mr Johnson, how are you? */\n"
      "show(greeting);\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* Show_Methods(void) {
  
  static struct Method methods[] = {
    {
      "show", 
      "int show(var self);\n"
      "int show_to(var self, var out, int pos);",
      "Show the object `self` either to `stdout` or to the object `output`."
    }, {
      "look", 
      "int look(var self);\n"
      "int look_from(var self, var input, int pos);",
      "Read the object `self` either from `stdout` or from the object `input`."
    }, {
      "print", 
      "#define print(fmt, ...)\n"
      "#define println(fmt, ...)\n"
      "#define print_to(out, pos, fmt, ...)\n"
      "int print_with(const char* fmt, var args);\n"
      "int println_with(const char* fmt, var args);\n"
      "int print_to_with(var out, int pos, const char* fmt, var args);",
      "Print the format string `fmt` either to `stdout` or to the object `out` "
      "at positions `pos`. Returns new position in output."
    }, {
      "scan", 
      "#define scan(fmt, ...)\n"
      "#define scanln(fmt, ...)\n"
      "#define scan_from(input, pos, fmt, ...)\n"
      "int scan_with(const char* fmt, var args);\n"
      "int scanln_with(const char* fmt, var args);\n"
      "int scan_from_with(var input, int pos, const char* fmt, var args);",
      "Scan the format string `fmt` either from `stdout` or from the object "
      "`input` at position `pos`. Returns new position in output."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Show = Cello(Show,
  Instance(Doc, 
    Show_Name,       Show_Brief,    Show_Description,
    Show_Definition, Show_Examples, Show_Methods));

int show(var self) {
  return show_to(self, $(File, stdout), 0);
}

int show_to(var self, var out, int pos) {
  
  struct Show* s = instance(self, Show);
  if (s and s->show) {
    return s->show(self, out, pos);
  }
  
  return print_to(out, pos, "<'%s' At 0x%p>", type_of(self), self);
  
}

int print_with(const char* fmt, var args) {
  return print_to_with($(File, stdout), 0, fmt, args);
}

int println_with(const char* fmt, var args) {
  int pos = 0;
  pos = print_to_with($(File, stdout), pos, fmt, args);
  pos = print_to($(File, stdout), pos, "\n");
  return pos;
}

int print_to_with(var out, int pos, const char* fmt, var args) {
  
  char* fmt_buf = malloc(strlen(fmt)+1); 
  size_t index = 0;
  
  while (true) {
    
    if (*fmt is '\0') { break; }
    
    const char* start = fmt;
    
    /* Match String */
    while(*fmt isnt '\0' and *fmt isnt '%') { fmt++; }
    
    if (start isnt fmt) {
      memcpy(fmt_buf, start, fmt - start);
      fmt_buf[fmt - start] = '\0';
      int off = format_to(out, pos, fmt_buf);
      if (off < 0) { throw(FormatError, "Unable to output format!"); }
      pos += off;
      continue;
    }
    
    /* Match %% */
    if (*fmt is '%' && *(fmt+1) is '%') {
      int off = format_to(out, pos, "%%");
      if (off < 0) { throw(FormatError, "Unable to output '%%%%'!"); }
      pos += off;
      fmt += 2;
      continue;
    }
    
    /* Match Format Specifier */
    while(not strchr("diuoxXfFeEgGaAxcsp$", *fmt)) { fmt++; }
    
    if (start isnt fmt) {
    
      memcpy(fmt_buf, start, fmt - start + 1);
      fmt_buf[fmt - start + 1] = '\0';
      
      if (index >= len(args)) {
        throw(FormatError, "Not enough arguments to Format String!");
      }
      
      var a = get(args, $I(index)); index++;
      
      if (*fmt is '$') { pos = show_to(a, out, pos); }
      
      if (*fmt is 's') {      
        int off = format_to(out, pos, fmt_buf, c_str(a));
        if (off < 0) { throw(FormatError, "Unable to output String!"); }
        pos += off;
      }
      
      if (strchr("diouxX", *fmt)) {
        int off = format_to(out, pos, fmt_buf, c_int(a));
        if (off < 0) { throw(FormatError, "Unable to output Int!"); }
        pos += off;
      }
      
      if (strchr("fFeEgGaA", *fmt)) { 
        int off = format_to(out, pos, fmt_buf, c_float(a));
        if (off < 0) { throw(FormatError, "Unable to output Real!"); }
        pos += off;
      }
      
      if (*fmt is 'c') {
        int off = format_to(out, pos, fmt_buf, c_int(a));
        if (off < 0) { throw(FormatError, "Unable to output Char!"); }
        pos += off;
      }
      
      if (*fmt is 'p') {
        int off = format_to(out, pos, fmt_buf, a);
        if (off < 0) { throw(FormatError, "Unable to output Object!"); }
        pos += off;
      }

      fmt++;
      continue;
    }
    
    throw(FormatError, "Invalid Format String!");
  }

  free(fmt_buf);
  
  return pos;
  
}

int look(var self) {
  return look_from(self, $(File, stdin), 0);
}

int look_from(var self, var input, int pos) {
  return method(self, Show, look, input, pos);
}

int scan_with(const char* fmt, var args) {
  return scan_from_with($(File, stdin), 0, fmt, args);
}

int scanln_with(const char* fmt, var args) {
  int pos = 0;
  pos = scan_from_with($(File, stdin), pos, fmt, args);
  pos = scan_from($(File, stdin), pos, "\n");
  return pos;
}

int scan_from_with(var input, int pos, const char* fmt, var args) {
  
  char* fmt_buf = malloc(strlen(fmt)+4);
  size_t index = 0;
  
  while (true) {
    
    if (*fmt is '\0') { break; }
    
    const char* start = fmt;
    
    /* Match String */
    while (*fmt isnt '\0' and *fmt isnt '%') { fmt++; }
    
    if (start isnt fmt) {  
      memcpy(fmt_buf, start, fmt - start);
      fmt_buf[fmt - start] = '\0';
      format_from(input, pos, fmt_buf);
      pos += (int)(fmt - start);
      continue;
    }
    
    /* Match %% */
    if (*fmt is '%' and *(fmt+1) is '%') {
      int err = format_from(input, pos, "%%");
      if (err < 0) { throw(FormatError, "Unable to input '%%%%'!"); }
      pos += 2;
      fmt += 2;
      continue;
    }
    
    /* Match Format Specifier */
    while (not strchr("diuoxXfFeEgGaAxcsp$[^]", *fmt)) { fmt++; }
    
    if (start isnt fmt) {
      
      int off = 0;
      memcpy(fmt_buf, start, fmt - start + 1);
      fmt_buf[fmt - start + 1] = '\0';
      strcat(fmt_buf, "%n");

      if (index >= len(args)) {
        throw(FormatError, "Not enough arguments to Format String!");
      }
      
      var a = get(args, $I(index)); index++;
      
      if (*fmt is '$') { pos = look_from(a, input, pos); }
      
      else if (*fmt is 's') {
        int err = format_from(input, pos, fmt_buf, c_str(a), &off);
        if (err < 1) { throw(FormatError, "Unable to input String!"); }
        pos += off;
      }
      
      /* TODO: Test */
      else if (*fmt is ']') {
        int err = format_from(input, pos, fmt_buf, c_str(a), &off);
        if (err < 1) { throw(FormatError, "Unable to input Scanset!"); }
        pos += off;
      }
      
      else if (strchr("diouxX", *fmt)) {
        long tmp = 0;
        int err = format_from(input, pos, fmt_buf, &tmp, &off);
        if (err < 1) { throw(FormatError, "Unable to input Int!"); }
        pos += off;
        assign(a, $I(tmp));
      }
      
      else if (strchr("fFeEgGaA", *fmt)) {
        if (strchr(fmt_buf, 'l')) {
          double tmp = 0;
          int err = format_from(input, pos, fmt_buf, &tmp, &off);
          if (err < 1) { throw(FormatError, "Unable to input Float!"); }
          pos += off;
          assign(a, $F(tmp));
        } else {
          float tmp = 0;
          int err = format_from(input, pos, fmt_buf, &tmp, &off);
          if (err < 1) { throw(FormatError, "Unable to input Float!"); }
          pos += off;
          assign(a, $F(tmp));
        }
      }
      
      else if (*fmt is 'c') {
        char tmp = '\0';
        int err = format_from(input, pos, fmt_buf, &tmp, &off);
        if (err < 1) { throw(FormatError, "Unable to input Char!"); }
        pos += off;
        assign(a, $I(tmp));
      }
      
      else if (*fmt is 'p') {
        void* tmp = NULL;
        int err = format_from(input, pos, fmt_buf, &tmp, &off);
        if (err < 1) { throw(FormatError, "Unable to input Ref!"); }
        pos += off;
        assign(a, $R(tmp));
      }
      
      else {
        /* TODO: Report Better */
        throw(FormatError, "Invalid Format Specifier!");
      }

      fmt++;
      continue;
    }
  }

  free(fmt_buf);
  
  return pos;

}

