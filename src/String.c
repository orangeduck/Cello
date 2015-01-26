#include "Cello.h"

const char* Char_Name(void) {
  return "Char";
}

/* TODO */
const char* Char_Brief(void) {
  return "";
}

/* TODO */
const char* Char_Description(void) {
  return "";
}

/* TODO */
const char* Char_Examples(void) {
  return "";
}

/* TODO */
const char* Char_Methods(void) {
  return "";
}

static var Char_New(var self, var args) {
  struct Char* c = self;
  c->val = c_char(get(args, $(Int, 0)));
  return self;
}

static var Char_Del(var self) {
  return self;
}

static size_t Char_Size(void) {
  return sizeof(struct Char);
}

static var Char_Assign(var self, var obj) {
  struct Char* c = self;
  c->val = c_char(obj);
  return self;
}

static var Char_Copy(var self) {
  return new(Char, self);
}

static var Char_Eq(var self, var obj) {
  return bool_var(c_char(self) == c_char(obj));
}

static var Char_Gt(var self, var obj) {
  return bool_var(c_char(self) > c_char(obj));
}

static var Char_Lt(var self, var obj) {
  return bool_var(c_char(self) < c_char(obj));
}

static uint64_t Char_Hash(var self) {
  return (uint64_t)c_char(self);
}

static char Char_C_Char(var self) {
  struct Char* c = self;
  return c->val;
}

static int Char_Show(var self, var output, int pos) {
  return print_to(output, pos, "%c", self);
}

static int Char_Look(var self, var input, int pos) {
  return scan_from(input, pos, "%c", self);
}

var Char = typedecl(Char,
  typeclass(Doc,
    Char_Name, Char_Brief, Char_Description, Char_Examples, Char_Methods),
  typeclass(New, Char_New, Char_Del, Char_Size),
  typeclass(Assign, Char_Assign),
  typeclass(Copy, Char_Copy),
  typeclass(Eq, Char_Eq),
  typeclass(Ord, Char_Gt, Char_Lt),
  typeclass(Hash, Char_Hash),
  typeclass(C_Char, Char_C_Char),
  typeclass(Show, Char_Show, Char_Look));

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
  if (s->val == NULL) {
    throw(OutOfMemoryError, "Cannot allocate string, out of memory!");
  }
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
  s->val = realloc(s->val, strlen(val) + 1);
  if (s->val == NULL) {
    throw(OutOfMemoryError, "Cannot allocate string, out of memory!");
  }
  strcpy(s->val, val);
  return self;
}

static var String_Copy(var self) {
  return new(String, self);
}

static var String_Eq(var self, var obj) {
  if (implements(obj, C_Str)) {
    return bool_var(strcmp(c_str(self), c_str(obj)) == 0);
  } else {
    return False;
  }
}

static var String_Gt(var self, var obj) {
  if (not implements(obj, C_Str)) return false;
  
  const char* fst = c_str(self);
  const char* snd = c_str(obj);
  
  int fstlen = strlen(fst);
  int sndlen = strlen(snd);
  int minlen = fstlen > sndlen ? sndlen : fstlen; 
  
  for(int i = 0; i < minlen; i++) {
    if (fst[i] > snd[i]) { return True; }
  }
  
  if (fstlen > sndlen) { return True; }
  
  return False;
}

static var String_Lt(var self, var obj) {
  if (not implements(obj, C_Str)) return false;
  
  const char* fst = c_str(self);
  const char* snd = c_str(obj);
  
  int fstlen = strlen(fst);
  int sndlen = strlen(snd);
  int minlen = fstlen > sndlen ? sndlen : fstlen; 
  
  for(int i = 0; i < minlen; i++) {
    if (fst[i] < snd[i]) { return True; }
  }
  
  if (fstlen < sndlen) { return True; }
  
  return False;
}


static size_t String_Len(var self) {
  struct String* s = self;
  return strlen(s->val);
}

static void String_Clear(var self) {
  struct String* s = self;
  s->val = realloc(s->val, 1);
  if (s->val == NULL) {
    throw(OutOfMemoryError, "Cannot allocate string, out of memory!");
  }
  s->val[0] = '\0';
}

