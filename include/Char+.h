#ifndef CharPlus_h
#define CharPlus_h

#include "Prelude+.h"
#include "Type+.h"

module Char;

data {
  var type;
  char value;
} CharData;

var Char_New(var, va_list*);
var Char_Delete(var);
var Char_Copy(var);

bool Char_Eq(var, var);
bool Char_Gt(var, var);
bool Char_Lt(var, var);

long Char_Hash(var);

char Char_AsChar(var);

instance(String, New) = { sizeof(CharData), Char_New, Char_Delete };
instance(String, Copy) = { Char_Copy };
instance(String, Eq) = { Char_Eq };
instance(String, Ord) = { Char_Gt, Char_Lt };
instance(String, Hash) = { Char_Hash  };
instance(String, AsChar) = { Char_AsChar  };


#endif