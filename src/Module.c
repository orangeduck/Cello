#include "Cello/Module.h"

#include "Cello/String.h"
#include "Cello/List.h"

#include <string.h>

var Module = type_data {
  type_begin(Module),
  type_entry(Module, New),
  type_entry(Module, Copy),
  type_entry(Module, Assign),
  type_entry(Module, With),
  type_entry(Module, Dict),
  type_entry(Module, Call),
  type_end(Module),
};

var Module_New(var self, var_list vl) {
  ModuleData* md = cast(self, Module);
  
  const char* filename = as_str(var_list_get(vl));
  
  md->name = malloc(strlen(filename) + 1);
  strcpy(md->name, filename);
  
  enter_with(self);
  
  return self;
}

var Module_Delete(var self) {
  ModuleData* md = cast(self, Module);
  exit_with(self);
  free(md->name);
  return self;
}

size_t Module_Size(void) {
  return sizeof(ModuleData);
}

var Module_Copy(var self) {
  ModuleData* md = cast(self, Module);
  return new(Module, md->name);
}

void Module_Assign(var self, var obj) {
  
  exit_with(self);
  
  ModuleData* md = cast(self, Module);
  ModuleData* od = cast(obj, Module);
  
  md->name = realloc(md->name, strlen(od->name) + 1);
  strcpy(md->name, od->name);
  
  enter_with(self);
  
}

void Module_Enter(var self) {
  ModuleData* md = cast(self, Module);
  
  if (md->lib isnt NULL) { Module_Exit(self); }
  
#ifdef _WIN32
  md->lib = LoadLibrary(md->name);
#else
  md->lib = dlopen(md->name, RTLD_LAZY | RTLD_NOLOAD);
#endif
  
  if (md->lib is NULL) {
#ifdef _WIN32
    var error = $(String, md->name);
#else
    var error = $(String, dlerror());
#endif
    throw(ResourceError, "Could not load dynamic library '%s'!", error);
  }
  
}

void Module_Exit(var self) {
  ModuleData* md = cast(self, Module);
  
  if (md->lib is NULL) { return; } 
  
#ifdef _WIN32
  FreeLibrary(md->lib);
#else
  dlclose(md->lib);
#endif
  
  md->lib = NULL;
  
}

var Module_Get(var self, var k) {
  
  ModuleData* md = cast(self, Module);

#ifdef _WIN32
  var func_ptr = GetProcAddress(md->lib, as_str(k));
#else
  var func_ptr = dlsym(md->lib, as_str(k));
#endif

  if (func_ptr is NULL) {
    return throw(KeyError, "No Function '%s' found in module '%s'", k, $(String, md->name));
  } else {
    return func_ptr;
  }
  
}

var Module_Call(var self, var args) {
  
  ModuleData* md = cast(self, Module);

  var func = cast(pop(args), String);
  var rest = cast(args, List);
  
  var func_ptr = get(self, func);
  
  return call($(Function, func_ptr), rest);
  
}