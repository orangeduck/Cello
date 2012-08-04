#include <assert.h>

#include "Function+.h"
#include "List+.h"

var Function = methods {
  methods_begin(Function),
  method(Function, Call),
  methods_end(Function)
};

var call_with(var self, var args) {
  Call* icall = Type_Class(type_of(self), Call);
  assert(icall->call_with); 
  return icall->call_with(self, args);
}

var call_with_ptr(var self, int argc, var* args) {
  var arglist = $(List, argc, argc, args, 0);
  return call_with(self, arglist);
}

var Function_Call(var func, var args) {
  FunctionData* fd = cast(func, Function);
  return fd->func(args);
}
