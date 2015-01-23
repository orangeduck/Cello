#include "Cello.h"

/* TODO */
static const char* Undefined_Name(void) {
  return "Undefined";
}

/* TODO */
static const char* Undefined_Brief(void) {
  return "";
}

/* TODO */
static const char* Undefined_Description(void) {
  return "";
}

/* TODO */
static const char* Undefined_Examples(void) {
  return "";
}

/* TODO */
static const char* Undefined_Methods(void) {
  return "";
}

var Undefined = typedecl(Undefined,
  typeclass(Doc,
    Undefined_Name,        Undefined_Brief,
    Undefined_Description, Undefined_Examples, Undefined_Methods));