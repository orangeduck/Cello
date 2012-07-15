#ifndef PlusString_h
#define PlusString_h

#include "+Prelude.h"

extern Type String;

/* Members */

object {
  Type type;
  char* value;
} StringObject;

/* Methods */

void String_New(var, va_list*);
void String_Delete(var);
var String_Copy(var);

bool String_Eq(var, var);

size_t String_Len(var);
bool String_Contains(var, var);

const char* String_AsStr(var);

/* Instances */

instance(String, New) = { sizeof(StringObject), String_New, String_Delete };
instance(String, Copy) = { String_Copy };
instance(String, Eq) = { String_Eq };
instance(String, Len) = { String_Len  };
instance(String, AsStr) = { String_AsStr  };


#endif