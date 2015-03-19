#include "Cello.h"

const char* Clear_Name(void) {
  return "Clear";
}

const char* Clear_Brief(void) {
  return "Object can be Emptied";
}

const char* Clear_Description(void) {
  return
    "The `Clear` class can be implemented when it makes sense to _clear_ an "
    "object, such as to remove all of the items from an `Array` or `Table`.";
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

