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

var Type_New(var self, var_list vl);
var Type_Delete(var self);
size_t Type_Size(void);

#define cast(X, T) Type_Cast(X, T, __func__, __FILE__, __LINE__)
var Type_Cast(var self, var type, const char* func, const char* file, int line);

#define type_implements(T, C) Type_Implements_Name(T, #C, __func__, __FILE__, __LINE__)
var Type_Implements_Name(var self, const char* class_name, const char* func, const char* file, int line);

#define type_implements_method(T, C, M) Type_Implements_Method_Name(T, offsetof(C, M), #C, __func__, __FILE__, __LINE__)
var Type_Implements_Method_Name(var self, int offset, const char* class_name, const char* func, const char* file, int line);

#define type_class(T, C) Type_Class_Name(T, #C, __func__, __FILE__, __LINE__)
var Type_Class_Name(var self, const char* class_name, const char* func, const char* file, int line);

#define type_class_method(T, C, M, ...) ((C*)Type_Class_Name_Method(T, ((C*)type_class(T, C))->M, #C, #M, __func__, __FILE__, __LINE__))->M(__VA_ARGS__)
var Type_Class_Name_Method(var self, var method, const char* class_name, const char* method_name, const char* func, const char* file, int line);

const char* Type_AsStr(var self);
int Type_Show(var self, var output, int pos);

instance(Type, New) = { Type_New, Type_Delete, Type_Size };
instance(Type, AsStr) = { Type_AsStr };
instance(Type, Show) = { Type_Show, NULL };  

#endif
