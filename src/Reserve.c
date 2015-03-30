#include "Cello.h"

const char* Reserve_Name(void) {
  return "Reserve";
}

const char* Reserve_Brief(void) {
  return "Resources can be Preallocated";
}

const char* Reserve_Description(void) {
  return
    "The `Reserve` class can be implemented by objects which allow for some "
    "resource or other to be preallocated or reserved. For example this class "
    "is implemented by `Array` and `Table` to preallocate memory space if it "
    "is known that many items are going to be added at a later date.";
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

