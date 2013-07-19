/*
** == String ==
**
**  Basic string wrapper
*/

#ifndef CelloString_h
#define CelloString_h

#include "Prelude.h"
#include "Type.h"
#include "Format.h"

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


const char* String_AsStr(var self);

void String_Append(var self, var obj);
void String_Reverse(var self);

int String_Format_To(var self, int pos, const char* fmt, va_list va);
int String_Format_From(var self, int pos, const char* fmt, va_list va);

int String_Show(var self, var out, int pos);
int String_Look(var self, var input, int pos);

instance(String, New) = { sizeof(StringData), String_New, String_Delete };
instance(String, Assign) = { String_Assign };
instance(String, Copy) = { String_Copy };
instance(String, Eq) = { String_Eq };
instance(String, Ord) = { String_Gt, String_Lt };
instance(String, Collection) = { String_Len, String_Clear, String_Contains, String_Discard };
instance(String, Hash) = { String_Hash };
instance(String, Reverse) = { String_Reverse };
instance(String, AsStr) = { String_AsStr };
instance(String, Append) = { String_Append };
instance(String, Format) = { String_Format_To, String_Format_From };
instance(String, Show) = { String_Show, String_Look };

#endif
