#include "Char+.h"

var Char = methods {
  methods_begin(Char),
  method(Char, New),
  method(Char, Assign),
  method(Char, Copy),
  method(Char, Eq),
  method(Char, Ord),
  method(Char, Hash),
  method(Char, AsChar),
  methods_end(Char)
};

var Char_New(var self, va_list* args) {
  CharData* cd = cast(self, Char);
  cd->value = va_arg(*args, int);
  return self;
}

var Char_Delete(var self) {
  return self;
}

void Char_Assign(var self, var obj) {
  CharData* cd = cast(self, Char);
  cd->value = as_char(obj);
}

var Char_Copy(var self) {
  CharData* cd = cast(self, Char);
  return new(Char, cd->value);
}

var Char_Eq(var self, var other) {
  CharData* cd = cast(self, Char);
  return (var)(cd->value == as_char(other));
}

var Char_Gt(var self, var other) {
  CharData* cd = cast(self, Char);
  return (var)(cd->value > as_char(other));
}

var Char_Lt(var self, var other) {
  CharData* cd = cast(self, Char);
  return (var)(cd->value < as_char(other));
}

long Char_Hash(var self) {
  CharData* cd = cast(self, Char);
  return cd->value;
}

char Char_AsChar(var self) {
  CharData* cd = cast(self, Char);
  return cd->value;
}
