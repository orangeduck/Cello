#include "Vec2.h"

#include <math.h>

float dot(var self, var obj) {
  return type_class_method(type_of(self), Vector, dot, self, obj);
}

float length(var self) {
  return type_class_method(type_of(self), Vector, length, self);
}

var Vec2 = type_data {
  type_begin(Vec2),
  type_entry(Vec2, New),
  type_entry(Vec2, Assign),
  type_entry(Vec2, Copy),
  type_entry(Vec2, Eq),
  type_entry(Vec2, Show),
  type_entry(Vec2, Vector),
  type_end(Vec2)
};

var Vec2_New(var self, var_list vl) {
  Vec2Data* v = cast(self, Vec2);
  v->x = as_double(var_list_get(vl));
  v->y = as_double(var_list_get(vl));
  return self;
}

var Vec2_Delete(var self) {
  return self;
}

size_t Vec2_Size(void) {
  return sizeof(Vec2Data);
}

void Vec2_Assign(var self, var obj) {
  Vec2Data* v1 = cast(self, Vec2);
  Vec2Data* v2 = cast(obj, Vec2);
  v1->x = v2->x;
  v1->y = v2->y;
}

var Vec2_Copy(var self) {
  Vec2Data* v = cast(self, Vec2);
  return new(Vec2, $(Real, v->x), $(Real, v->y));
}

var Vec2_Eq(var self, var obj) {
  Vec2Data* v1 = cast(self, Vec2);
  Vec2Data* v2 = cast(obj, Vec2);
  return bool_var(v1->x is v2->x and v1->y is v2->y);
}

int Vec2_Show(var self, var output, int pos) {
  Vec2Data* v = cast(self, Vec2);
  return print_to(output, pos, "(%f, %f)", $(Real, v->x), $(Real, v->y));
}

int Vec2_Look(var self, var input, int pos) {
  Vec2Data* v = cast(self, Vec2);
  var x = $(Real, 0);
  var y = $(Real, 0);
  pos = scan_from(input, pos, "(%f, %f)", x, y);
  v->x = as_double(x);
  v->y = as_double(y);
  return pos;
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

int main(int argc, char** argv) {
  return 0;
}
