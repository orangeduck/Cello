#include "Cello.h"

static const char* Subtype_Name(void) {
  return "Subtype";
}

/* TODO */
static const char* Subtype_Brief(void) {
  return "";
}

/* TODO */
static const char* Subtype_Description(void) {
  return "";
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