#include "Cello/Number.h"

#include "Cello/Type.h"
#include "Cello/Bool.h"
#include "Cello/Exception.h"

#include <math.h>
#include <stdio.h>

void add(var lhs, var rhs) {
  return type_class_method(type_of(lhs), Num, add, lhs, rhs);
}

void sub(var lhs, var rhs) {
  return type_class_method(type_of(lhs), Num, sub, lhs, rhs);
}

void mul(var lhs, var rhs) {
  return type_class_method(type_of(lhs), Num, mul, lhs, rhs);
}

void divide(var lhs, var rhs) {
  return type_class_method(type_of(lhs), Num, div, lhs, rhs);
}

void negate(var self) {
  return type_class_method(type_of(self), Num, negate, self);
}

void absolute(var self) {
  return type_class_method(type_of(self), Num, absolute, self);
}

var Int = type_data {
  type_begin(Int),
  type_entry(Int, New), 
  type_entry(Int, Assign),
  type_entry(Int, Copy),
  type_entry(Int, Eq), 
  type_entry(Int, Ord),
  type_entry(Int, Hash),
  type_entry(Int, AsLong),
  type_entry(Int, AsDouble),
  type_entry(Int, Num),
  type_entry(Int, Serialize),
  type_entry(Int, Show),
  type_end(Int)
};

var Int_New(var self, var_list vl) {
  IntData* intdata = cast(self, Int);
  intdata->value = as_long(var_list_get(vl));
  return self;
}

var Int_Delete(var self) {
  return self;
}

size_t Int_Size(void) {
  return sizeof(IntData);
}

void Int_Assign(var self, var obj) {
  IntData* intdata = cast(self, Int);
  intdata->value = as_long(obj);
}

var Int_Copy(var self) {
  return new(Int, self);
}

var Int_Eq(var self, var other) {
  IntData* io = cast(self, Int);
  if (type_implements(type_of(other), AsLong)) {
    return bool_var(io->value == as_long(other));
  } else {
    return False;
  }
}

var Int_Gt(var self, var other) {
  IntData* io = cast(self, Int);
  return bool_var(io->value > as_long(other));
}
var Int_Lt(var self, var other) {
  IntData* io = cast(self, Int);
  return bool_var(io->value < as_long(other));
}

long Int_Hash(var self) {
  IntData* io = cast(self, Int);
  return io->value;
}

void Int_Add(var self, var other) {
  IntData* io = cast(self, Int);
  io->value += as_long(other);
}

void Int_Sub(var self, var other) {
  IntData* io = cast(self, Int);
  io->value -= as_long(other);
}

void Int_Mul(var self, var other) {
  IntData* io = cast(self, Int);
  io->value *= as_long(other);
}

void Int_Div(var self, var other) {
  IntData* io = cast(self, Int);
  io->value /= as_long(other);
}

void Int_Neg(var self) {
  IntData* io = cast(self, Int);
  io->value = -io->value;
}

void Int_Abs(var self) {
  IntData* io = cast(self, Int);
  io->value = abs(io->value);
}

void Int_Serial_Read(var self, var stream) {
  IntData* io = cast(self, Int);
  stream_read(stream, &io->value, sizeof(long));
}

void Int_Serial_Write(var self, var stream) {
  IntData* io = cast(self, Int);
  stream_write(stream, &io->value, sizeof(long));
}

long Int_AsLong(var self) {
  IntData* io = cast(self, Int);
  return io->value;
}

double Int_AsDouble(var self) {
  IntData* io = cast(self, Int);
  return io->value;
}

int Int_Show(var self, var output, int pos) {
  IntData* io = cast(self, Int);
  pos += format_to(output, pos, "%li", io->value);
  return pos;
}

int Int_Look(var self, var input, int pos) {
  IntData* io = cast(self, Int);
  int off = 0;
  int err = format_from(input, pos, "%li%n", &io->value, &off);
  pos += off;
  return pos;
}

var Real = type_data {
  type_begin(Real),
  type_entry(Real, New),
  type_entry(Real, Assign),
  type_entry(Real, Copy),
  type_entry(Real, Eq),
  type_entry(Real, Ord),
  type_entry(Real, Hash),
  type_entry(Real, AsDouble),
  type_entry(Real, AsLong), 
  type_entry(Real, Num),
  type_entry(Real, Serialize),
  type_entry(Real, Show), 
  type_end(Real)
};

var Real_New(var self, var_list vl) {
  RealData* ro = cast(self, Real);
  ro->value = as_double(var_list_get(vl));
  return self;
}

var Real_Delete(var self) {
  return self;
}

size_t Real_Size(void) {
  return sizeof(RealData);
}

void Real_Assign(var self, var obj) {
  RealData* ro = cast(self, Real);
  ro->value = as_double(obj);
}

var Real_Copy(var self) {
  return new(Real, self);
}

var Real_Eq(var self, var other) {
  RealData* ro = cast(self, Real);
  if (type_implements(type_of(other), AsDouble)) {
    return bool_var(ro->value == as_double(other));
  } else {
    return False;
  }
}

var Real_Gt(var self, var other) {
  RealData* ro = cast(self, Real);
  return bool_var(ro->value > as_double(other));
}

var Real_Lt(var self, var other) {
  RealData* ro = cast(self, Real);
  return bool_var(ro->value < as_double(other));
}

union interp_cast {
  double as_double;
  long as_long;
};

long Real_Hash(var self) {
  RealData* ro = cast(self, Real);
  union interp_cast ic;
  ic.as_double = ro->value;
  return ic.as_long;
}

void Real_Add(var self, var other) {
  RealData* ro = cast(self, Real);
  ro->value += as_double(other);
}

void Real_Sub(var self, var other) {
  RealData* ro = cast(self, Real);
  ro->value -= as_double(other);
}

void Real_Mul(var self, var other) {
  RealData* ro = cast(self, Real);
  ro->value *= as_double(other);
}

void Real_Div(var self, var other) {
  RealData* ro = cast(self, Real);
  ro->value /= as_double(other);
}

void Real_Neg(var self) {
  RealData* ro = cast(self, Real);
  ro->value = - ro->value;
}

void Real_Abs(var self) {
  RealData* ro = cast(self, Real);
  ro->value = fabs(ro->value);
}

void Real_Serial_Read(var self, var stream) {
  RealData* ro = cast(self, Real);
  stream_read(stream, &ro->value, sizeof(double));
}

void Real_Serial_Write(var self, var stream) {
  RealData* ro = cast(self, Real);
  stream_write(stream, &ro->value, sizeof(double));
}

double Real_AsDouble(var self) {
  RealData* ro = cast(self, Real);
  return ro->value;
}

long Real_AsLong(var self) {
  RealData* ro = cast(self, Real);
  return ro->value;
}

int Real_Show(var self, var output, int pos) {
  return print_to(output, pos, "%f", self);
}

int Real_Look(var self, var input, int pos) {
  return scan_from(input, pos, "%f", self);
}

