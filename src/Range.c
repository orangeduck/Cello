#include "Cello.h"

static const char* Range_Name(void) {
  return "Range";
}

/* TODO */
static const char* Range_Brief(void) {
  return "";
}

/* TODO */
static const char* Range_Description(void) {
  return "";
}

/* TODO */
static const char* Range_Examples(void) {
  return "";
}

/* TODO */
static const char* Range_Methods(void) {
  return "";
}

static var Range_Iter_Init(var self) {
  struct Range* r = self;
  return r->start;
}

static var Range_Iter_Next(var self, var curr) {
  struct Range* r = self;
  madd(r->start, r->step);
  if (eq(r->step, $(Int, 0))) { return Terminal; }
  if (gt(r->step, $(Int, 0)) and ge(r->start, r->stop)) { return Terminal; }
  if (lt(r->step, $(Int, 0)) and le(r->start, r->stop)) { return Terminal; }
  return r->start;
}

/* TODO: Add extra methods */
var Range = typedecl(Range,
  typeclass(Doc,
    Range_Name, Range_Brief, Range_Description, Range_Examples, Range_Methods),
  typeclass(Iter, Range_Iter_Init, Range_Iter_Next));
  
