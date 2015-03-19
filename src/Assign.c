#include "Cello.h"

static const char* Assign_Name(void) {
  return "Assign";
}

static const char* Assign_Brief(void) {
  return "Construction from Object";
}

static const char* Assign_Description(void) {
  return
    "`Assign` is potentially the most important class in Cello. It is used "
    "throughout Cello to initialise objects using other objects. In C++ this is "
    "called the _copy constructor_ and it is used to assign the value of one "
    "object to another."
    "\n\n"
    "By default the `Assign` class uses the `Size` class to copy the memory "
    "from one object to another. But for more complex objects which maintain "
    "their own behaviours and state this may need to be overridden."
    "\n\n"
    "The most important thing about the `Assign` class is that it must work on "
    "the assumption that the target object could be uninitialised with zero'd "
    "memory.";
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
