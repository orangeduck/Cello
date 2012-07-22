#ifndef CharPlus_h
#define CharPlus_h

#include "Prelude+.h"
#include "Type+.h"

module Char;

data {
  var type;
  char value;
} CharData;

/** Char_New(var, char); */
var Char_New(var, va_list*);
var Char_Delete(var);
void Char_Assign(var, var);
var Char_Copy(var);

bool Char_Eq(var, var);
bool Char_Gt(var, var);
bool Char_Lt(var, var);

long Char_Hash(var);

char Char_AsChar(var);

instance(Char, New) = { sizeof(CharData), Char_New, Char_Delete };
instance(Char, Assign) = { Char_Assign };
instance(Char, Copy) = { Char_Copy };
instance(Char, Eq) = { Char_Eq };
instance(Char, Ord) = { Char_Gt, Char_Lt };
instance(Char, Hash) = { Char_Hash  };
instance(Char, AsChar) = { Char_AsChar  };


#endif