#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "+Number.h"

var add(var lhs, var rhs) {
  Num* inum = type_class(type_of(lhs), Num);
  assert(inum->add);
  return inum->add(lhs, rhs);
}

var sub(var lhs, var rhs) {
  Num* inum = type_class(type_of(lhs), Num);
  assert(inum->sub);
  return inum->sub(lhs, rhs);
}

var mul(var lhs, var rhs) {
  Num* inum = type_class(type_of(lhs), Num);
  assert(inum->mul);
  return inum->mul(lhs, rhs);
}

var divide(var lhs, var rhs) {
  Num* inum = type_class(type_of(lhs), Num);
  assert(inum->div);
  return inum->div(lhs, rhs);
}

var negate(var self) {
  Num* inum = type_class(type_of(self), Num);
  assert(inum->negate);
  return inum->negate(self);
}

var absolute(var self) {
  Num* inum = type_class(type_of(self), Num);
  assert(inum->absolute);
  return inum->absolute(self);
}

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

var Int_Add(var self, var other) {
  IntObject* io = cast(self, Int);
  io->value += as_long(other);
  return io;
}

var Int_Sub(var self, var other) {
  IntObject* io = cast(self, Int);
  io->value -= as_long(other);
  return io;
}

var Int_Mul(var self, var other) {
  IntObject* io = cast(self, Int);
  io->value *= as_long(other);
  return io;
}

var Int_Div(var self, var other) {
  IntObject* io = cast(self, Int);
  io->value /= as_long(other);
  return io;
}

var Int_Neg(var self) {
  IntObject* io = cast(self, Int);
  io->value = - io->value;
  return self;
}

var Int_Abs(var self) {
  IntObject* io = cast(self, Int);
  io->value = abs(io->value);
  return self;
}

long Int_AsLong(var self) {
  IntObject* io = cast(self, Int);
  return io->value;
}

double Int_AsDouble(var self) {
  IntObject* io = cast(self, Int);
  return io->value;
}

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

var Real_Add(var self, var other) {
  RealObject* ro = cast(self, Real);
  ro->value += as_double(other);
  return ro;
}

var Real_Sub(var self, var other) {
  RealObject* ro = cast(self, Real);
  ro->value -= as_double(other);
  return ro;
}

var Real_Mul(var self, var other) {
  RealObject* ro = cast(self, Real);
  ro->value *= as_double(other);
  return ro;
}

var Real_Div(var self, var other) {
  RealObject* ro = cast(self, Real);
  ro->value /= as_double(other);
  return ro;
}

var Real_Neg(var self) {
  RealObject* ro = cast(self, Real);
  ro->value = - ro->value;
  return self;
}

var Real_Abs(var self) {
  RealObject* ro = cast(self, Real);
  ro->value = fabs(ro->value);
  return self;
}

double Real_AsDouble(var self) {
  RealObject* ro = cast(self, Real);
  return ro->value;
}

long Real_AsLong(var self) {
  RealObject* ro = cast(self, Real);
  return ro->value;
}
