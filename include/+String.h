#ifndef PlusString_h
#define PlusString_h

#include "+Prelude.h"

object {
  Type type;
  char* value;
} StringObject;

void String_New(var, va_list*);
void String_Delete(var);
var String_Copy(var);

bool String_Eq(var, var);

size_t String_Len(var);
bool String_Contains(var, var);

const char* String_AsStr(var);

implements(String, New) as { sizeof(StringObject), String_New, String_Delete };
implements(String, Copy) as { String_Copy };
implements(String, Eq) as { String_Eq };
implements(String, Len) as { String_Len  };
implements(String, AsStr) as { String_AsStr  };

methods(String) as {
  method_begin(String),
  method(String, New), method(String, Copy),
  method(String, Eq), method(String, Len),
  method(String, AsStr), 
  method_end(String),
};


#endif