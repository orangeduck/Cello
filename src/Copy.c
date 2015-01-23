#include "Cello.h"

/* TODO */
static const char* Copy_Name(void) {
  return "Copy";
}

/* TODO */
static const char* Copy_Brief(void) {
  return "";
}

/* TODO */
static const char* Copy_Description(void) {
  return "";
}

/* TODO */
static const char* Copy_Examples(void) {
  return "";
}

var Copy = type(Copy,
  type_class(Document, Copy_Name, Copy_Brief, Copy_Description, Copy_Examples));


var copy(var self) {
  return method(self, Copy, copy);
}
