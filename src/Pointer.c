#include "Cello.h"

static const char* Pointer_Name(void) {
  return "Pointer";
}

/* TODO */
static const char* Pointer_Brief(void) {
  return "";
}

/* TODO */
static const char* Pointer_Description(void) {
  return "";
}

/* TODO */
static const char* Pointer_Examples(void) {
  return "";
}

/* TODO */
static const char* Pointer_Methods(void) {
  return "";
}

var Pointer = Cello(Pointer,
  Member(Doc,
    Pointer_Name, Pointer_Brief, Pointer_Description, 
    Pointer_Examples, Pointer_Methods));

void ref(var self, var item) {
  method(self, Pointer, ref, item);
}

var deref(var self) {
  return method(self, Pointer, deref);
}

static const char* Ref_Name(void) {
  return "Ref";
}

/* TODO */
static const char* Ref_Brief(void) {
  return "";
}

/* TODO */
static const char* Ref_Description(void) {
  return "";
}

/* TODO */
static const char* Ref_Examples(void) {
  return "";
}

/* TODO */
static const char* Ref_Methods(void) {
  return "";
}

static var Ref_New(var self, var args) {
  struct Ref* r = self;
  r->val = get(args, $(Int, 0));
  return self;
}

static var Ref_Del(var self) {
  return self;
}

static size_t Ref_Size(void) {
  return sizeof(struct Ref);
}

static var Ref_Assign(var self, var obj) {
  if (implements(obj, Pointer)) {
    ref(self, deref(obj));
  } else {
    ref(self, obj);
  }
  return self;
}

static var Ref_Copy(var self) {
  return new(Ref, self);
}

static int Ref_Show(var self, var output, int pos) {
  return print_to(output, pos, "<'Ref' at 0x%p (%$)>", self, deref(self));
}

static void Ref_Ref(var self, var val) {
  struct Ref* r = self;
  r->val = val;
}

static var Ref_Deref(var self) {
  struct Ref* r = self;
  return r->val;
}

var Ref = Cello(Ref,
  Member(Doc,
    Ref_Name, Ref_Brief, Ref_Description, Ref_Examples, Ref_Methods),
  Member(Size,    Ref_Size),
  Member(New,     Ref_New, Ref_Del),
  Member(Assign,  Ref_Assign),
  Member(Copy,    Ref_Copy),
  Member(Show,    Ref_Show, NULL),
  Member(Pointer, Ref_Ref, Ref_Deref));

  
static const char* Box_Name(void) {
  return "Box";
}

/* TODO */
static const char* Box_Brief(void) {
  return "";
}

/* TODO */
static const char* Box_Description(void) {
  return "";
}

/* TODO */
static const char* Box_Examples(void) {
  return "";
}

/* TODO */
static const char* Box_Methods(void) {
  return "";
}

static var Box_New(var self, var args) {
  struct Box* b = self;
  b->val = get(args, $(Int, 0));
  return self;
}

static var Box_Del(var self) {
  del(deref(self));
  ref(self, None);
  return self;
}

static size_t Box_Size(void) {
  return sizeof(struct Box);
}

static var Box_Assign(var self, var obj) {
  if (implements(obj, Pointer)) {
    ref(self, deref(obj));
  } else {
    ref(self, obj);
  }
  return self;
}

static var Box_Copy(var self) {
  return new(Box, self);
}

static int Box_Show(var self, var output, int pos) {
  return print_to(output, pos, "<'Box' at 0x%p (%$)>", self, deref(self));
}

static void Box_Ref(var self, var val) {
  struct Box* b = self;
  b->val = val;
}

static var Box_Deref(var self) {
  struct Box* b = self;
  return b->val;
}

var Box = Cello(Box,
  Member(Doc,
    Box_Name, Box_Brief, Box_Description, Box_Examples, Box_Methods),
  Member(Size,    Box_Size),
  Member(New,     Box_New, Box_Del),
  Member(Assign,  Box_Assign),
  Member(Copy,    Box_Copy),
  Member(Show,    Box_Show, NULL),
  Member(Pointer, Box_Ref, Box_Deref));
  
