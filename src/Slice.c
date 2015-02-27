#include "Cello.h"

static const char* Slice_Name(void) {
  return "Slice";
}

/* TODO */
static const char* Slice_Brief(void) {
  return "";
}

/* TODO */
static const char* Slice_Description(void) {
  return "";
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

  return Terminal;
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

