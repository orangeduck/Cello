#include "Cello.h"

static const char* Get_Name(void) {
  return "Get";
}

static const char* Get_Brief(void) {
  return "Gettable or Settable";
}

static const char* Get_Description(void) {
  return
    "The `Get` class provides a method to _get_ or _set_ certain properties "
    "of an object using keys and value. Typically it is implemented by "
    "data lookup structures such as `Table` or `Map` but it is also used "
    "more generally such as using idices to look up items in `Array`, or "
    "as thread local storage for the `Thread` object.";
}

/* TODO */
static const char* Get_Examples(void) {
  return "";
}

/* TODO */
static const char* Get_Methods(void) {
  return "";
}

var Get = Cello(Get,
  Instance(Doc,
    Get_Name, Get_Brief, Get_Description, Get_Examples, Get_Methods));

var get(var self, var key) {
  return method(self, Get, get, key);
}

void set(var self, var key, var val) {
  method(self, Get, set, key, val);
}

bool mem(var self, var key) {
  return method(self, Get, mem, key);
}

void rem(var self, var key) {
  method(self, Get, rem, key);
}

