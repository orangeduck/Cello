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

static struct DocMethod* Eq_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "eq", 
      "bool eq(var self, var obj);",
      "Returns true if the object `self` is equal to the object `obj`."
    }, {
      "neq", 
      "bool neq(var self, var obj);",
      "Returns false if the object `self` is equal to the object `obj`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

/* TODO: Examples */

var Eq = Cello(Eq,
  Instance(Doc,
    Eq_Name, Eq_Brief, Eq_Description, NULL, Eq_Methods));

bool eq(var self, var obj) {
  
  struct Eq* e = instance(self, Eq);
  if (e and e->eq) {
    return e->eq(self, obj);
  }
  
  size_t s = size(type_of(self));
  if (type_of(self) is type_of(obj) and s) {
    return memcmp(self, obj, s) is 0;
  }
  
  throw(TypeError,
    "Cannot compare type %s to type %s", type_of(obj), type_of(self));
  
  return false;
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

static struct DocMethod* Ord_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "gt", 
      "bool gt(var self, var obj);",
      "Returns true if the object `self` is greater than the object `obj`."
    }, {
      "lt", 
      "bool lt(var self, var obj);",
      "Returns false if the object `self` is less than the object `obj`."
    }, {
      "ge", 
      "bool ge(var self, var obj);",
      "Returns false if the object `self` is greater than or equal to the "
      "object `obj`."
    }, {
      "le", 
      "bool le(var self, var obj);",
      "Returns false if the object `self` is less than or equal to the "
      "object `obj`."
    }, {
      "cmp", 
      "int cmp(var self, var obj);",
      "The return value of `cmp` is `< 0` if `self` is less than `obj`, `> 0` "
      "if `self` is greater than `obj` and `0` if they are equal."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

/* TODO: Examples */

var Ord = Cello(Ord,
  Instance(Doc,
    Ord_Name, Ord_Brief, Ord_Description, NULL, Ord_Methods));

bool gt(var self, var obj) {

  struct Ord* o = instance(self, Ord);
  if (o and o->gt) {
    return o->gt(self, obj);
  }
  
  size_t s = size(type_of(self));
  if (type_of(self) is type_of(obj) and s) {
    return memcmp(self, obj, s) > 0;
  }
  
  throw(TypeError,
    "Cannot compare type %s to type %s", type_of(obj), type_of(self));
  
  return false;
  
}

bool lt(var self, var obj) {

  struct Ord* o = instance(self, Ord);
  if (o and o->lt) {
    return o->lt(self, obj);
  }
  
  size_t s = size(type_of(self));
  if (type_of(self) is type_of(obj) and s) {
    return memcmp(self, obj, s) < 0;
  }
  
  throw(TypeError,
    "Cannot compare type %s to type %s", type_of(obj), type_of(self));
  
  return false;  
}

bool ge(var self, var obj) { return not lt(self, obj); }
bool le(var self, var obj) { return not gt(self, obj); }

int cmp(var self, var obj) {
  
  struct Ord* o = instance(self, Ord);
  if (o and o->cmp) {
    return o->cmp(self, obj);
  }
  
  size_t s = size(type_of(self));
  if (type_of(self) is type_of(obj) and s) {
    return memcmp(self, obj, s);
  }
  
  throw(TypeError,
    "Cannot compare type %s to type %s", type_of(obj), type_of(self));
  
  return false;
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

static struct DocMethod* Sort_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "sort", 
      "void sort(var self);",
      "Sorts the object `self`."
    }, {
      "sort_with", 
      "void sort_with(var self, var func);",
      "Sorts the object `self` using the function `func`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

/* TODO: Examples */

var Sort = Cello(Sort, 
  Instance(Doc,
    Sort_Name, Sort_Brief, Sort_Description, 
    NULL, Sort_Methods));
  
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

static struct DocMethod* Reverse_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "reverse", 
      "void reverse(var self);",
      "The order of the object `self` is reversed."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

/* TODO: Examples */

var Reverse = Cello(Reverse, 
  Instance(Doc,
    Reverse_Name, Reverse_Brief, Reverse_Description, 
    NULL, Reverse_Methods));
  
void reverse(var self) {
  method(self, Reverse, reverse);
}

