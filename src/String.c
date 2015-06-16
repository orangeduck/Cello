#include "Cello.h"

static const char* C_Str_Name(void) {
  return "C_Str";
}

static const char* C_Str_Brief(void) {
  return "Interpret as C String";
}

static const char* C_Str_Description(void) {
  return
    "The `C_Str` class should be overridden by types which are representable "
    "as a C style String.";
}

static const char* C_Str_Definition(void) {
  return
    "struct C_Str {\n"
    "  char* (*c_str)(var);\n"
    "};\n";
}

static struct Example* C_Str_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "puts(c_str($S(\"Hello\"))); /* Hello */\n"
      "puts(c_str($S(\"There\"))); /* There */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* C_Str_Methods(void) {
  
  static struct Method methods[] = {
    {
      "c_str", 
      "char* c_str(var self);",
      "Returns the object `self` represented as a `char*`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var C_Str = Cello(C_Str,
  Instance(Doc,
    C_Str_Name,       C_Str_Brief,    C_Str_Description, 
    C_Str_Definition, C_Str_Examples, C_Str_Methods));

char* c_str(var self) {
  
  if (type_of(self) is String) {
    return ((struct String*)self)->val;
  }
  
  return method(self, C_Str, c_str);
}
    
static const char* String_Name(void) {
  return "String";
}

static const char* String_Brief(void) {
  return "String Object";
}

static const char* String_Description(void) {
  return
    "The `String` type is a wrapper around the native C string type. This "
    "includes strings that are allocated on either the Stack or the Heap."
    "\n\n"
    "For strings allocated on the heap a number of extra operations are "
    "provided overs standard C strings such as concatenation.";
}

static const char* String_Definition(void) {
  return
    "struct String {\n"
    "  char* val;\n"
    "};\n";
}

static struct Example* String_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var s0 = $(String, \"Hello\");\n"
      "var s1 = new(String, $S(\"Hello\"));\n"
      "append(s1, $S(\" There\"));\n"
      "show(s0); /* Hello */\n"
      "show(s1); /* Hello There */\n"
    }, {
      "Manipulation",
      "var s0 = new(String, $S(\"Balloons\"));\n"
      "\n"
      "show($I(len(s0))); /* 8 */\n"
      "show($I(mem(s0, $S(\"Ball\"))));     /* 1 */\n"
      "show($I(mem(s0, $S(\"oon\"))));      /* 1 */\n"
      "show($I(mem(s0, $S(\"Balloons\")))); /* 1 */\n"
      "show($I(mem(s0, $S(\"l\"))));        /* 1 */\n"
      "\n"
      "rem(s0, $S(\"oons\"));\n"
      "\n"
      "show($I(eq(s0, $S(\"Ball\")))); /* 1 */\n"
      "\n"
      "resize(s0, 0);\n"
      "\n"
      "show($I(len(s0))); /* 0 */\n"
      "show($I(eq(s0, $S(\"\")))); /* 1 */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static void String_Assign(var self, var obj);

static void String_New(var self, var args) {
  struct String* s = self;
  if (len(args) > 0) {
    String_Assign(self, get(args, $I(0)));
  } else {
    s->val = calloc(1, 1);
  }
  
#if CELLO_MEMORY_CHECK == 1
  if (s->val is NULL) {
    throw(OutOfMemoryError, "Cannot allocate String, out of memory!");
  }
#endif
}

static void String_Del(var self) {
  struct String* s = self;

#if CELLO_ALLOC_CHECK == 1
  if (header(self)->alloc is (var)AllocStack
  or  header(self)->alloc is (var)AllocStatic) {
    throw(ValueError, "Cannot destruct String, not on heap!");
  }
#endif

  free(s->val);
}

static void String_Assign(var self, var obj) {
  struct String* s = self;
  char* val = c_str(obj);
  
#if CELLO_ALLOC_CHECK == 1
  if (header(self)->alloc is (var)AllocStack
  or  header(self)->alloc is (var)AllocStatic) {
    throw(ValueError, "Cannot reallocate String, not on heap!");
  }
#endif
  
  s->val = realloc(s->val, strlen(val) + 1);
  
#if CELLO_MEMORY_CHECK == 1
  if (s->val is NULL) {
    throw(OutOfMemoryError, "Cannot allocate String, out of memory!");
  }
#endif

  strcpy(s->val, val);
}

static char* String_C_Str(var self) {
  struct String* s = self;
  return s->val;
}

static int String_Cmp(var self, var obj) {
  return strcmp(String_C_Str(self), c_str(obj));
}

static size_t String_Len(var self) {
  struct String* s = self;
  return strlen(s->val);
}

static void String_Clear(var self) {
  struct String* s = self;
  
#if CELLO_ALLOC_CHECK == 1
  if (header(self)->alloc is (var)AllocStack
  or  header(self)->alloc is (var)AllocStatic) {
    throw(ValueError, "Cannot reallocate String, not on heap!");
  }
#endif
  
  s->val = realloc(s->val, 1);
  
#if CELLO_MEMORY_CHECK == 1
  if (s->val is NULL) {
    throw(OutOfMemoryError, "Cannot allocate String, out of memory!");
  }
#endif

  s->val[0] = '\0';
}

static bool String_Mem(var self, var obj) {
  
  struct C_Str* c = instance(obj, C_Str);
  if (c and c->c_str) {
    return strstr(String_C_Str(self), c->c_str(obj));
  }
  
  return false;
}

static void String_Rem(var self, var obj) {
  
  struct C_Str* c = instance(obj, C_Str);
  if (c and c->c_str) {
    char* pos = strstr(String_C_Str(self), c->c_str(obj));
    size_t count = strlen(String_C_Str(self)) - strlen(pos) - 
      strlen(c->c_str(obj)) + 1;
    memmove((char*)pos, pos + strlen(c->c_str(obj)), count);
  }
  
}

static uint64_t String_Hash(var self) {
  struct String* s = self;
  return hash_data(s->val, strlen(s->val));
}

static void String_Concat(var self, var obj) {
  struct String* s = self;
  
#if CELLO_ALLOC_CHECK == 1
  if (header(self)->alloc is (var)AllocStack
  or  header(self)->alloc is (var)AllocStatic) {
    throw(ValueError, "Cannot reallocate String, not on heap!");
  }
#endif
  
  s->val = realloc(s->val, strlen(s->val) + strlen(c_str(obj)) + 1);
  
#if CELLO_MEMORY_CHECK == 1
  if (s->val is NULL) {
    throw(OutOfMemoryError, "Cannot allocate String, out of memory!");
  }
#endif
  
  strcat(s->val, c_str(obj));
}

static void String_Resize(var self, size_t n) {
  struct String* s = self;
  
#if CELLO_ALLOC_CHECK == 1
  if (header(self)->alloc is (var)AllocStack
  or  header(self)->alloc is (var)AllocStatic) {
    throw(ValueError, "Cannot reallocate String, not on heap!");
  }
#endif
  
  size_t m = String_Len(self);
  s->val = realloc(s->val, n+1);
  
  if (n > m) {
    memset(&s->val[m], 0, n - m);
  } else {
    s->val[n] = '\0';
  }
  
#if CELLO_MEMORY_CHECK == 1
  if (s->val is NULL) {
    throw(OutOfMemoryError, "Cannot allocate String, out of memory!");
  }
#endif
  
}

static int String_Format_To(var self, int pos, const char* fmt, va_list va) {

  struct String* s = self;
  
#ifdef CELLO_WINDOWS
  
  va_list va_tmp;
  va_copy(va_tmp, va);
  int size = _vscprintf(fmt, va_tmp);
  va_end(va_tmp);
  
#if CELLO_ALLOC_CHECK == 1
  if (header(self)->alloc is (var)AllocStack
  or  header(self)->alloc is (var)AllocStatic) {
    throw(ValueError, "Cannot reallocate String, not on heap!");
  }
#endif
  
  s->val = realloc(s->val, pos + size + 1);

#if CELLO_MEMORY_CHECK == 1
  if (s->val is NULL) {
    throw(OutOfMemoryError, "Cannot allocate String, out of memory!");  
  }
#endif
  
  return vsprintf(s->val + pos, fmt, va); 
  
#elif defined(CELLO_MAC)
  
  va_list va_tmp;
  va_copy(va_tmp, va);
  char* tmp = NULL;
  int size = vasprintf(&tmp, fmt, va_tmp);
  va_end(va_tmp);
  
#if CELLO_ALLOC_CHECK == 1
  if (header(self)->alloc is (var)AllocStack
  or  header(self)->alloc is (var)AllocStatic) {
    throw(ValueError, "Cannot reallocate String, not on heap!");
  }
#endif
  
  s->val = realloc(s->val, pos + size + 1);
  
#if CELLO_MEMORY_CHECK == 1
  if (s->val is NULL) {
    throw(OutOfMemoryError, "Cannot allocate String, out of memory!");
  }
#endif
  
  s->val[pos] = '\0';
  strcat(s->val, tmp);
  free(tmp);
  
  return size;
  
#else

  va_list va_tmp;
  va_copy(va_tmp, va);
  int size = vsnprintf(NULL, 0, fmt, va_tmp);
  va_end(va_tmp);
  
#if CELLO_ALLOC_CHECK == 1
  if (header(self)->alloc is (var)AllocStack
  or  header(self)->alloc is (var)AllocStatic) {
    throw(ValueError, "Cannot reallocate String, not on heap!");
  }
#endif
  
  s->val = realloc(s->val, pos + size + 1);
  
#if CELLO_MEMORY_CHECK == 1
  if (s->val is NULL) {
    throw(OutOfMemoryError, "Cannot allocate String, out of memory!");
  }
#endif
  
  return vsprintf(s->val + pos, fmt, va); 
  
#endif

}

static int String_Format_From(var self, int pos, const char* fmt, va_list va) {
  struct String* s = self;
  return vsscanf(s->val + pos, fmt, va);
}

static int String_Show(var self, var out, int pos) {
  struct String* s = self;
  pos = print_to(out, pos, "\"", self);
  char* v = s->val;
  while (*v) {
    switch (*v) {
      case '\a': pos = print_to(out, pos, "\\a"); break;
      case '\b': pos = print_to(out, pos, "\\b"); break;
      case '\f': pos = print_to(out, pos, "\\f"); break;
      case '\n': pos = print_to(out, pos, "\\n"); break;
      case '\r': pos = print_to(out, pos, "\\r"); break;
      case '\t': pos = print_to(out, pos, "\\t"); break;
      case '\v': pos = print_to(out, pos, "\\v"); break;
      case '\\': pos = print_to(out, pos, "\\\\"); break;
      case '\'': pos = print_to(out, pos, "\\'"); break;
      case '\"': pos = print_to(out, pos, "\\\""); break;
      case '\?': pos = print_to(out, pos, "\\?"); break;
      default:   pos = print_to(out, pos, "%c", $I(*v));
    }
    v++;
  }
  return print_to(out, pos, "\"", self);
}

static int String_Look(var self, var input, int pos) {
  
  String_Clear(self);
  
  var chr = $I(0);
  pos = scan_from(input, pos, "%c", chr);
  
  if (c_int(chr) isnt '\"') {
    throw(FormatError, 
      "String literal does not start with quotation marks!");
  }
  
  while (true) {
    
    pos = scan_from(input, pos, "%c", chr);
    
    if (c_int(chr) == '"') { break; }
    
    if (c_int(chr) == '\\') {
      pos = scan_from(input, pos, "%c", chr);
      switch (c_int(chr)) {
        case 'a':  String_Concat(self, $S("\a")); break;
        case 'b':  String_Concat(self, $S("\b")); break;
        case 'f':  String_Concat(self, $S("\f")); break;
        case 'n':  String_Concat(self, $S("\n")); break;
        case 'r':  String_Concat(self, $S("\r")); break;
        case 't':  String_Concat(self, $S("\t")); break;
        case 'v':  String_Concat(self, $S("\v")); break;
        case '\\': String_Concat(self, $S("\\")); break;
        case '\'': String_Concat(self, $S("\'")); break;
        case '"':  String_Concat(self, $S("\"")); break;
        case '?':  String_Concat(self, $S("\?")); break;
        default: throw(FormatError, "Unknown Escape Sequence '\\%c'!", chr);
      }
    }
    
    char buffer[2];
    buffer[0] = (char)c_int(chr);
    buffer[1] = '\0';
    
    String_Concat(self, $S(buffer));
  }
  
  return pos;
}

var String = Cello(String,
  Instance(Doc,
    String_Name,       String_Brief,    String_Description,
    String_Definition, String_Examples, NULL),
  Instance(New,     String_New, String_Del),
  Instance(Assign,  String_Assign),
  Instance(Cmp,     String_Cmp),
  Instance(Hash,    String_Hash),
  Instance(Len,     String_Len),
  Instance(Get,     NULL, NULL, String_Mem, String_Rem),
  Instance(Resize,  String_Resize),
  Instance(Concat,  String_Concat, String_Concat),
  Instance(C_Str,   String_C_Str),
  Instance(Format,  String_Format_To, String_Format_From),
  Instance(Show,    String_Show, String_Look));

