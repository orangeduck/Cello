#include "Cello.h"

static const char* Range_Name(void) {
  return "Range";
}

static const char* Range_Brief(void) {
  return "A Sequence of Numbers";
}

static const char* Range_Description(void) {
  return
    "The `Range` type is a basic iterable type which acts as a virtual "
    "sequence of numbers, starting from some value, stopping at some value "
    "and incrementing by some step."
    "\n\n"
    "This can be a useful replacement for the standard C `for` loop with "
    "decent performance but returning a wrapped integer. To range backwards "
    "`iter_prev` must be defined on the iterable object.";
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
  if (r->step == 0) { return NULL; }
  if (r->step  > 0) { i->val = r->start; }
  if (r->step  < 0) { i->val = r->stop-1; }
  if (r->step  > 0 and i->val >= r->stop) { return NULL; }
  if (r->step  < 0 and i->val <  r->stop) { return NULL; }
  return i;
}

static var Range_Iter_Next(var self, var curr) {
  struct Range* r = self;
  struct Int* i = r->iter;
  i->val += r->step;
  if (r->step == 0) { return NULL; }
  if (r->step  > 0 and i->val >= r->stop) { return NULL; }
  if (r->step  < 0 and i->val < r->start) { return NULL; }
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


static const char* Slice_Name(void) {
  return "Slice";
}

static const char* Slice_Brief(void) {
  return "Partial Iterable";
}

static const char* Slice_Description(void) {
  return
    "The `Slice` type is an iterable that allows one to only iterate over "
    "certain items of another iterable. Given some start, stop and step only "
    "those entries described by the slice are returned in the iteration."
    "\n\n"
    "Under the hood the `Slice` object still iterates over the whole iterable "
    "but it only returns those values in the range. For this reason there is "
    "no performance benefit to slicing.";
}

/* TODO */
static const char* Slice_Examples(void) {
  return "";
}

/* TODO */
static const char* Slice_Methods(void) {
  return "";
}

static var Slice_Iter_Init(var self) {
  struct Slice* s = self;
  struct Range* r = s->range;
  
  if (r->step > 0) {
    var curr = iter_init(s->iter);
    for(int64_t i = 0; i < r->start; i++) {
      curr = iter_next(s->iter, curr);
    }
    return curr;
  }
  
  if (r->step < 0) {
    var curr = iter_last(s->iter);
    for(int64_t i = 0; i < (int64_t)len(s->iter)-r->stop; i++) {
      curr = iter_prev(s->iter, curr);
    }
    return curr;
  }

  return NULL;
}

static var Slice_Iter_Next(var self, var curr) {
  struct Slice* s = self;
  struct Range* r = s->range;
  
  if (r->step > 0) {
    for (int64_t i = 0; i < r->step; i++) {
      curr = iter_next(s->iter, curr);
    }
  }
  
  if (r->step < 0) {
    for (int64_t i = 0; i < -r->step; i++) {
      curr = iter_prev(s->iter, curr);
    }
  }
  
  return curr;
}

/* TODO: Add extra methods */
var Slice = Cello(Slice,
  Instance(Doc,
    Slice_Name, Slice_Brief, Slice_Description, Slice_Examples, Slice_Methods),
  Instance(Iter, Slice_Iter_Init, Slice_Iter_Next));

