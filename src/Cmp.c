#include "Cello.h"

static const char* Eq_Name(void) {
  return "Eq";
}

/* TODO */
static const char* Eq_Brief(void) {
  return "";
}

/* TODO */
static const char* Eq_Description(void) {
  return "";
}

/* TODO */
static const char* Eq_Examples(void) {
  return "";
}

/* TODO */
static const char* Eq_Methods(void) {
  return "";
}

var Eq = Cello(Eq,
  Member(Doc,
    Eq_Name, Eq_Brief, Eq_Description, Eq_Examples, Eq_Methods));

var eq(var self, var obj) {

  if (implements(self, Eq)) {
    return method(self, Eq, eq, obj);
  }
  
  return bool_var(self is obj);
}

var neq(var self, var obj) { return bool_var(not eq(self, obj)); }

static const char* Ord_Name(void) {
  return "Ord";
}

/* TODO */
static const char* Ord_Brief(void) {
  return "";
}

/* TODO */
static const char* Ord_Description(void) {
  return "";
}

/* TODO */
static const char* Ord_Examples(void) {
  return "";
}

/* TODO */
static const char* Ord_Methods(void) {
  return "";
}

var Ord = Cello(Ord,
  Member(Doc,
    Ord_Name, Ord_Brief, Ord_Description, Ord_Examples, Ord_Methods));

var gt(var self, var obj) { return method(self, Ord, gt, obj); }
var lt(var self, var obj) { return method(self, Ord, lt, obj); }

var ge(var self, var obj) { return bool_var(not lt(self, obj)); }
var le(var self, var obj) { return bool_var(not gt(self, obj)); }
