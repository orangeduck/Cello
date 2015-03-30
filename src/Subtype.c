#include "Cello.h"

static const char* Subtype_Name(void) {
  return "Subtype";
}

static const char* Subtype_Brief(void) {
  return "Contains elements of some Subtype";
}

static const char* Subtype_Description(void) {
  return
    "The `Subtype` class can be used to find the type of elements contained "
    "within another class. For example if an `Array` of a particular type is "
    "created, this class can be used to find what type that was.";
}

/* TODO */
static const char* Subtype_Examples(void) {
  return "";
}

/* TODO */
static const char* Subtype_Methods(void) {
  return "";
}

var Subtype = Cello(Subtype,
  Instance(Doc,
    Subtype_Name, Subtype_Brief, Subtype_Description, 
    Subtype_Examples, Subtype_Methods));

var subtype(var self) {
  return method(self, Subtype, subtype);
}

var key_subtype(var self) {
  return method(self, Subtype, key_subtype);
}

var val_subtype(var self) {
  return method(self, Subtype, val_subtype);
}
