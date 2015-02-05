#include "Cello.h"

const char* Reverse_Name(void) {
  return "Reverse";
}

/* TODO */
const char* Reverse_Brief(void) {
  return "";
}

/* TODO */
const char* Reverse_Description(void) {
  return "";
}

/* TODO */
const char* Reverse_Examples(void) {
  return "";
}

/* TODO */
const char* Reverse_Methods(void) {
  return "";
}

var Reverse = Cello(Reverse, 
  Member(Doc,
    Reverse_Name, Reverse_Brief, Reverse_Description, 
    Reverse_Examples, Reverse_Methods));
  
void reverse(var self) {
  method(self, Reverse, reverse);
}

