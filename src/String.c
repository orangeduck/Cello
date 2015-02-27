#include "Cello.h"

static const char* String_Name(void) {
  return "String";
}

/* TODO */
static const char* String_Brief(void) {
  return "";
}

/* TODO */
static const char* String_Description(void) {
  return "";
}

/* TODO */
static const char* String_Examples(void) {
  return "";
}

/* TODO */
static const char* String_Methods(void) {
  return "";
}

static var String_New(var self, var args) {
  struct String* s = self;
  char* init = c_str(get(args, $(Int, 0)));
  s->val = malloc(strlen(init) + 1);
#if CELLO_MEMORY_CHECK == 1
  if (s->val is None) {
    throw(OutOfMemoryError, "Cannot allocate String, out of memory!");
  }
#endif
  strcpy(s->val, init);
  return self;
}

static var String_Del(var self) {
  struct String* s = self;
  free(s->val);
  return self;
}

static size_t String_Size(void) {
  return sizeof(struct String);
}

static var String_Assign(var self, var obj) {
  struct String* s = self;
  char* val = c_str(obj);
  
#if CELLO_ALLOC_CHECK == 1
  if (CelloHeader_GetFlag(Cello_GetHeader(self), CelloStackAlloc)
  or  CelloHeader_GetFlag(Cello_GetHeader(self), CelloStaticAlloc)) {
    throw(ValueError, "Cannot reallocate String, not on heap!");
  }
#endif
  
  s->val = realloc(s->val, strlen(val) + 1);
  
#if CELLO_MEMORY_CHECK == 1
  if (s->val is None) {
    throw(OutOfMemoryError, "Cannot allocate String, out of memory!");
  }
#endif
  strcpy(s->val, val);
  return self;
}

static var String_Copy(var self) {
  return new(String, self);
}

static char* String_C_Str(var self) {
  struct String* s = self;
  return s->val;
}

static var String_Eq(var self, var obj) {
  return bool_var(strcmp(String_C_Str(self), c_str(obj)) == 0);
}

static var String_Gt(var self, var obj) {
  return bool_var(strcmp(String_C_Str(self), c_str(obj)) > 0);
}

static var String_Lt(var self, var obj) {
  return bool_var(strcmp(String_C_Str(self), c_str(obj)) < 0);
}

static size_t String_Len(var self) {
  struct String* s = self;
  return strlen(s->val);
}

static void String_Clear(var self) {
  struct String* s = self;
  
#if CELLO_ALLOC_CHECK == 1
  if (CelloHeader_GetFlag(Cello_GetHeader(self), CelloStackAlloc)
  or  CelloHeader_GetFlag(Cello_GetHeader(self), CelloStaticAlloc)) {
    throw(ValueError, "Cannot reallocate String, not on heap!");
  }
#endif
  
  s->val = realloc(s->val, 1);
  
#if CELLO_MEMORY_CHECK == 1
  if (s->val is None) {
    throw(OutOfMemoryError, "Cannot allocate String, out of memory!");
  }
#endif

  s->val[0] = '\0';
}

static var String_Mem(var self, var obj) {
  
  if (implements(obj, C_Str)) {
    return bool_var(strstr(c_str(self), c_str(obj)) isnt None);
  }
  
  return False;
}

static void String_Rem(var self, var obj) {
  
  if (implements(obj, C_Str)) {
    char* pos = strstr(c_str(self), c_str(obj));
    size_t count = strlen(c_str(self)) - strlen(pos) - strlen(c_str(obj)) + 1;
    memmove((char*)pos, pos + strlen(c_str(obj)), count);
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
  if (CelloHeader_GetFlag(Cello_GetHeader(self), CelloStackAlloc)
  or  CelloHeader_GetFlag(Cello_GetHeader(self), CelloStaticAlloc)) {
    throw(ValueError, "Cannot reallocate String, not on heap!");
  }
#endif
  
  s->val = realloc(s->val, pos + size + 1);

#if CELLO_MEMORY_CHECK == 1
  if (s->val is None) {
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
  if (CelloHeader_GetFlag(Cello_GetHeader(self), CelloStackAlloc)
  or  CelloHeader_GetFlag(Cello_GetHeader(self), CelloStaticAlloc)) {
    throw(ValueError, "Cannot reallocate String, not on heap!");
  }
#endif
  
  s->val = realloc(s->val, pos + size + 1);
  
#if CELLO_MEMORY_CHECK == 1
  if (s->val is None) {
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
  if (CelloHeader_GetFlag(Cello_GetHeader(self), CelloStackAlloc)
  or  CelloHeader_GetFlag(Cello_GetHeader(self), CelloStaticAlloc)) {
    throw(ValueError, "Cannot reallocate String, not on heap!");
  }
#endif
  
  s->val = realloc(s->val, pos + size + 1);
  
#if CELLO_MEMORY_CHECK == 1
  if (s->val is None) {
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

static var String_Gen(void) {
  size_t l = gen_c_int() % 10;
  
  char characters[62] = 
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "0123456789";
  
  char* data = malloc(l+1);
  for (size_t i = 0; i < l; i++) {
    data[i] = characters[gen_c_int() % 62];
  }
  data[l] = '\0';
  var x = new(String, $S(data));
  free(data);
  return x;
}

var String = Cello(String,
  Member(Doc,
    String_Name, String_Brief, String_Description,
    String_Examples, String_Methods),
  Member(Size,    String_Size),
  Member(New,     String_New, String_Del),
  Member(Assign,  String_Assign),
  Member(Copy,    String_Copy),
  Member(Eq,      String_Eq),
  Member(Ord,     String_Gt, String_Lt),
  Member(Len,     String_Len),
  Member(Get,     NULL, NULL, String_Mem, String_Rem),
  Member(Clear,   String_Clear),
  Member(Reverse, String_Reverse),
  Member(Hash,    String_Hash),
  Member(C_Str,   String_C_Str),
  Member(Format,  String_Format_To, String_Format_From),
  Member(Show,    String_Show, String_Look),
  Member(Gen,     String_Gen));

