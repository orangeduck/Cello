#ifndef TypePlus_h
#define TypePlus_h

#include "Prelude+.h"

module Type;

data {
  const void* class_object;
  const char* class_name;
} TypeData;

var Type_New(var, va_list*);
var Type_Delete(var);

#define cast(X, T) Type_Cast(X, T, __func__)
var Type_Cast(var, var, const char*);

#define Type_Implements(T, C) Type_Implements_Name(T, #C)
#define Type_Class(T, C) Type_Class_Name(T, #C)

bool Type_Implements_Name(var, const char* class_name);
var Type_Class_Name(var, const char* class_name);

const char* Type_AsStr(var);

instance(Type, New) = { 0, Type_New, Type_Delete };
instance(Type, AsStr) = { Type_AsStr };


#endif