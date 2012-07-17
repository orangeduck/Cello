#include "Char+.h"

var Char_New(var self, va_list* args) {
  CharData* cd = cast(self, Char);
  cd->value = va_arg(*args, char);
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
  return (cd->value == to_char(other));
}

bool Char_Gt(var self, var other) {
  CharData* cd = cast(self, Char);
  return (cd->value > to_char(other));
}

bool Char_Lt(var self, var other) {
  CharData* cd = cast(self, Char);
  return (cd->value < to_char(other));
}

long Char_Hash(var self) {
  CharData* cd = cast(self, Char);
  return cd->value;
}

char Char_AsChar(var self) {
  CharData* cd = cast(self, Char);
  return cd->value;
}