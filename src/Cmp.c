#include "Cello.h"

static const char* Cmp_Name(void) {
  return "Cmp";
}

static const char* Cmp_Brief(void) {
  return "Comparison";
}

static const char* Cmp_Description(void) {
  return
    "The `Cmp` class is used to define comparison between two object values. "
    "This class is important as it is used by many data structures to test "
    "equality or ordering of objects."
    "\n\n"
    "By default, if passed two objects of the same type, the `Cmp` class will "
    "simply compare the raw memory of both objects, using the `Size` "
    "class."
    "\n\n"
    "To implement this class a `cmp` function must be provided which returns "
    "`< 0` if the first object is _less than_ the second, `> 0` if the first "
    "object is _greater than_ the second, and `0` if they are _equal_. "
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

static const char* Cmp_Definition(void) {
  return
    "struct Cmp {\n"
    "  int (*cmp)(var, var);\n"
    "};\n";
}

static struct Example* Cmp_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage 1",
      "show($I( eq($I(1), $I( 1)))); /* 1 */\n"
      "show($I(neq($I(2), $I(20)))); /* 1 */\n"
      "show($I(neq($S(\"Hello\"), $S(\"Hello\")))); /* 0 */\n"
      "show($I( eq($S(\"Hello\"), $S(\"There\")))); /* 0 */\n"
      "\n"
      "var a = $I(1); var b = $I(1);\n"
      "\n"
      "show($I(eq(a, b))); /* 1 */\n"
      "show($I(a is b));   /* 0 */\n"
      "show($I(a isnt b)); /* 1 */\n"
    }, {
      "Usage 2",
      "show($I(gt($I(15), $I(3 )))); /* 1 */\n"
      "show($I(lt($I(70), $I(81)))); /* 1 */\n"
      "show($I(lt($I(71), $I(71)))); /* 0 */\n"
      "show($I(ge($I(78), $I(71)))); /* 1 */\n"
      "show($I(gt($I(32), $I(32)))); /* 0 */\n"
      "show($I(le($I(21), $I(32)))); /* 1 */\n"
      "\n"
      "show($I(cmp($I(20), $I(20)))); /*  0 */\n"
      "show($I(cmp($I(21), $I(20)))); /*  1 */\n"
      "show($I(cmp($I(20), $I(21)))); /* -1 */\n"
    }, {NULL, NULL}
  };
  
  return examples;
}

static struct Method* Cmp_Methods(void) {
  
  static struct Method methods[] = {
    {
      "cmp", 
      "int cmp(var self, var obj);",
      "The return value of `cmp` is `< 0` if `self` is less than `obj`, `> 0` "
      "if `self` is greater than `obj` and `0` if they are equal."
    }, {
      "eq", 
      "bool eq(var self, var obj);",
      "Returns true if the object `self` is equal to the object `obj`."
    }, {
      "neq", 
      "bool neq(var self, var obj);",
      "Returns false if the object `self` is equal to the object `obj`."
    }, {
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
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Cmp = Cello(Cmp,
  Instance(Doc,
    Cmp_Name,       Cmp_Brief,    Cmp_Description, 
    Cmp_Definition, Cmp_Examples, Cmp_Methods));

int cmp(var self, var obj) {
  
  struct Cmp* c = instance(self, Cmp);
  if (c and c->cmp) { return c->cmp(self, obj); }
  
  size_t s = size(type_of(self));
  if (type_of(self) is type_of(obj) and s) { return memcmp(self, obj, s); }
  
  throw(TypeError,
    "Cannot compare type %s to type %s", type_of(obj), type_of(self));
  
  return 0;
}
    
bool  eq(var self, var obj) { return cmp(self, obj) is 0; }
bool neq(var self, var obj) { return not eq(self, obj); }
bool  gt(var self, var obj) { return cmp(self, obj) > 0; }
bool  lt(var self, var obj) { return cmp(self, obj) < 0; }
bool  ge(var self, var obj) { return not lt(self, obj); }
bool  le(var self, var obj) { return not gt(self, obj); }

static const char* Sort_Name(void) {
  return "Sort";
}

static const char* Sort_Brief(void) {
  return "Sortable";
}

static const char* Sort_Description(void) {
  return
    "The `Sort` class can be implemented by types which can be sorted in some "
    "way such as `Array`. By default the sorting function uses the `lt` method "
    "to compare elements, but a custom function can also be provided.";
}

static const char* Sort_Definition(void) {
  return
    "struct Sort {\n"
    "  void (*sort_by)(var,bool(*f)(var,var));\n"
    "};";
}

static struct Example* Sort_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var x = new(Array, Float, \n"
      "  $F(5.2), $F(7.1), $F(2.2));\n"
      "\n"
      "show(x); /* <'Array' At 0x00414603 [5.2, 7.1, 2.2]> */\n"
      "sort(x);\n"
      "show(x); /* <'Array' At 0x00414603 [2.2, 5.2, 7.1]> */\n"
    }, {NULL, NULL}
  };
  
  return examples;
}

static struct Method* Sort_Methods(void) {
  
  static struct Method methods[] = {
    {
      "sort", 
      "void sort(var self);",
      "Sorts the object `self`."
    }, {
      "sort_by", 
      "void sort_by(var self, bool(*f)(var,var));",
      "Sorts the object `self` using the function `f`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Sort = Cello(Sort, 
  Instance(Doc,
    Sort_Name,       Sort_Brief,    Sort_Description, 
    Sort_Definition, Sort_Examples, Sort_Methods));
  
void sort(var self) {
  method(self, Sort, sort_by, lt);
}

void sort_by(var self, bool(*f)(var,var)) {
  method(self, Sort, sort_by, f);
}
