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
  
  struct Eq* e = instance(self, Eq);
  if (e and e->eq) {
    return e->eq(self, obj);
  }
  
  if (type_of(self) is type_of(obj)) {
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

  struct Ord* o = instance(self, Ord);
  if (o and o->gt) {
    return o->gt(self, obj);
  }
  
  if (type_of(self) is type_of(obj)) {
    return memcmp(self, obj, size(type_of(self))) > 0;
  }
  
  return self > obj;
  
}

bool lt(var self, var obj) {

  struct Ord* o = instance(self, Ord);
  if (o and o->lt) {
    return o->lt(self, obj);
  }
  
  if (type_of(self) is type_of(obj)) {
    return memcmp(self, obj, size(type_of(self))) < 0;
  }
  
  return self < obj;
  
}

bool ge(var self, var obj) { return not lt(self, obj); }
bool le(var self, var obj) { return not gt(self, obj); }

int cmp(var self, var obj) {
  
  struct Ord* o = instance(self, Ord);
  if (o and o->cmp) {
    return o->cmp(self, obj);
  }
  
  if (type_of(self) is type_of(obj)) {
    return memcmp(self, obj, size(type_of(self)));
  }
  
  if (self < obj) { return -1; }
  if (self > obj) { return  1; }
  return 0;
}


const char* Sort_Name(void) {
  return "Sort";
}

const char* Sort_Brief(void) {
  return "Elements can be sorted";
}

const char* Sort_Description(void) {
  return
    "The `Sort` class can be implemented by types which can be sorted in some "
    "way such as `Array`. By default the sorting function uses the `lt` method "
    "to compare elements, but a custom function can also be provided.";
}

/* TODO */
const char* Sort_Examples(void) {
  return "";
}

/* TODO */
const char* Sort_Methods(void) {
  return "";
}

var Sort = Cello(Sort, 
  Instance(Doc,
    Sort_Name, Sort_Brief, Sort_Description, 
    Sort_Examples, Sort_Methods));
  
static var Sort_Lt(var args) {
  return (var)lt(get(args, $I(0)), get(args, $I(1)));
}
  
void sort(var self) {
  method(self, Sort, sort_with, $(Function, Sort_Lt));
}

void sort_with(var self, var func) {
  method(self, Sort, sort_with, func);
}

const char* Reverse_Name(void) {
  return "Reverse";
}

const char* Reverse_Brief(void) {
  return "Order can be reversed";
}

const char* Reverse_Description(void) {
  return
    "The `Reverse` class can be implemented by types which are reversible in "
    "some way such as `Array`.";
}

/* TODO */
const char* Reverse_Examples(void) {
  return "";
}

/* TODO */
const char* Reverse_Methods(void) {
  return "";
}

var Reverse = Cello(Reverse, 
  Instance(Doc,
    Reverse_Name, Reverse_Brief, Reverse_Description, 
    Reverse_Examples, Reverse_Methods));
  
void reverse(var self) {
  method(self, Reverse, reverse);
}

