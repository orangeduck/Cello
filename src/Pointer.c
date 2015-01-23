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

var Pointer = typedecl(Pointer,
  typeclass(Doc,
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

static var Ref_Forward(var self) {
  return deref(self);
}

var Ref = typedecl(Ref,
  typeclass(Doc,
    Ref_Name, Ref_Brief, Ref_Description, Ref_Examples, Ref_Methods),
  typeclass(New, Ref_New, Ref_Del, Ref_Size),
  typeclass(Assign, Ref_Assign),
  typeclass(Copy, Ref_Copy),
  typeclass(Show, Ref_Show, NULL),
  typeclass(Pointer, Ref_Ref, Ref_Deref));

  
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

static var Box_Forward(var self) {
  return deref(self);
}

var Box = typedecl(Box,
  typeclass(Doc,
    Box_Name, Box_Brief, Box_Description, Box_Examples, Box_Methods),
  typeclass(New, Box_New, Box_Del, Box_Size),
  typeclass(Assign, Box_Assign),
  typeclass(Copy, Box_Copy),
  typeclass(Show, Box_Show, NULL),
  typeclass(Pointer, Box_Ref, Box_Deref));
  