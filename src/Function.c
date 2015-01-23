#include "Cello.h"

static const char* Call_Name(void) {
  return "Name";
}

/* TODO */
static const char* Call_Brief(void) {
  return "";
}

/* TODO */
static const char* Call_Description(void) {
  return "";
}

/* TODO */
static const char* Call_Examples(void) {
  return "";
}

/* TODO */
static const char* Call_Methods(void) {
  return "";
}

var Call = typedecl(Call,
  typeclass(Doc,
    Call_Name, Call_Brief, Call_Description, Call_Examples, Call_Methods));

var call_with(var self, var args) {
  return method(self, Call, call_with, args);
}

static const char* Function_Name(void) {
  return "Function";
}

/* TODO */
static const char* Function_Brief(void) {
  return "";
}

/* TODO */
static const char* Function_Description(void) {
  return "";
}

/* TODO */
static const char* Function_Examples(void) {
  return "";
}

/* TODO */
static const char* Function_Methods(void) {
  return "";
}

static var Function_New(var self, var args) {
  struct Function* f = self;
  f->func = get(args, $(Int, 0));
  return self;
}

static var Function_Del(var self) {
  return self;
}

static size_t Function_Size(void) {
  return sizeof(struct Function);
}

static var Function_Copy(var self) {
  return new(Function, self);
}

static var Function_Assign(var self, var obj) {
  struct Function* f0 = cast(self, Function);
  struct Function* f1 = cast(obj, Function);
  f0->func = f1->func;
  return self;
}

static var Function_Call(var self, var args) {
  struct Function* f = self;
  return f->func(args);
}

var Function = typedecl(Function,
  typeclass(Doc,
    Function_Name, Function_Brief, Function_Description,
    Function_Examples, Function_Methods),
  typeclass(New, Function_New, Function_Del, Function_Size),
  typeclass(Copy, Function_Copy),
  typeclass(Assign, Function_Assign),
  typeclass(Call, Function_Call));

void map(var self, var func) {
  foreach(item in self) {
    call(func, item);
  }
}