static var String_Mem(var self, var obj) {
  
  if (implements(obj, C_Str)) {
    return bool_var(strstr(c_str(self), c_str(obj)) isnt None);
  }
  
  if (implements(obj, C_Char)) {
    return bool_var(strchr(c_str(self), c_char(obj)) isnt None);
  }
  
  return False;
}

static void String_Rem(var self, var obj) {
  
  if (implements(obj, C_Str)) {
    char* pos = strstr(c_str(self), c_str(obj));
    int count = strlen(c_str(self)) - strlen(pos) - strlen(c_str(obj)) + 1;
    memmove((char*)pos, pos + strlen(c_str(obj)), count);
  }
  
  if (implements(type_of(obj), C_Char)) {
    char* pos = strchr(c_str(self), c_char(obj));
    while (pos isnt NULL) { *pos = (*pos+1); pos++; }
  }
  
}

static uint64_t String_MurmurHash64(
  const void* key, size_t len, uint64_t seed) {
  
  const uint64_t m = 0xc6a4a7935bd1e995;
	const int r = 47;
	const uint64_t * data = (const uint64_t *)key;
	const uint64_t * end = data + (len/8);
  
	uint64_t h = seed ^ (len * m);

	while (data != end) {
		uint64_t k = *data++;
		k *= m; 
		k ^= k >> r; 
		k *= m; 
		h ^= k;
		h *= m; 
	}

	const unsigned char * data2 = (const unsigned char*)data;

	switch (len & 7) {
    case 7: h ^= (uint64_t)(data2[6]) << 48;
    case 6: h ^= (uint64_t)(data2[5]) << 40;
    case 5: h ^= (uint64_t)(data2[4]) << 32;
    case 4: h ^= (uint64_t)(data2[3]) << 24;
    case 3: h ^= (uint64_t)(data2[2]) << 16;
    case 2: h ^= (uint64_t)(data2[1]) << 8;
    case 1: h ^= (uint64_t)(data2[0]);
            h *= m;
	};
  
	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
} 

static uint64_t String_Hash(var self) {
  struct String* s = self;
  return String_MurmurHash64(s->val, strlen(s->val), hash_seed());
}

static char* String_C_Str(var self) {
  struct String* s = self;
  return s->val;
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
  
  s->val = realloc(s->val, pos + size + 1);
  if (s->val == NULL) {
    throw(OutOfMemoryError, "Cannot allocate string, out of memory!");  
  }
  
  return vsprintf(s->val + pos, fmt, va); 
  
#elif defined(__APPLE__)
  
  va_list va_tmp;
  va_copy(va_tmp, va);
  char* tmp = NULL;
  int size = vasprintf(&tmp, fmt, va_tmp);
  va_end(va_tmp);
  
  s->val = realloc(s->val, pos + size + 1);
  if (s->val == NULL) {
    throw(OutOfMemoryError, "Cannot allocate string, out of memory!");
  }
  
  s->val[pos] = '\0';
  strcat(s->val, tmp);
  free(tmp);
  
  return size;
  
#else

  va_list va_tmp;
  va_copy(va_tmp, va);
  int size = vsnprintf(NULL, 0, fmt, va_tmp);
  va_end(va_tmp);
  
  s->val = realloc(s->val, pos + size + 1);
  if (s->val == NULL) {
    throw(OutOfMemoryError, "Cannot allocate string, out of memory!");
  }
  
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

var String = typedecl(String,
  typeclass(Doc,
    String_Name, String_Brief, String_Description,
    String_Examples, String_Methods),
  typeclass(New, String_New, String_Del, String_Size),
  typeclass(Assign, String_Assign),
  typeclass(Copy, String_Copy),
  typeclass(Eq, String_Eq),
  typeclass(Ord, String_Gt, String_Lt),
  typeclass(Len, String_Len),
  typeclass(Get, NULL, NULL, String_Mem, String_Rem),
  typeclass(Clear, String_Clear),
  typeclass(Reverse, String_Reverse),
  typeclass(Hash, String_Hash),
  typeclass(C_Str, String_C_Str),
  typeclass(Format, String_Format_To, String_Format_From),
  typeclass(Show, String_Show, String_Look));

