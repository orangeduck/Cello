#include "Cello.h"

static const char* Traverse_Name(void) {
  return "Traverse";
}

/* TODO */
static const char* Traverse_Brief(void) {
  return "";
}

/* TODO */
static const char* Traverse_Description(void) {
  return "";
}

/* TODO */
static const char* Traverse_Examples(void) {
  return "";
}

/* TODO */
static const char* Traverse_Methods(void) {
  return "";
}

var Traverse = Cello(Traverse, Instance(Doc, 
  Traverse_Name, Traverse_Brief, Traverse_Description, 
  Traverse_Examples, Traverse_Methods));
  
void traverse(var self, var func) {
  method(self, Traverse, traverse, func);
}
