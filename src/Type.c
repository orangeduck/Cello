#include "Cello/Type.h"

#include "Cello/Bool.h"
#include "Cello/Exception.h"
#include "Cello/Number.h"
#include "Cello/String.h"

#include <string.h>

var Type = methods {
  methods_begin(Type),
  method(Type, New),
  method(Type, AsStr),
  method(Type, Show),
  methods_end(Type),
};

var Type_Cast(var obj, var t, const char* func, const char* file, int line) {
  
  if (type_of(obj) is t) {
    return obj;
  } else {
    return throw(TypeError,
      "Argument to function '%s' at '%s:%i' :: "
      "Got Type '%s' :: "
      "Expected Type '%s'", 
      $(String, (char*)func), $(String, (char*)file), $(Int, line), type_of(obj), t);
  }
  
}

var Type_New(var self, va_list* args) {
  
  const char* name = va_arg(*args, const char*);
  int count = va_arg(*args, int);
  var* ifaces = va_arg(*args, var*);
  const char** inames = va_arg(*args, const char**);
  
  TypeData* newtype = malloc(sizeof(TypeData) * (count + 2));
  
  newtype[0].class_object = NULL;
  newtype[0].class_name = "__Type";
  
  newtype[1].class_object = (void*)name;
  newtype[1].class_name = "__Name";
  
  for(int i = 0; i < count; i++) {
    newtype[2+i].class_object = ifaces[i];
    newtype[2+i].class_name = inames[i];
  }
  
  return newtype;
}

var Type_Delete(var self) {
  return self;
}

const char* Type_AsStr(var self) {
  return type_class(self, __Name);
}

var Type_Implements_Name(var self, const char* class_name, const char* func, const char* file, int line) {
  TypeData* t = self;
  
  if (t[0].class_object != NULL) {
    return throw(ClassError,
      "Function '%s' at '%s:%i' :: "
      "Object '%p' is not a type! :: "
      "It does not start with a NULL pointer",
      $(String, (char*)func), $(String, (char*)file), $(Int, line), self);
  }
  
  while(t->class_name) {
    if (strcmp(t->class_name, class_name) == 0) return True;
    t++;
  }
  
  return False;
}

var Type_Class_Name(var self, const char* class_name, const char* func, const char* file, int line) {
  TypeData* t = self;
  
  if (t[0].class_object != NULL) {
    return throw(ClassError,
      "Function '%s' at '%s:%i' :: "
      "Object '%p' is not a type! :: "
      "It does not start with a NULL pointer",
      $(String, (char*)func), $(String, (char*)file), $(Int, line), self);
  }
  
  const char* type_name = NULL;
  
  while(t->class_name) {
    if (strcmp(t->class_name, "__Name") == 0) {
      type_name = t->class_object;
    }
    if (strcmp(t->class_name, class_name) == 0) {
      return (var)t->class_object;
    }
    t++;
  }
  
  if (type_name == NULL) {
  
    return throw(ClassError,
      "Function '%s' at '%s:%i' :: "
      "Cannot find class '__Name' for object '%p' :: "
      "Was is correctly Constructed? :: "
      "Does it start with a 'type' entry?",
      $(String, (char*)func), $(String, (char*)file), $(Int, line), self);
      
  } else {
  
    return throw(ClassError,
      "Function '%s' at '%s:%i' :: "
      "Type '%s' does not implement class '%s'",
      $(String, (char*)func), $(String, (char*)file), $(Int, line), 
      $(String, type_name), $(String, class_name));
    
  }
}

int Type_Show(var self, var output, int pos) {
  return print_to(output, pos, "%s", self);
}


