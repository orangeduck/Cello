#include "Cello.h"

static const char* Len_Name(void) {
  return "Len";
}

/* TODO */
static const char* Len_Brief(void) {
  return "";
}

/* TODO */
static const char* Len_Description(void) {
  return "";
}

/* TODO */
static const char* Len_Examples(void) {
  return "";
}

/* TODO */
static const char* Len_Methods(void) {
  return "";
}

var Len = Cello(Len,
  Instance(Doc,
    Len_Name, Len_Brief, Len_Description, Len_Examples, Len_Methods));

size_t len(var self) {
  return method(self, Len, len);
}

bool empty(var self) {
  return len(self) is 0;
}
