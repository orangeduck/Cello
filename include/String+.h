#ifndef StringPlus_h
#define StringPlus_h

#include "Prelude+.h"

module String;

/* Members */

data {
  Type type;
  char* value;
} StringData;

/* Methods */

void String_New(var, va_list*);
void String_Delete(var);
var String_Copy(var);

bool String_Eq(var, var);

size_t String_Len(var);
bool String_Contains(var, var);

long String_Hash(var);

const char* String_AsStr(var);

/* Instances */

instance(String, New) = { sizeof(StringData), String_New, String_Delete };
instance(String, Copy) = { String_Copy };
instance(String, Eq) = { String_Eq };
instance(String, Len) = { String_Len  };
instance(String, Hash) = { String_Hash  };
instance(String, AsStr) = { String_AsStr  };


#endif