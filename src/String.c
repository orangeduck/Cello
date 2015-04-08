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

static struct DocMethod* C_Str_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "c_str", 
      "char* c_str(var self);",
      "Returns the object `self` represented as a `char*`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

/* TODO: Examples */

var C_Str = Cello(C_Str,
  Instance(Doc,
    C_Str_Name, C_Str_Brief, C_Str_Description, 
    NULL, C_Str_Methods));

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

static bool String_Eq(var self, var obj) {
  return strcmp(String_C_Str(self), c_str(obj)) is 0;
}

static bool String_Gt(var self, var obj) {
  return strcmp(String_C_Str(self), c_str(obj)) > 0;
}

static bool String_Lt(var self, var obj) {
  return strcmp(String_C_Str(self), c_str(obj)) < 0;
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

static void String_Reverse(var self) {
  struct String* s = self;
  for(size_t i = 0; i < strlen(s->val) / 2; i++) {
    char temp = s->val[i];
    s->val[i] = s->val[strlen(s->val)-1-i];
    s->val[strlen(s->val)-1-i] = temp;
  }
}

static int String_Format_To(var self, int pos, const char* fmt, va_list va) {

  struct String* s = self;
  
#if defined(_WIN32)
  
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
  
#elif defined(__APPLE__)
  
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
  return print_to(out, pos, "\"%s\"", self);
}

static int String_Look(var self, var input, int pos) {
  return scan_from(input, pos, "\"%[^\"]\"", self);
}

var String = Cello(String,
  Instance(Doc,
    String_Name, String_Brief, String_Description,
    NULL, NULL),
  Instance(New,     NULL, String_Del),
  Instance(Assign,  String_Assign),
  Instance(Eq,      String_Eq),
  Instance(Ord,     String_Gt, String_Lt, String_Cmp),
  Instance(Len,     String_Len),
  Instance(Get,     NULL, NULL, String_Mem, String_Rem),
  Instance(Clear,   String_Clear),
  Instance(Reverse, String_Reverse),
  Instance(Hash,    String_Hash),
  Instance(C_Str,   String_C_Str),
  Instance(Format,  String_Format_To, String_Format_From),
  Instance(Show,    String_Show, String_Look));

