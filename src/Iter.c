#include "Cello.h"

static const char* Iter_Name(void) {
  return "Iter";
}

/* TODO */
static const char* Iter_Brief(void) {
  return "";
}

/* TODO */
static const char* Iter_Description(void) {
  return "";
}

/* TODO */
static const char* Iter_Examples(void) {
  return "";
}

/* TODO */
static const char* Iter_Methods(void) {
  return "";
}

var Iter = Cello(Iter,
  Instance(Doc,
    Iter_Name, Iter_Brief, Iter_Description, Iter_Examples, Iter_Methods));

var iter_init(var self) {
  return method(self, Iter, iter_init);
}

var iter_next(var self, var curr) {
  return method(self, Iter, iter_next, curr);
}

var iter_last(var self) {
  return method(self, Iter, iter_last);
}

var iter_prev(var self, var curr) {
  return method(self, Iter, iter_prev, curr);
}

