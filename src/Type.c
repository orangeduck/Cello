#include "Cello/Type.h"

#include "Cello/Bool.h"
#include "Cello/Exception.h"
#include "Cello/Number.h"
#include "Cello/String.h"

#include <string.h>

var Type = type_data {
  type_begin(Type),
  type_entry(Type, New),
  type_entry(Type, AsStr),
  type_entry(Type, Show),
  type_end(Type),
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

var Type_New(var self, var_list vl) {
  
  const char* name = as_str(var_list_get(vl));
  int count = as_long(var_list_get(vl));
  var* ifaces = var_list_get(vl);
  const char** inames = var_list_get(vl);
  
  TypeData* newtype = malloc(sizeof(TypeData) * (count + 3));
  
  newtype[0].class_object = NULL;
  newtype[0].class_name = "__Type";
  
  newtype[1].class_object = (void*)name;
  newtype[1].class_name = "__Name";
  
  newtype[2].class_object = NULL;
  newtype[2].class_name = "__Parent"; 
  
  for(int i = 0; i < count; i++) {
    newtype[3+i].class_object = ifaces[i];
    newtype[3+i].class_name = inames[i];
  }
  
  return newtype;
}

var Type_Delete(var self) {
  return self;
}

size_t Type_Size(void) {
  return 0;
}

const char* Type_AsStr(var self) {
  return type_class(self, __Name);
}

const char* Type_Name(var self) {

  TypeData* t = self;

  while(t->class_name) {
    if (strcmp(t->class_name, "__Name") == 0) {
      return t->class_object;
    }
    t++;
  }
  
  return throw(ClassError,
    "Cannot find Class '__Name' for object '%p' :: "
    "Was is correctly constructed? :: "
    "Does the data start with a 'type' entry? ::"
    "Was `type_begin` used?", self);
  
}

var Type_Parent(var self) {

  TypeData* t = self;

  while(t->class_name) {
    if (strcmp(t->class_name, "__Parent") == 0) {
      return t->class_object;
    }
    t++;
  }
  
  return throw(ClassError,
    "Cannot find Class '__Parent' for object '%p' :: "
    "Was is correctly constructed? :: "
    "Does the data start with a 'type' entry? ::"
    "Was `type_begin` used?", self);
  
}

var Type_Implements(var self, const char* class_name, const char* func, const char* file, int line) {
  
  TypeData* t = self;
  
  if (t[0].class_object != NULL) {
    return throw(ClassError,
      "Function '%s' at '%s:%i' :: "
      "Object '%p' is not a type! :: "
      "It does not start with a NULL pointer",
      $(String, (char*)func), $(String, (char*)file), $(Int, line), self);
  }
  
  while(t->class_name) {
    if (strcmp(t->class_name, class_name) == 0) { return True; }
    t++;
  }
  
  var parent = Type_Parent(self);
  
  return bool_var(parent isnt NULL and Type_Implements(parent, class_name, func, file, line));
  
}

var Type_Implements_Method(var self, int offset, const char* class_name, const char* func, const char* file, int line) {
  
  if (not Type_Implements(self, class_name, func, file, line)) {
    
    return Type_Implements_Method(Type_Parent(self), offset, class_name, func, file, line);
    
  } else {
  
    intptr_t* func_address = Type_Class(self, class_name, func, file, line) + offset;
    return bool_var(*func_address);
  
  }
  
}

var Type_Class(var self, const char* class_name, const char* func, const char* file, int line) {
  TypeData* t = self;
  
  if (t[0].class_object != NULL) {
    return throw(ClassError,
      "Function '%s' at '%s:%i' :: "
      "Object '%p' is not a type! :: "
      "It does not start with a NULL pointer",
      $(String, (char*)func), $(String, (char*)file), $(Int, line), self);
  }
  
  while(t->class_name) {
    if (strcmp(t->class_name, class_name) == 0) {
      return (var)t->class_object;
    }
    t++;
  }
  
  var parent = Type_Parent(self);
  
  if (parent isnt NULL and Type_Implements(parent, class_name, func, file, line)) {
    return Type_Class(parent, class_name, func, file, line);
  }
  
  return throw(ClassError,
    "Function '%s' at '%s:%i' :: "
    "Type '%s' does not implement class '%s'",
    $(String, (char*)func), $(String, (char*)file), $(Int, line), 
    $(String, (char*)Type_Name(self)), $(String, (char*)class_name));
      
}

var Type_Class_Method(var self, int offset, const char* class_name, const char* method_name, const char* func, const char* file, int line) {
  
  if (Type_Implements_Method(self, offset, class_name, func, file, line)) {
    return Type_Class(self, class_name, func, file, line);
  }
  
  var parent = Type_Parent(self);
  
  if (Type_Implements_Method(parent, offset, class_name, func, file, line)) {
    return Type_Class(parent, class_name, func, file, line);
  }
  
  return throw(ClassError,
    "Function '%s' at '%s:%i' :: "
    "Type '%s' implements class '%s' but not the specific method '%s' required",
    $(String, (char*)func), $(String, (char*)file), $(Int, line), 
    self, $(String, (char*)class_name), $(String, (char*)method_name));
  
}

int Type_Show(var self, var output, int pos) {
  return print_to(output, pos, "%s", self);
}

void Type_Inherit(var self, var parent) {
  
  TypeData* t = self;

  while(t->class_name) {
    if (strcmp(t->class_name, "__Parent") == 0) {
      t->class_object = parent;
      return;
    }
    t++;
  }
  
  throw(ClassError,
    "Cannot find Class '__Parent' for object '%p' :: "
    "Was is correctly constructed? :: "
    "Does the data start with a 'type' entry? ::"
    "Was `type_begin` used?", self);
  
}


