#ifndef NumberPlus_h
#define NumberPlus_h

#include "Prelude+.h"
#include "Type+.h"

/** Num - performs numeric operations */

class {
  void (*add)(var, var);
  void (*sub)(var, var);
  void (*mul)(var, var);
  void (*div)(var, var);
  void (*negate)(var);
  void (*absolute)(var);
} Num;

void add(var, var);
void sub(var, var);
void mul(var, var);
void divide(var, var);

void negate(var);
void absolute(var);

/* Integer */

module Int;

data {
  var type;
  long value;
} IntData;

/** Int_New(var, long); */
var Int_New(var, va_list*);
var Int_Delete(var);
void Int_Assign(var, var);
var Int_Copy(var);

bool Int_Eq(var, var);
bool Int_Gt(var, var);
bool Int_Lt(var, var);

long Int_Hash(var);

void Int_Add(var, var);
void Int_Sub(var, var);
void Int_Mul(var, var);
void Int_Div(var, var);
void Int_Neg(var);
void Int_Abs(var);

long Int_AsLong(var);
double Int_AsDouble(var);

instance(Int, New) = { sizeof(IntData), Int_New, Int_Delete };
instance(Int, Assign) = { Int_Assign };
instance(Int, Copy) = { Int_Copy };
instance(Int, Eq) = { Int_Eq };
instance(Int, Ord) = { Int_Gt, Int_Lt };
instance(Int, Hash) = { Int_Hash };
instance(Int, AsLong) = { Int_AsLong };
instance(Int, AsDouble) = { Int_AsDouble };
instance(Int, Num) = { Int_Add, Int_Sub, Int_Mul, Int_Div, Int_Neg, Int_Abs };

/* Real */

module Real;

data {
  var type;
  double value;
} RealData;

/** Real_New(var, double); */
var Real_New(var, va_list*);
var Real_Delete(var);
void Real_Assign(var, var);
var Real_Copy(var);

bool Real_Eq(var, var);
bool Real_Gt(var, var);
bool Real_Lt(var, var);

long Real_Hash(var);

void Real_Add(var, var);
void Real_Sub(var, var);
void Real_Mul(var, var);
void Real_Div(var, var);
void Real_Neg(var);
void Real_Abs(var);

double Real_AsDouble(var);
long Real_AsLong(var);

instance(Real, New) = { sizeof(RealData), Real_New, Real_Delete };
instance(Real, Assign) = { Real_Assign };
instance(Real, Copy) = { Real_Copy };
instance(Real, Eq) = { Real_Eq };
instance(Real, Ord) = { Real_Gt, Real_Lt };
instance(Real, Hash) = { Real_Hash };
instance(Real, AsDouble) = { Real_AsDouble };
instance(Real, AsLong) = { Real_AsLong };
instance(Real, Num) = { Real_Add, Real_Sub, Real_Mul, Real_Div, Real_Neg, Real_Abs };


#endif