/*
** == Bool ==
**
**  Boolean type which provide singleton data objects.
**  These objects evaluate to true or false in "if" statments.
**  They have no data object but are hardcoded into type_of
**
**  Normal boolean operators should work on Bool as expected.
**  And also "bool" can be cast to "var" and back again.
**
*/

#ifndef BoolPlus_h
#define BoolPlus_h

#include "Prelude+.h"

module Bool;

static const var True = (var)1;
static const var False = (var)0;

var Bool_Eq(var self, var obj);
var Bool_Gt(var self, var obj);
var Bool_Lt(var self, var obj);

long Bool_Hash(var self);
char Bool_AsChar(var self);
const char* Bool_AsStr(var self);
long Bool_AsLong(var self);
double Bool_AsDouble(var self);

instance(Bool, Eq) = { Bool_Eq };
instance(Bool, Ord) = { Bool_Gt, Bool_Lt };
instance(Bool, Hash) = { Bool_Hash };
instance(Bool, AsChar) = { Bool_AsChar };
instance(Bool, AsLong) = { Bool_AsLong };
instance(Bool, AsDouble) = { Bool_AsDouble };
instance(Bool, AsStr) = { Bool_AsStr };

#endif
