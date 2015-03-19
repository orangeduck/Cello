#include "Cello.h"

static const char* Eq_Name(void) {
  return "Eq";
}

static const char* Eq_Brief(void) {
  return "Value Equality";
}

static const char* Eq_Description(void) {
  return
    "The `Eq` class is used to define equality between two object values. "
    "This class is important as it is used by many data structures to test "
    "for duplicate items or equality between two items."
    "\n\n"
    "By default, if passed two objects of the same type, the `Eq` class will "
    "simply compare the raw memory of both objects, using the `Size` "
    "class. If passed two objects of different types it will return `false`."
    "\n\n"
    "For objects that manage their own data this class may need to be "
    "overridden to ensure that objects of the same _value_ are still treated "
    "as equal. E.G. for string types."
    "\n\n"
    "This class to used to test for _value_ equality between objects, I.E. if "
    "they represent the same thing. For _object_ equality the `is` keyword can "
    "be used, which will return `true` only if two variables are pointing to "
    "the same object in memory.";
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
  Instance(Doc,
    Eq_Name, Eq_Brief, Eq_Description, Eq_Examples, Eq_Methods));

bool eq(var self, var obj) {
  
  if (implements(self, Eq)) {
    return method(self, Eq, eq, obj);
  }
  
  if (type_of(self) is type_of(obj) and implements_method(self, Size, size)) {
    return memcmp(self, obj, size(type_of(self))) is 0;
  }
  
  return self is obj;
}

bool neq(var self, var obj) { return not eq(self, obj); }

static const char* Ord_Name(void) {
  return "Ord";
}

static const char* Ord_Brief(void) {
  return "Comparable by Order";
}

static const char* Ord_Description(void) {
  return
    "The `Ord` class can be implemented to provide an ordering relationship "
    "between objects. In other words, to test if one object is _less than_ or "
    "_greater than_ another."
    "\n\n"
    "By default the `Ord` class will simply compare the raw memory of two "
    "objects using `memcmp` and the `Size` class, but for objects where "
    "ordering is more meaningful (such as Strings or Integers) this should "
    "be overridden.";
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
  Instance(Doc,
    Ord_Name, Ord_Brief, Ord_Description, Ord_Examples, Ord_Methods));

bool gt(var self, var obj) {

  if (implements_method(self, Ord, gt)) {
    return method(self, Ord, gt, obj);
  }
  
  if (type_of(self) is type_of(obj)) {
    return memcmp(self, obj, size(type_of(self))) > 0;
  }
  
  return self > obj;
  
}

bool lt(var self, var obj) {

  if (implements_method(self, Ord, lt)) {
    return method(self, Ord, lt, obj);
  }
  
  if (type_of(self) is type_of(obj)) {
    return memcmp(self, obj, size(type_of(self))) < 0;
  }
  
  return self < obj;
  
}

bool ge(var self, var obj) { return not lt(self, obj); }
bool le(var self, var obj) { return not gt(self, obj); }

