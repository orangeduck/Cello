#ifndef PlusNumber_h
#define PlusNumber_h

#include "+Prelude.h"

/** Num - number */

class {
  var (*add)(var, var);
  var (*sub)(var, var);
  var (*mul)(var, var);
  var (*div)(var, var);
  var (*negate)(var);
  var (*absolute)(var);
} Num;

var add(var, var);
var sub(var, var);
var mul(var, var);
var divide(var, var);

var negate(var);
var absolute(var);

/* Integer Type */

object {
  Type type;
  long value;
} IntObject;

void Int_New(var, va_list*);
void Int_Delete(var);
var Int_Copy(var);

bool Int_Eq(var, var);
bool Int_Gt(var, var);
bool Int_Lt(var, var);

var Int_Add(var, var);
var Int_Sub(var, var);
var Int_Mul(var, var);
var Int_Div(var, var);
var Int_Neg(var);
var Int_Abs(var);

long Int_AsLong(var);
double Int_AsDouble(var);

implements(Int, New) as { sizeof(IntObject), Int_New, Int_Delete };
implements(Int, Copy) as { Int_Copy };
implements(Int, Eq) as { Int_Eq };
implements(Int, Ord) as { Int_Gt, Int_Lt };
implements(Int, AsLong) as { Int_AsLong  };
implements(Int, AsDouble) as { Int_AsDouble  };
implements(Int, Num) as { Int_Add, Int_Sub, Int_Mul, Int_Div, Int_Neg, Int_Abs };

methods(Int) as {
  method_begin(Int),
  method(Int, New), method(Int, Copy),
  method(Int, Eq), method(Int, Ord),
  method(Int, AsLong),method(Int, AsDouble),
  method(Int, Num),
  method_end(Int),
};

/* Real Type */

object {
  Type type;
  double value;
} RealObject;

void Real_New(var, va_list*);
void Real_Delete(var);
var Real_Copy(var);

bool Real_Eq(var, var);
bool Real_Gt(var, var);
bool Real_Lt(var, var);

var Real_Add(var, var);
var Real_Sub(var, var);
var Real_Mul(var, var);
var Real_Div(var, var);
var Real_Neg(var);
var Real_Abs(var);

double Real_AsDouble(var);
long Real_AsLong(var);

implements(Real, New) as { sizeof(RealObject), Real_New, Real_Delete };
implements(Real, Copy) as { Real_Copy };
implements(Real, Eq) as { Real_Eq };
implements(Real, Ord) as { Real_Gt, Real_Lt };
implements(Real, AsDouble) as { Real_AsDouble };
implements(Real, AsLong) as { Real_AsLong };
implements(Real, Num) as { Real_Add, Real_Sub, Real_Mul, Real_Div, Real_Neg, Real_Abs };

methods(Real) as {
  method_begin(Real),
  method(Real, New), method(Real, Copy),
  method(Real, Eq), method(Real, Ord),
  method(Real, AsDouble), method(Real, AsLong), 
  method(Real, Num), 
  method_end(Real),
};

#endif