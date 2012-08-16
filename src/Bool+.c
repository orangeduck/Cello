
#include "Prelude+.h"
#include "Type+.h"

#include "Bool+.h"

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
  return (var)(self == obj);
}

var Bool_Gt(var self, var obj) {
  return (var)(self > obj);
}

var Bool_Lt(var self, var obj) {
  return (var)(self < obj);
}

long Bool_Hash(var self) {
  return (long)self;
}

char Bool_AsChar(var self) {
  return (char)(self is True);
}

static const char* true_str = "True";
static const char* false_str = "False";

const char* Bool_AsStr(var self) {
  if (self) {
    return true_str;
  } else {
    return false_str;
  }
}

long Bool_AsLong(var self) {
  return (long)self;
}

double Bool_AsDouble(var self) {
  return (double)(self is True);
}

