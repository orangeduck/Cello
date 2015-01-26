#include "Cello.h"

const char* Sort_Name(void) {
  return "Sort";
}

/* TODO */
const char* Sort_Brief(void) {
  return "";
}

/* TODO */
const char* Sort_Description(void) {
  return "";
}

/* TODO */
const char* Sort_Examples(void) {
  return "";
}

/* TODO */
const char* Sort_Methods(void) {
  return "";
}

var Sort = typedecl(Sort, 
  typeclass(Doc,
    Sort_Name, Sort_Brief, Sort_Description, 
    Sort_Examples, Sort_Methods));
  
void sort(var self) {
  method(self, Sort, sort);
}


