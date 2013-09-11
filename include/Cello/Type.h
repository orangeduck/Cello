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

#define type_implements(T, C) Type_Implements(T, #C, __func__, __FILE__, __LINE__)
#define type_implements_method(T, C, M) Type_Implements_Method(T, offsetof(C, M), #C, __func__, __FILE__, __LINE__)

#define type_class(T, C) Type_Class(T, #C, __func__, __FILE__, __LINE__)
#define type_class_method(T, C, M, ...) ((C*)Type_Class_Method(T, offsetof(C, M), #C, #M, __func__, __FILE__, __LINE__))->M(__VA_ARGS__)

var Type_Cast(var self, var type, const char* func, const char* file, int line);

var Type_Implements(var self, const char* class_name, const char* func, const char* file, int line);
var Type_Implements_Method(var self, int offset, const char* class_name, const char* func, const char* file, int line);

var Type_Class(var self, const char* class_name, const char* func, const char* file, int line);
var Type_Class_Method(var self, int offset, const char* class_name, const char* method_name, const char* func, const char* file, int line);

const char* Type_Name(var self);
var Type_Parent(var self);
void Type_Inherit(var self, var parent); 

const char* Type_AsStr(var self);
int Type_Show(var self, var output, int pos);

instance(Type, New) = { Type_New, Type_Delete, Type_Size };
instance(Type, AsStr) = { Type_AsStr };
instance(Type, Show) = { Type_Show, NULL };  

#endif
