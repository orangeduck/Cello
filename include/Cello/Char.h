/*
** == Char ==
**
**  Basic char wrapper
*/

#ifndef CelloChar_h
#define CelloChar_h

#include "Prelude.h"
#include "Type.h"

global var Char;

data {
  var type;
  char value;
} CharData;

/** Char_New(var self, char value); */
var Char_New(var self, va_list* args);
var Char_Delete(var self);
void Char_Assign(var self, var obj);
var Char_Copy(var self);

var Char_Eq(var self, var obj);
var Char_Gt(var self, var obj);
var Char_Lt(var self, var obj);

long Char_Hash(var self);

char Char_AsChar(var self);

void Char_Serial_Read(var self, var input);
void Char_Serial_Write(var self, var output);

int Char_Show(var self, var output, int pos);
int Char_Look(var self, var input, int pos);

instance(Char, New) = { sizeof(CharData), Char_New, Char_Delete };
instance(Char, Assign) = { Char_Assign };
instance(Char, Copy) = { Char_Copy };
instance(Char, Eq) = { Char_Eq };
instance(Char, Ord) = { Char_Gt, Char_Lt };
instance(Char, Hash) = { Char_Hash  };
instance(Char, AsChar) = { Char_AsChar  };
instance(Char, Serialize) = { Char_Serial_Read, Char_Serial_Write };
instance(Char, Show) = { Char_Show, Char_Look };

#endif
