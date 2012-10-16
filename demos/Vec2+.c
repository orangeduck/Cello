#include "Vec2+.h"

#include <assert.h>
#include <math.h>

float dot(var self, var obj) {
  Vector* ivector = type_class(type_of(self), Vector);
  assert(ivector->dot);
  return ivector->dot(self, obj);
}

float length(var self) {
  Vector* ivector = type_class(type_of(self), Vector);
  assert(ivector->length);
  return ivector->length(self);
}

var Vec2 = methods {
  methods_begin(Vec2),
  method(Vec2, New),
  method(Vec2, Assign),
  method(Vec2, Copy),
  method(Vec2, Eq),
  methods_end(Vec2)
};

var Vec2_New(var self, va_list* args) {
  Vec2Data* v = cast(self, Vec2);
  v->x = va_arg(*args, double);
  v->y = va_arg(*args, double);
  return self;
}

var Vec2_Delete(var self) {
  return self;
}

void Vec2_Assign(var self, var obj) {
  Vec2Data* v1 = cast(self, Vec2);
  Vec2Data* v2 = cast(obj, Vec2);
  v1->x = v2->x;
  v1->y = v2->y;
}

var Vec2_Copy(var self) {
  Vec2Data* v = cast(self, Vec2);
  return new(Vec2, v->x, v->y);
}

var Vec2_Eq(var self, var obj) {
  Vec2Data* v1 = cast(self, Vec2);
  Vec2Data* v2 = cast(obj, Vec2);
  return (var)(v1->x is v2->x and v1->y is v2->y);
}

float Vec2_Dot(var self, var obj) {
  Vec2Data* v1 = cast(self, Vec2);
  Vec2Data* v2 = cast(obj, Vec2);
  return (v1->x * v2->x + v2->y * v2->y);
}

float Vec2_Length(var self) {
  Vec2Data* v = cast(self, Vec2);
  return sqrt(v->x * v->x + v->y * v->y);
}