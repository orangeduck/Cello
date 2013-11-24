#include "Cello/Char.h"

#include "Cello/Bool.h"
#include "Cello/Exception.h"

var Char = type_data {
  type_begin(Char),
  type_entry(Char, New),
  type_entry(Char, Assign),
  type_entry(Char, Copy),
  type_entry(Char, Eq),
  type_entry(Char, Ord),
  type_entry(Char, Hash),
  type_entry(Char, AsChar),
  type_entry(Char, Serialize),
  type_entry(Char, Show),
  type_end(Char)
};

var Char_New(var self, var_list vl) {
  CharData* cd = cast(self, Char);
  cd->value = as_char(var_list_get(vl));
  return self;
}

var Char_Delete(var self) {
  return self;
}

size_t Char_Size(void) {
  return sizeof(CharData);
}

void Char_Assign(var self, var obj) {
  CharData* cd = cast(self, Char);
  cd->value = as_char(obj);
}

var Char_Copy(var self) {
  return new(Char, self);
}

var Char_Eq(var self, var other) {
  CharData* cd = cast(self, Char);
  return bool_var(cd->value == as_char(other));
}

var Char_Gt(var self, var other) {
  CharData* cd = cast(self, Char);
  return bool_var(cd->value > as_char(other));
}

var Char_Lt(var self, var other) {
  CharData* cd = cast(self, Char);
  return bool_var(cd->value < as_char(other));
}

long Char_Hash(var self) {
  CharData* cd = cast(self, Char);
  return cd->value;
}

char Char_AsChar(var self) {
  CharData* cd = cast(self, Char);
  return cd->value;
}

void Char_Serial_Read(var self, var input) {
  CharData* cd = cast(self, Char);
  stream_read(input, &cd->value, 1);
}

void Char_Serial_Write(var self, var output) {
  CharData* cd = cast(self, Char);
  stream_write(output, &cd->value, 1);
}

int Char_Show(var self, var output, int pos) {
  return print_to(output, pos, "%c", self);
}

int Char_Look(var self, var input, int pos) {
  return scan_from(input, pos, "%c", self);
}

