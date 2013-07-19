#include "Cello/Function.h"

#include "Cello/List.h"

#include <assert.h>

var call_with_ptr(var self, var* args) {
  int num = 0;
  while(args[num] != (var)-1) { num++; }
  return call_with(self, $(List, num, num, args, 0));
}

var call_with(var self, var args) {
  Call* icall = type_class(type_of(self), Call);
  assert(icall->call_with);
  return icall->call_with(self, args);
}

var Function = methods {
  methods_begin(Function),
  method(Function, New),
  method(Function, Copy),
  method(Function, Assign),
  method(Function, Call),
  methods_end(Function),
};

var Function_New(var self, va_list* args) {
  FunctionData* fd = cast(self, Function);
  fd->func = va_arg(*args, var);
  return self;
}

var Function_Delete(var self) {
  return self;
}

var Function_Copy(var self) {
  FunctionData* fd = cast(self, Function);
  return new(Function, fd->func);
}

void Function_Assign(var self, var obj) {
  FunctionData* fd = cast(self, Function);
  FunctionData* other = cast(obj, Function);
  fd->func = other->func;
}

var Function_Call(var self, var args) {
  FunctionData* fd = cast(self, Function);
  return fd->func(args);
}
