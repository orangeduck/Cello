#include "Cello.h"

static const char* Eq_Name(void) {
  return "Eq";
}

/* TODO */
static const char* Eq_Brief(void) {
  return "";
}

/* TODO */
static const char* Eq_Description(void) {
  return "";
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
  
  struct Size* s = instance(self, Size);
  if (type_of(self) is type_of(obj) and s and s->size) {
    return memcmp(self, obj, s->size()) is 0;
  }
  
  return self is obj;
}

bool neq(var self, var obj) { return not eq(self, obj); }

static const char* Ord_Name(void) {
  return "Ord";
}

/* TODO */
static const char* Ord_Brief(void) {
  return "";
}

/* TODO */
static const char* Ord_Description(void) {
  return "";
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
  
  struct Size* s = instance(self, Size);
  if (type_of(self) is type_of(obj) and s and s->size) {
    return memcmp(self, obj, s->size()) > 0;
  }
  
  return self > obj;
  
}

bool lt(var self, var obj) {

  struct Ord* o = instance(self, Ord);
  if (o and o->lt) {
    return o->lt(self, obj);
  }
  
  struct Size* s = instance(self, Size);
  if (type_of(self) is type_of(obj) and s and s->size) {
    return memcmp(self, obj, s->size()) < 0;
  }
  
  return self < obj;
  
}

bool ge(var self, var obj) { return not lt(self, obj); }
bool le(var self, var obj) { return not gt(self, obj); }

