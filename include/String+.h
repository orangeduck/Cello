#ifndef StringPlus_h
#define StringPlus_h

#include "Prelude+.h"
#include "Type+.h"

module String;

data {
  var type;
  char* value;
} StringData;

var String_New(var, va_list*);
var String_Delete(var);
var String_Copy(var);

bool String_Eq(var, var);

int String_Len(var);
bool String_IsEmpty(var);
void String_Clear(var);
bool String_Contains(var, var);
void String_Discard(var, var);

long String_Hash(var);

const char* String_AsStr(var);

instance(String, New) = { sizeof(StringData), String_New, String_Delete };
instance(String, Copy) = { String_Copy };
instance(String, Eq) = { String_Eq };
instance(String, Collection) = { String_Len, String_IsEmpty, String_Clear, String_Contains, String_Discard };
instance(String, Hash) = { String_Hash  };
instance(String, AsStr) = { String_AsStr  };


#endif