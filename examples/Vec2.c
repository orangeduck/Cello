#include "Cello.h"

/*
extern var Vector;

struct Vector {
  float (*dot)(var, var);
  float (*length)(var);
};

float dot(var self, var obj);
float length(var self);

extern var Vec2;

struct Vec2 {
  float x, y;
};

static float dot(var self, var obj) {
  return method(self, Vector, dot, obj);
}

static float length(var self) {
  return method(self, Vector, length);
}

static var Vec2_New(var self, var_list vl) {
  struct Vec2* v = cast(self, Vec2);
  v->x = as_double(var_list_get(vl));
  v->y = as_double(var_list_get(vl));
  return self;
}

static var Vec2_Delete(var self) {
  return self;
}

static size_t Vec2_Size(void) {
  return sizeof(struct Vec2);
}

static void Vec2_Assign(var self, var obj) {
  struct Vec2* v1 = cast(self, Vec2);
  struct Vec2* v2 = cast(obj, Vec2);
  v1->x = v2->x;
  v1->y = v2->y;
}

static var Vec2_Copy(var self) {
  struct Vec2* v = cast(self, Vec2);
  return new(Vec2, $(Real, v->x), $(Real, v->y));
}

static var Vec2_Eq(var self, var obj) {
  struct Vec2* v1 = cast(self, Vec2);
  struct Vec2* v2 = cast(obj, Vec2);
  return boolean(v1->x is v2->x and v1->y is v2->y);
}

static int Vec2_Show(var self, var output, int pos) {
  struct Vec2* v = cast(self, Vec2);
  return print_to(output, pos, "(%f, %f)", $(Real, v->x), $(Real, v->y));
}

static int Vec2_Look(var self, var input, int pos) {
  struct Vec2* v = cast(self, Vec2);
  var x = $(Real, 0);
  var y = $(Real, 0);
  pos = scan_from(input, pos, "(%f, %f)", x, y);
  v->x = as_double(x);
  v->y = as_double(y);
  return pos;
}

static float Vec2_Dot(var self, var obj) {
  struct Vec2* v1 = cast(self, Vec2);
  struct Vec2* v2 = cast(obj, Vec2);
  return (v1->x * v2->x + v1->y * v2->y);
}

static float Vec2_Length(var self) {
  struct Vec2* v = cast(self, Vec2);
  return sqrt(v->x * v->x + v->y * v->y);
}

var Vec2 = typedecl(Vec2,
  typeclass(New, Vec2_New, Vec2_Delete, Vec2_Size),
  typeclass(Assign, Vec2_Assign),
  typeclass(Copy, Vec2_Copy),
  typeclass(Eq, Vec2_Eq),
  typeclass(Show, Vec2_Show, Vec2_Look),
  typeclass(Vector, Vec2_Dot, Vec2_Length));

*/

int main(int argc, char** argv) {
  return 0;
}
