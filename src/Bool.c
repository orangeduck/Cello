#include "Cello/Bool.h"

#include "Cello/Prelude.h"
#include "Cello/Type.h"

#include <string.h>

var bool_var(bool x) {
  return (var)(intptr_t)(x);
}

var Bool = type_data {
  type_begin(Bool),
  type_entry(Bool, Eq),
  type_entry(Bool, Ord),
  type_entry(Bool, Hash),
  type_entry(Bool, AsChar),
  type_entry(Bool, AsLong),
  type_entry(Bool, AsDouble),
  type_entry(Bool, AsStr),
  type_entry(Bool, Show),
  type_end(Bool)
};

var Bool_Eq(var self, var obj) {
  return bool_var(self == obj);
}

var Bool_Gt(var self, var obj) {
  return bool_var(self > obj);
}

var Bool_Lt(var self, var obj) {
  return bool_var(self < obj);
}

long Bool_Hash(var self) {
  return (long)(intptr_t)self;
}

char Bool_AsChar(var self) {
  return (char)(self is True);
}

local const char* true_str = "True";
local const char* false_str = "False";

const char* Bool_AsStr(var self) {
  if (self) {
    return true_str;
  } else {
    return false_str;
  }
}

long Bool_AsLong(var self) {
  return (long)(intptr_t)self;
}

double Bool_AsDouble(var self) {
  return (double)(self is True);
}

int Bool_Show(var self, var output, int pos) {
  if (self) { pos = print_to(output, pos, true_str); }
  else { pos = print_to(output, pos, false_str); }
  return pos;
}

