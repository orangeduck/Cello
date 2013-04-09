/*
** == Type ==
**
**  Data object for types of data objects.
**  Contains lists of implemented classes
**  Also contains some metadata
**  Rather than single TypeData object,
**  Constructor and Destructor are overwritten
**  and return NULL terminated array of TypeData
*/

#ifndef CelloType_h
#define CelloType_h

#include "Prelude.h"
#include "Format.h"

global var Type;

data {
  void* class_object;
  const char* class_name;
} TypeData;

/** Type_New(var self, const char* name, int num_classes, var* class_instances, const char** class_names); */
var Type_New(var self, va_list* args);
var Type_Delete(var self);

#define cast(X, T) Type_Cast(X, T, __func__, __FILE__, __LINE__)
var Type_Cast(var self, var type, const char* func, const char* file, int line);

#define type_implements(T, C) Type_Implements_Name(T, #C, __func__, __FILE__, __LINE__)
var Type_Implements_Name(var self, const char* class_name, const char* func, const char* file, int line);

#define type_class(T, C) Type_Class_Name(T, #C, __func__, __FILE__, __LINE__)
var Type_Class_Name(var self, const char* class_name, const char* func, const char* file, int line);

const char* Type_AsStr(var self);
int Type_Show(var self, var output, int pos);

instance(Type, New) = { 0, Type_New, Type_Delete };
instance(Type, AsStr) = { Type_AsStr };
instance(Type, Show) = { Type_Show, NULL };  

#endif
