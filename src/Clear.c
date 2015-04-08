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

static struct DocMethod* Clear_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "clear", 
      "void clear(var self);",
      "Clear the object `self`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

/* TODO: Examples, Methods */

var Clear = Cello(Clear, 
  Instance(Doc,
    Clear_Name, Clear_Brief, Clear_Description, 
    NULL, Clear_Methods));
  
void clear(var self) {
  method(self, Clear, clear);
}

