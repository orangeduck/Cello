#include <string.h>
#include <stdio.h>

#include "String+.h"

var String = methods {
  methods_begin(List),
  method(String, New),
  method(String, Assign),
  method(String, Copy),
  method(String, Eq),
  method(String, Collection),
  method(String, Hash),
  method(String, AsStr), 
  methods_end(List)
};

var String_New(var self, va_list* args) {
  StringData* s = cast(self, String);
  const char* init = va_arg(*args, const char*);
  s->value = malloc(strlen(init) + 1);
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
  strcpy(s->value, value);
}

var String_Copy(var self) {
  StringData* s = cast(self, String);
  return new(String, s->value);
}

bool String_Eq(var self, var other) {
  StringData* fst = cast(self, String);
  StringData* snd = cast(other, String);
  return (strcmp(fst->value, snd->value) == 0);
}

int String_Len(var self) {
  StringData* s = cast(self, String);
  return strlen(s->value);
}

bool String_IsEmpty(var self) {
  return (len(self) == 0);
}
void String_Clear(var self) {
  StringData* s = cast(self, String);
  s->value = realloc(s->value, 1);
  s->value[0] = '\0';
}

bool String_Contains(var self, var obj) {
  StringData* s = cast(self, String);
  
  if (Type_Implements(type_of(obj), AsStr)) {
    const char* ostr = as_str(obj);
    return (bool)strstr(s->value, ostr);
  }
  
  if (Type_Implements(type_of(obj), AsChar)) {
    char ochar = as_char(obj);
    return (bool)strchr(s->value, ochar);
  }
  
  return false;
}

/* TODO: Actually implement */

void String_Discard(var self, var obj) {
  StringData* s = cast(self, String);
  
  if (Type_Implements(type_of(obj), AsStr)) {
    const char* ostr = as_str(obj);
    const char* pos = strstr(s->value, ostr);
  }
  
  if (Type_Implements(type_of(obj), AsChar)) {
    char ochar = as_char(obj);
    const char* pos = strchr(s->value, ochar);
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

