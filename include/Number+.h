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

void add(var self, var obj);
void sub(var self, var obj);
void mul(var self, var obj);
void divide(var self, var obj);

void negate(var self);
void absolute(var self);

/*
** == Int ==
**
**  Basic integer wrapper
*/

global var Int;

data {
  var type;
  long value;
} IntData;

/** Int_New(var self, long value); */
var Int_New(var self, va_list* args);
var Int_Delete(var self);
void Int_Assign(var self, var obj);
var Int_Copy(var self);

var Int_Eq(var self, var obj);
var Int_Gt(var self, var obj);
var Int_Lt(var self, var obj);

long Int_Hash(var self);

void Int_Add(var self, var obj);
void Int_Sub(var self, var obj);
void Int_Mul(var self, var obj);
void Int_Div(var self, var obj);
void Int_Neg(var self);
void Int_Abs(var self);

void Int_Parse_Read(var self, var stream);
void Int_Parse_Write(var self, var stream);

long Int_AsLong(var self);
double Int_AsDouble(var self);

int Int_Show_Size(var self);
int Int_Show(var self, char* out);

instance(Int, New) = { sizeof(IntData), Int_New, Int_Delete };
instance(Int, Assign) = { Int_Assign };
instance(Int, Copy) = { Int_Copy };
instance(Int, Eq) = { Int_Eq };
instance(Int, Ord) = { Int_Gt, Int_Lt };
instance(Int, Hash) = { Int_Hash };
instance(Int, Parse) = { Int_Parse_Read, Int_Parse_Write };
instance(Int, AsLong) = { Int_AsLong };
instance(Int, AsDouble) = { Int_AsDouble };
instance(Int, Num) = { Int_Add, Int_Sub, Int_Mul, Int_Div, Int_Neg, Int_Abs };
instance(Int, Show) = { Int_Show_Size, Int_Show };

/*
** == Real ==
**
**  Basic float/double wrapper
*/

global var Real;

data {
  var type;
  double value;
} RealData;

/** Real_New(var self, double value); */
var Real_New(var self, va_list* args);
var Real_Delete(var self);
void Real_Assign(var self, var obj);
var Real_Copy(var self);

var Real_Eq(var self, var obj);
var Real_Gt(var self, var obj);
var Real_Lt(var self, var obj);

long Real_Hash(var self);

void Real_Add(var self, var obj);
void Real_Sub(var self, var obj);
void Real_Mul(var self, var obj);
void Real_Div(var self, var obj);
void Real_Neg(var self);
void Real_Abs(var self);

void Real_Parse_Read(var self, var stream);
void Real_Parse_Write(var self, var stream);

double Real_AsDouble(var self);
long Real_AsLong(var self);

int Real_Show_Size(var self);
int Real_Show(var self, char* out);

instance(Real, New) = { sizeof(RealData), Real_New, Real_Delete };
instance(Real, Assign) = { Real_Assign };
instance(Real, Copy) = { Real_Copy };
instance(Real, Eq) = { Real_Eq };
instance(Real, Ord) = { Real_Gt, Real_Lt };
instance(Real, Hash) = { Real_Hash };
instance(Real, Parse) = { Real_Parse_Read, Real_Parse_Write };
instance(Real, AsDouble) = { Real_AsDouble };
instance(Real, AsLong) = { Real_AsLong };
instance(Real, Num) = { Real_Add, Real_Sub, Real_Mul, Real_Div, Real_Neg, Real_Abs };
instance(Real, Show) = { Real_Show_Size, Real_Show };

#endif
