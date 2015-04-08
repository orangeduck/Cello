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

static struct DocMethod* Get_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "get", 
      "var get(var self, var key);",
      "Get the value at a given `key` for object `self`."
    }, {
      "set", 
      "void set(var self, var key, var val);",
      "Set the value at a given `key` for object `self`."
    }, {
      "mem", 
      "bool mem(var self, var key);",
      "Returns true if `key` is a member of the object `self`."
    }, {
      "rem", 
      "void rem(var self, var key);",
      "Removes the `key` from object `self`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

/* TODO: Examples */

var Get = Cello(Get,
  Instance(Doc,
    Get_Name, Get_Brief, Get_Description, NULL, Get_Methods));

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

