#include "Cello.h"

const char* Reserve_Name(void) {
  return "Reserve";
}

/* TODO */
const char* Reserve_Brief(void) {
  return "";
}

/* TODO */
const char* Reserve_Description(void) {
  return "";
}

/* TODO */
const char* Reserve_Examples(void) {
  return "";
}

/* TODO */
const char* Reserve_Methods(void) {
  return "";
}

var Reserve = Cello(Reserve, 
  Instance(Doc,
    Reserve_Name, Reserve_Brief, Reserve_Description, 
    Reserve_Examples, Reserve_Methods));
  
void reserve(var self, var amount) {
  method(self, Reserve, reserve, amount);
}

