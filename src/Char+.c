#include "Char+.h"

var Char = methods {
  methods_begin(Char),
  method(Char, New),
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

var Char_Copy(var self) {
  CharData* cd = cast(self, Char);
  return new(Char, cd->value);
}

bool Char_Eq(var self, var other) {
  CharData* cd = cast(self, Char);
  return (cd->value == as_char(other));
}

bool Char_Gt(var self, var other) {
  CharData* cd = cast(self, Char);
  return (cd->value > as_char(other));
}

bool Char_Lt(var self, var other) {
  CharData* cd = cast(self, Char);
  return (cd->value < as_char(other));
}

long Char_Hash(var self) {
  CharData* cd = cast(self, Char);
  return cd->value;
}

char Char_AsChar(var self) {
  CharData* cd = cast(self, Char);
  return cd->value;
}