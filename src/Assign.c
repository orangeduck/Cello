#include "Cello.h"

/* TODO */
static const char* Assign_Name(void) {
  return "Assign";
}

/* TODO */
static const char* Assign_Brief(void) {
  return "";
}

/* TODO */
static const char* Assign_Description(void) {
  return "";
}

/* TODO */
static const char* Assign_Examples(void) {
  return "";
}

/* TODO */
static const char* Assign_Methods(void) {
  return "";
}

var Assign = Cello(Assign,
  Instance(Doc,
    Assign_Name, Assign_Brief, Assign_Description,
    Assign_Examples, Assign_Methods));

var assign(var self, var obj) {
  struct Assign* a = instance(self, Assign);
  if (a and a->assign) {
    return a->assign(self, obj);
  } else {
    memcpy(self, obj, size(type_of(self)));
    return self;
  }
}
