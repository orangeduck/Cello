/*
** == Char ==
**
**  Basic char wrapper
*/

#ifndef CharPlus_h
#define CharPlus_h

#include "Prelude+.h"
#include "Type+.h"

module Char;

data {
  var type;
  char value;
} CharData;

/** Char_New(var self, char value); */
var Char_New(var self, va_list* args);
var Char_Delete(var self);
void Char_Assign(var self, var obj);
var Char_Copy(var self);

bool Char_Eq(var self, var obj);
bool Char_Gt(var self, var obj);
bool Char_Lt(var self, var obj);

long Char_Hash(var self);

char Char_AsChar(var self);

instance(Char, New) = { sizeof(CharData), Char_New, Char_Delete };
instance(Char, Assign) = { Char_Assign };
instance(Char, Copy) = { Char_Copy };
instance(Char, Eq) = { Char_Eq };
instance(Char, Ord) = { Char_Gt, Char_Lt };
instance(Char, Hash) = { Char_Hash  };
instance(Char, AsChar) = { Char_AsChar  };


#endif