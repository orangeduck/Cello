#include "Cello/String.h"

#include "Cello/Bool.h"
#include "Cello/Exception.h"

#include <string.h>
#include <stdio.h>

var String = methods {
  methods_begin(String),
  method(String, New),
  method(String, Assign),
  method(String, Copy),
  method(String, Eq),
  method(String, Ord),
  method(String, Collection),
  method(String, Hash),
  method(String, Reverse),
  method(String, AsStr), 
  method(String, Append), 
  method(String, Format),
  method(String, Show),
  methods_end(String)
};

var String_New(var self, va_list* args) {
  StringData* s = cast(self, String);
  const char* init = va_arg(*args, const char*);
  s->value = malloc(strlen(init) + 1);
  if (s->value == NULL) { throw(OutOfMemoryError, "Cannot allocate string, out of memory!"); }
  strcpy(s->value, init);
  return self;
}

var String_Delete(var self) {
  StringData* s = cast(self, String);
  free(s->value);
  return self;
}

void String_Assign(var self, var obj) {
  StringData* s = cast(self, String);
  const char* value = as_str(obj);
  s->value = realloc(s->value, strlen(value) + 1);
  if (s->value == NULL) { throw(OutOfMemoryError, "Cannot allocate string, out of memory!"); }
  strcpy(s->value, value);
}

var String_Copy(var self) {
  StringData* s = cast(self, String);
  return new(String, s->value);
}

var String_Eq(var self, var other) {
  StringData* fst = cast(self, String);
  if (type_implements(type_of(other), AsStr)) {
    return (var)(intptr_t)(strcmp(fst->value, as_str(other)) == 0);
  } else {
    return False;
  }
}

var String_Gt(var self, var obj) {
  StringData* s = cast(self, String);
  if (not type_implements(type_of(obj), AsStr)) return false;
  
  const char* fst = s->value;
  const char* snd = as_str(obj);
  
  int fstlen = strlen(fst);
  int sndlen = strlen(snd);
  int minlen = fstlen > sndlen ? sndlen : fstlen; 
  
  for(int i = 0; i < minlen; i++) {
    if (fst[i] > snd[i]) return True;
  }
  
  if (fstlen > sndlen) return True;
  
  return False;
}

var String_Lt(var self, var obj) {
  StringData* s = cast(self, String);
  if (not type_implements(type_of(obj), AsStr)) return false;
  
  const char* fst = s->value;
  const char* snd = as_str(obj);
  
  int fstlen = strlen(fst);
  int sndlen = strlen(snd);
  int minlen = fstlen > sndlen ? sndlen : fstlen; 
  
  for(int i = 0; i < minlen; i++) {
    if (fst[i] < snd[i]) return True;
  }
  
  if (fstlen < sndlen) return True;
  
  return False;
}


int String_Len(var self) {
  StringData* s = cast(self, String);
  return strlen(s->value);
}

void String_Clear(var self) {
  StringData* s = cast(self, String);
  s->value = realloc(s->value, 1);
  if (s->value == NULL) { throw(OutOfMemoryError, "Cannot allocate string, out of memory!"); }
  s->value[0] = '\0';
}

var String_Contains(var self, var obj) {
  StringData* s = cast(self, String);
  
  if (type_implements(type_of(obj), AsStr)) {
    const char* ostr = as_str(obj);
    if ( strstr(s->value, ostr) ) {
      return True;
    } else {
      return False;
    }
  }
  
  if (type_implements(type_of(obj), AsChar)) {
    char ochar = as_char(obj);
    if (strchr(s->value, ochar)) {
      return True;
    } else {
     return False;
    }
  }
  
  return False;
}



void String_Discard(var self, var obj) {
  StringData* s = cast(self, String);
  
  if (type_implements(type_of(obj), AsStr)) {
    const char* ostr = as_str(obj);
    const char* pos = strstr(s->value, ostr);
    
    int bytecount = strlen(s->value) - strlen(pos) - strlen(ostr) + 1;
    memmove((char*)pos, pos + strlen(ostr), bytecount);
  }
  
  if (type_implements(type_of(obj), AsChar)) {
    char ochar = as_char(obj);
    const char* pos = strchr(s->value, ochar);
    while(pos != NULL) {
      pos = pos+1;
    }
  }
  
}

long String_Hash(var self) {
  StringData* s = cast(self, String);
  
  int total = 1;
  int i = 0;
  while(  s->value[i] != '\0' ) {
    int value = (int)s->value[i];
    total = total + value + i;
    i++;
  }
  
  return abs(total);
}

const char* String_AsStr(var self) {
  StringData* s = cast(self, String);
  return s->value;
}

void String_Append(var self, var obj) {
  StringData* s = cast(self, String);
  const char* os = as_str(obj);
  
  size_t newlen = strlen(s->value) + strlen(os);
  
  s->value = realloc(s->value, newlen + 1);
  if (s->value == NULL) { throw(OutOfMemoryError, "Cannot allocate string, out of memory!"); }
  
  strcat(s->value, os);
}

void String_Reverse(var self) {
  StringData* s = cast(self, String);
  for(unsigned int i = 0; i < strlen(s->value) / 2; i++) {
    char temp = s->value[i];
    s->value[i] = s->value[strlen(s->value)-1-i];
    s->value[strlen(s->value)-1-i] = temp;
  }
}

int String_Format_To(var self, int pos, const char* fmt, va_list va) {

  StringData* s = cast(self, String);
  
#if defined(_WIN32)
  
  va_list va_tmp;
  va_copy(va_tmp, va);
  int size = _vscprintf(fmt, va_tmp);
  va_end(va_tmp);
  
  s->value = realloc(s->value, pos + size + 1);
  if (s->value == NULL) { throw(OutOfMemoryError, "Cannot allocate string, out of memory!"); }
  
  return vsprintf(s->value + pos, fmt, va); 
  
#elif defined(__APPLE__)
  
  va_list va_tmp;
  va_copy(va_tmp, va);
  char* tmp = NULL;
  int size = vasprintf(&tmp, fmt, va_tmp);
  va_end(va_tmp);
  
  s->value = realloc(s->value, pos + size + 1);
  if (s->value == NULL) { throw(OutOfMemoryError, "Cannot allocate string, out of memory!"); }
  
  s->value[pos] = '\0';
  strcat(s->value, tmp);
  free(tmp);
  
  return size;
  
#else

  va_list va_tmp;
  va_copy(va_tmp, va);
  int size = snprintf(NULL, 0, fmt, va_tmp);
  va_end(va_tmp);
  
  s->value = realloc(s->value, pos + size + 1);
  if (s->value == NULL) { throw(OutOfMemoryError, "Cannot allocate string, out of memory!"); }
  
  return vsprintf(s->value + pos, fmt, va); 
  
#endif

}

int String_Format_From(var self, int pos, const char* fmt, va_list va) {
  StringData* s = cast(self, String);
  return vsscanf(s->value + pos, fmt, va);
}

int String_Show(var self, var out, int pos) {
  return print_to(out, pos, "\"%s\"", self);
}

int String_Look(var self, var input, int pos) {
  return scan_from(input, pos, "\"%[^\"]\"", self);
}
