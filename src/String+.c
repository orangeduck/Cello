#include <string.h>
#include <stdio.h>

#include "String+.h"

Type String = methods {
  methods_begin(List),
  method(String, New),
  method(String, Copy),
  method(String, Eq),
  method(String, Len),
  method(String, Hash),
  method(String, AsStr), 
  methods_end(List)
};

void String_New(var self, va_list* args) {
  
  StringData* s = cast(self, String);
  const char* init = va_arg(*args, const char*);
  s->value = malloc(strlen(init) + 1);
  strcpy(s->value, init);
}

void String_Delete(var self) {
  StringData* s = cast(self, String);
  free(s->value);
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

size_t String_Len(var self) {
  StringData* s = cast(self, String);
  return strlen(s->value);
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

