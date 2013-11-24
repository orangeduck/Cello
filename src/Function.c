#include "Cello/Function.h"

#include "Cello/List.h"

var call_vl(var self, var_list vl) {
  
  var wrapped = new(List);
  
  while(not var_list_end(vl)) {
    push(wrapped, var_list_get(vl));
  }
  
  var res = call_with(self, wrapped);
  
  delete(wrapped);
  
  return res;
}

var call_with(var self, var args) {
  return type_class_method(type_of(self), Call, call_with, self, args);
}

var Function = type_data {
  type_begin(Function),
  type_entry(Function, New),
  type_entry(Function, Copy),
  type_entry(Function, Assign),
  type_entry(Function, Call),
  type_end(Function),
};

var Function_New(var self, var_list vl) {
  FunctionData* fd = cast(self, Function);
  fd->func = cast(var_list_get(vl), Function);
  return self;
}

var Function_Delete(var self) {
  return self;
}

size_t Function_Size(void) {
  return sizeof(FunctionData);
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
