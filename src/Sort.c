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

var Sort = Cello(Sort, 
  Instance(Doc,
    Sort_Name, Sort_Brief, Sort_Description, 
    Sort_Examples, Sort_Methods));
  
static var Sort_Lt(var args) {
  return lt(get(args, $I(0)), get(args, $I(1)));
}
  
void sort(var self) {
  method(self, Sort, sort_with, $(Function, Sort_Lt));
}

void sort_with(var self, var func) {
  method(self, Sort, sort_with, func);
}


