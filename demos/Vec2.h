
#include "Cello.h"

/** Vector - vector operations */

class {
  float (*dot)(var, var);
  float (*length)(var);
} Vector;

float dot(var self, var obj);
float length(var self);


/** Vec2 - vector 2d data type */

global var Vec2;

data {
  var type;
  float x, y;
} Vec2Data;

/** Vec2_New(var self, float x, float y); */
var Vec2_New(var self, va_list* args);
var Vec2_Delete(var self);
void Vec2_Assign(var self, var obj);
var Vec2_Copy(var self);

var Vec2_Eq(var self, var obj);

int Vec2_Show(var self, var output, int pos);
int Vec2_Look(var self, var input, int pos);

float Vec2_Dot(var self, var obj);
float Vec2_Length(var self);

instance(Vec2, New) = { sizeof(Vec2Data), Vec2_New, Vec2_Delete };
instance(Vec2, Assign) = { Vec2_Assign };
instance(Vec2, Copy) = { Vec2_Copy };
instance(Vec2, Eq) = { Vec2_Eq };
instance(Vec2, Show) = { Vec2_Show, Vec2_Look };
instance(Vec2, Vector) = { Vec2_Dot, Vec2_Length };
