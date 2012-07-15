#include <string.h>
#include <stdio.h>

#include "+String.h"

methods(String) = {
  methods_begin(List),
  method(String, New),
  method(String, Copy),
  method(String, Eq),
  method(String, Len),
  method(String, AsStr), 
  methods_end(List)
};

Type String = methods_table(String);

void String_New(var self, va_list* args) {
  
  StringObject* s = cast(self, String);
  const char* init = va_arg(*args, const char*);
  s->value = malloc(strlen(init) + 1);
  strcpy(s->value, init);
}

void String_Delete(var self) {
  StringObject* s = cast(self, String);
  free(s->value);
}

var String_Copy(var self) {
  StringObject* s = cast(self, String);
  return new(String, s->value);
}

bool String_Eq(var self, var other) {
  StringObject* fst = cast(self, String);
  StringObject* snd = cast(other, String);
  return (strcmp(fst->value, snd->value) == 0);
}

size_t String_Len(var self) {
  StringObject* s = cast(self, String);
  return strlen(s->value);
}

const char* String_AsStr(var self) {
  StringObject* s = cast(self, String);
  return s->value;
}

