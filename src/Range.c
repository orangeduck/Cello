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
  struct Int* i = r->iter;
  if (r->step == 0) { return Terminal; }
  if (r->step  > 0) { i->val = r->start; }
  if (r->step  < 0) { i->val = r->stop-1; }
  if (r->step  > 0 and i->val >= r->stop) { return Terminal; }
  if (r->step  < 0 and i->val <  r->stop) { return Terminal; }
  return i;
}

static var Range_Iter_Next(var self, var curr) {
  struct Range* r = self;
  struct Int* i = r->iter;
  i->val += r->step;
  if (r->step == 0) { return Terminal; }
  if (r->step  > 0 and i->val >= r->stop) { return Terminal; }
  if (r->step  < 0 and i->val < r->start) { return Terminal; }
  return i;
}

/* TODO: Add extra methods */
var Range = typedecl(Range,
  typeclass(Doc,
    Range_Name, Range_Brief, Range_Description, Range_Examples, Range_Methods),
  typeclass(Iter, Range_Iter_Init, Range_Iter_Next));
  
