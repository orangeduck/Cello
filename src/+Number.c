#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "+Number.h"

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

methods(Int) = {
  methods_begin(Int),
  method(Int, New), 
  method(Int, Copy),
  method(Int, Eq), 
  method(Int, Ord),
  method(Int, AsLong),
  method(Int, AsDouble),
  method(Int, Num),
  methods_end(Int)
};

Type Int = methods_table(Int);

void Int_New(var self, va_list* args) {
  IntObject* io = cast(self, Int);
  io->value = va_arg(*args, int);
}

void Int_Delete(var self) {}

var Int_Copy(var self) {
  IntObject* io = cast(self, Int);
  return new(Int, io->value);
}

bool Int_Eq(var self, var other) {
  IntObject* io = cast(self, Int);
  return io->value == as_long(other);
}

bool Int_Gt(var self, var other) {
  IntObject* io = cast(self, Int);
  return io->value > as_long(other);
}

bool Int_Lt(var self, var other) {
  IntObject* io = cast(self, Int);
  return io->value < as_long(other);
}

void Int_Add(var self, var other) {
  IntObject* io = cast(self, Int);
  io->value += as_long(other);
}

void Int_Sub(var self, var other) {
  IntObject* io = cast(self, Int);
  io->value -= as_long(other);
}

void Int_Mul(var self, var other) {
  IntObject* io = cast(self, Int);
  io->value *= as_long(other);
}

void Int_Div(var self, var other) {
  IntObject* io = cast(self, Int);
  io->value /= as_long(other);
}

void Int_Neg(var self) {
  IntObject* io = cast(self, Int);
  io->value = - io->value;
}

void Int_Abs(var self) {
  IntObject* io = cast(self, Int);
  io->value = abs(io->value);
}

long Int_AsLong(var self) {
  IntObject* io = cast(self, Int);
  return io->value;
}

double Int_AsDouble(var self) {
  IntObject* io = cast(self, Int);
  return io->value;
}

methods(Real) = {
  methods_begin(Real),
  method(Real, New), method(Real, Copy),
  method(Real, Eq), method(Real, Ord),
  method(Real, AsDouble), method(Real, AsLong), 
  method(Real, Num), 
  methods_end(Real)
};

Type Real = methods_table(Real);

void Real_New(var self, va_list* args) {
  RealObject* ro = cast(self, Real);
  ro->value = va_arg(*args, double);
}

void Real_Delete(var self) {}

var Real_Copy(var self) {
  RealObject* ro = cast(self, Real);
  return new(Real, ro->value);
}

bool Real_Eq(var self, var other) {
  RealObject* ro = cast(self, Real);
  return ro->value == as_double(other);
}

bool Real_Gt(var self, var other) {
  RealObject* ro = cast(self, Real);
  return ro->value > as_double(other);
}

bool Real_Lt(var self, var other) {
  RealObject* ro = cast(self, Real);
  return ro->value < as_double(other);
}

void Real_Add(var self, var other) {
  RealObject* ro = cast(self, Real);
  ro->value += as_double(other);
}

void Real_Sub(var self, var other) {
  RealObject* ro = cast(self, Real);
  ro->value -= as_double(other);
}

void Real_Mul(var self, var other) {
  RealObject* ro = cast(self, Real);
  ro->value *= as_double(other);
}

void Real_Div(var self, var other) {
  RealObject* ro = cast(self, Real);
  ro->value /= as_double(other);
}

void Real_Neg(var self) {
  RealObject* ro = cast(self, Real);
  ro->value = - ro->value;
}

void Real_Abs(var self) {
  RealObject* ro = cast(self, Real);
  ro->value = fabs(ro->value);
}

double Real_AsDouble(var self) {
  RealObject* ro = cast(self, Real);
  return ro->value;
}

long Real_AsLong(var self) {
  RealObject* ro = cast(self, Real);
  return ro->value;
}
