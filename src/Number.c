#include "Cello/Number.h"

#include "Cello/Type.h"
#include "Cello/Bool.h"
#include "Cello/Exception.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

void add(var lhs, var rhs) {
  Num* inum = type_class(type_of(lhs), Num);
  assert(inum->add);
  inum->add(lhs, rhs);
}

void sub(var lhs, var rhs) {
  Num* inum = type_class(type_of(lhs), Num);
  assert(inum->sub);
  inum->sub(lhs, rhs);
}

void mul(var lhs, var rhs) {
  Num* inum = type_class(type_of(lhs), Num);
  assert(inum->mul);
  inum->mul(lhs, rhs);
}

void divide(var lhs, var rhs) {
  Num* inum = type_class(type_of(lhs), Num);
  assert(inum->div);
  inum->div(lhs, rhs);
}

void negate(var self) {
  Num* inum = type_class(type_of(self), Num);
  assert(inum->negate);
  inum->negate(self);
}

void absolute(var self) {
  Num* inum = type_class(type_of(self), Num);
  assert(inum->absolute);
  inum->absolute(self);
}

var Int = methods {
  methods_begin(Int),
  method(Int, New), 
  method(Int, Assign),
  method(Int, Copy),
  method(Int, Eq), 
  method(Int, Ord),
  method(Int, Hash),
  method(Int, AsLong),
  method(Int, AsDouble),
  method(Int, Num),
  method(Int, Serialize),
  method(Int, Show),
  methods_end(Int)
};

var Int_New(var self, va_list* args) {
  IntData* intdata = cast(self, Int);
  intdata->value = va_arg(*args, int);
  return self;
}

var Int_Delete(var self) {
  return self;
}

void Int_Assign(var self, var obj) {
  IntData* intdata = cast(self, Int);
  intdata->value = as_long(obj);
}

var Int_Copy(var self) {
  IntData* intdata = cast(self, Int);
  return new(Int, intdata->value);
}

var Int_Eq(var self, var other) {
  IntData* io = cast(self, Int);
  if (type_implements(type_of(other), AsLong)) {
    return (var)(intptr_t)(io->value == as_long(other));
  } else {
    return False;
  }
}

var Int_Gt(var self, var other) {
  IntData* io = cast(self, Int);
  return (var)(intptr_t)(io->value > as_long(other));
}
var Int_Lt(var self, var other) {
  IntData* io = cast(self, Int);
  return (var)(intptr_t)(io->value < as_long(other));
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
  read(stream, &io->value, sizeof(long));
}

void Int_Serial_Write(var self, var stream) {
  IntData* io = cast(self, Int);
  write(stream, &io->value, sizeof(long));
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

var Real = methods {
  methods_begin(Real),
  method(Real, New),
  method(Real, Assign),
  method(Real, Copy),
  method(Real, Eq),
  method(Real, Ord),
  method(Real, Hash),
  method(Real, AsDouble),
  method(Real, AsLong), 
  method(Real, Num),
  method(Real, Serialize),
  method(Real, Show), 
  methods_end(Real)
};

var Real_New(var self, va_list* args) {
  RealData* ro = cast(self, Real);
  ro->value = va_arg(*args, double);
  return self;
}

var Real_Delete(var self) {
  return self;
}

void Real_Assign(var self, var obj) {
  RealData* ro = cast(self, Real);
  ro->value = as_double(obj);
}

var Real_Copy(var self) {
  RealData* ro = cast(self, Real);
  return new(Real, ro->value);
}

var Real_Eq(var self, var other) {
  RealData* ro = cast(self, Real);
  if (type_implements(type_of(other), AsDouble)) {
    return (var)(intptr_t)(ro->value == as_double(other));
  } else {
    return False;
  }
}

var Real_Gt(var self, var other) {
  RealData* ro = cast(self, Real);
  return (var)(intptr_t)(ro->value > as_double(other));
}

var Real_Lt(var self, var other) {
  RealData* ro = cast(self, Real);
  return (var)(intptr_t)(ro->value < as_double(other));
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
  read(stream, &ro->value, sizeof(double));
}

void Real_Serial_Write(var self, var stream) {
  RealData* ro = cast(self, Real);
  write(stream, &ro->value, sizeof(double));
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

