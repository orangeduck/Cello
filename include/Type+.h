/*
** == Type ==
**
**  Data object for types of data objects.
**
**  + Contains lists of implemented classes
**  + Also contains some metadata
**  + Rather than single TypeData object,
**    Constructor and Destructor are overwritten
**    and return NULL terminated array of TypeData
*/

#ifndef TypePlus_h
#define TypePlus_h

#include "Prelude+.h"

module Type;

data {
  const void* class_object;
  const char* class_name;
} TypeData;

/** Type_New(var self, const char* name, int num_classes, var* class_instances, const char** class_names); */
var Type_New(var self, va_list* args);
var Type_Delete(var self);

#define cast(X, T) Type_Cast(X, T, __func__)
var Type_Cast(var self, var type, const char* func);

#define Type_Implements(T, C) Type_Implements_Name(T, #C)
#define Type_Class(T, C) Type_Class_Name(T, #C)

bool Type_Implements_Name(var self, const char* class_name);
var Type_Class_Name(var self, const char* class_name);

const char* Type_AsStr(var self);

instance(Type, New) = { 0, Type_New, Type_Delete };
instance(Type, AsStr) = { Type_AsStr };


#endif