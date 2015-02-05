#include "Cello.h"

static const char* Begin_Name(void) {
  return "Begin";
}

/* TODO */
static const char* Begin_Brief(void) {
  return "";
}

/* TODO */
static const char* Begin_Description(void) {
  return "";
}

/* TODO */
static const char* Begin_Examples(void) {
  return "";
}

/* TODO */
static const char* Begin_Methods(void) {
  return "";
}

var Begin = Cello(Begin,
  Member(Doc,
    Begin_Name, Begin_Brief, Begin_Description, 
    Begin_Examples, Begin_Methods));

void begin(var self) {
  if (implements_method(self, Begin, begin)) {
    method(self, Begin, begin);
  }
}

void end(var self) {
  if (implements_method(self, Begin, end)) {
    method(self, Begin, end);
  }
}

var begin_with(var self) {
  begin(self);
  return self;
}

var end_with(var self) {
  end(self);
  return Terminal;
}
