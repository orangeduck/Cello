#include "Bool+.h"

#include "Prelude+.h"
#include "Type+.h"

var Bool = methods {
  methods_begin(Bool),
  method(Bool, Eq),
  method(Bool, Ord),
  method(Bool, Hash),
  method(Bool, AsChar),
  method(Bool, AsLong),
  method(Bool, AsDouble),
  method(Bool, AsStr),
  methods_end(Bool)
};

var Bool_Eq(var self, var obj) {
  return (var)(intptr_t)(self == obj);
}

var Bool_Gt(var self, var obj) {
  return (var)(intptr_t)(self > obj);
}

var Bool_Lt(var self, var obj) {
  return (var)(intptr_t)(self < obj);
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

