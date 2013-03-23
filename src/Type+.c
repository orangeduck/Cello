#include "Bool+.h"

#include "Type+.h"
#include "Exception+.h"

#include <string.h>

var Type = methods {
  methods_begin(Type),
  method(Type, New),
  method(Type, AsStr),
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
      func, file, line, as_str(type_of(obj)), as_str(t));
  }
  
}

var Type_New(var self, va_list* args) {
  
  const char* name = va_arg(*args, const char*);
  int count = va_arg(*args, int);
  var* ifaces = va_arg(*args, var*);
  const char** inames = va_arg(*args, const char**);
  
  TypeData* newtype = malloc(sizeof(TypeData) * (count + 2));
  
  newtype[0].class_object = Type;
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

local void Type_Print(var self) {
  TypeData* t = self;
  printf("(Type: '%p')\n", t);
  while(t->class_name) {
    if (strcmp(t->class_name, "__Name") == 0) {
      printf("(%s, %p '%s')\n", t->class_name, t->class_object, (const char*)t->class_object);
    } else {
      printf("(%s, %p)\n", t->class_name, t->class_object);
    }
    t++;
  }
}

var Type_Implements_Name(var self, const char* class_name) {
  TypeData* t = self;
  while(t->class_name) {
    if (strcmp(t->class_name, class_name) == 0) return True;
    t++;
  }
  return False;
}

var Type_Class_Name(var self, const char* class_name, const char* func, const char* file, int line) {
  TypeData* t = self;
  
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
  
    throw(ClassError,
      "Function '%s' at '%s:%i' :: "
      "Cannot find class '__Name' for object '%p' :: "
      "Was is correctly Constructed? :: "
      "Does it start with a 'type' entry?",
      func, file, line, self);
    return Undefined;
      
  } else {
  
    throw(ClassError,
      "Function '%s' at '%s:%i' :: "
      "Type '%s' does not implement class '%s'",
      func, file, line, type_name, class_name);
    return Undefined;
    
  }
}
