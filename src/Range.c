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
var Range = Cello(Range,
  Instance(Doc,
    Range_Name, Range_Brief, Range_Description, Range_Examples, Range_Methods),
  Instance(Iter, Range_Iter_Init, Range_Iter_Next));
  
/* TODO: Make this `New` and just get macro to call constructor */
  
var range_with(var self, var args) {
  struct Range* r = self;
  size_t nargs = len(args);
  
  if (nargs > 3 or nargs < 1) {
    throw(FormatError, "Recieved too many arguments to range constructor");
  }
  
  if (nargs is 1) {
    r->start = 0;
    r->stop  = c_int(get(args, $I(0)));
    r->step  = 1;
    return self;
  }
  
  if (nargs is 2) {
    r->start = c_int(get(args, $I(0)));
    r->stop  = c_int(get(args, $I(1)));
    r->step  = 1;
    return self;
  }
  
  if (nargs is 3) {
    r->start = c_int(get(args, $I(0)));
    r->stop  = c_int(get(args, $I(1)));
    r->step  = c_int(get(args, $I(2)));
  }
  
  return self;
}
  
