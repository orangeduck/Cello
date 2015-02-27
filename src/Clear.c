#include "Cello.h"

const char* Clear_Name(void) {
  return "Clear";
}

/* TODO */
const char* Clear_Brief(void) {
  return "";
}

/* TODO */
const char* Clear_Description(void) {
  return "";
}

/* TODO */
const char* Clear_Examples(void) {
  return "";
}

/* TODO */
const char* Clear_Methods(void) {
  return "";
}

var Clear = Cello(Clear, 
  Instance(Doc,
    Clear_Name, Clear_Brief, Clear_Description, 
    Clear_Examples, Clear_Methods));
  
void clear(var self) {
  method(self, Clear, clear);
}

