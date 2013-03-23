/*
** == String ==
**
**  Basic string wrapper
*/

#ifndef StringPlus_h
#define StringPlus_h

#include "Prelude+.h"
#include "Type+.h"

global var String;

data {
  var type;
  char* value;
} StringData;

/** String_New(var self, char* value); */
var String_New(var self, va_list* args);
var String_Delete(var self);
void String_Assign(var self, var obj);
var String_Copy(var self);

var String_Eq(var self, var obj);
var String_Gt(var self, var obj);
var String_Lt(var self, var obj);

int String_Len(var self);
void String_Clear(var self);
var String_Contains(var self, var sub);
void String_Discard(var self, var sub);

long String_Hash(var self);

void String_Parse_Read(var self, var stream);
void String_Parse_Write(var self, var stream);

const char* String_AsStr(var self);

void String_Append(var self, var obj);
void String_Reverse(var self);

instance(String, New) = { sizeof(StringData), String_New, String_Delete };
instance(String, Assign) = { String_Assign };
instance(String, Copy) = { String_Copy };
instance(String, Eq) = { String_Eq };
instance(String, Ord) = { String_Gt, String_Lt };
instance(String, Collection) = { String_Len, String_Clear, String_Contains, String_Discard };
instance(String, Hash) = { String_Hash  };
instance(String, Reverse) = { String_Reverse  };
instance(String, Parse) = { String_Parse_Read, String_Parse_Write };
instance(String, AsStr) = { String_AsStr  };
instance(String, Append) = { String_Append  };


#endif
