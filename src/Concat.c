#include "Cello.h"

static const char* Concat_Name(void) {
  return "Concat";
}

static const char* Concat_Brief(void) {
  return "Join Object";
}

static const char* Concat_Description(void) {
  return
    "The `Concat` class is implemented by objects that can have other objects "
    "either _appended_ to their, on _concatenated_ to them. For example "
    "collections or strings.";
}

/* TODO */
static const char* Concat_Examples(void) {
  return "";
}

/* TODO */
static const char* Concat_Methods(void) {
  return "";
}

var Concat = Cello(Concat,
  Instance(Doc,
    Concat_Name, Concat_Brief, Concat_Description,
    Concat_Examples, Concat_Methods));

void append(var self, var obj) {
  return method(self, Concat, append, obj);
}

void concat(var self, var obj) {
  return method(self, Concat, concat, obj);
}