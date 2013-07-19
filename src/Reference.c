#include "Cello/Reference.h"

#include "Cello/Type.h"
#include "Cello/Exception.h"

#include <string.h>

var Reference = methods {
  methods_begin(Reference),
  method(Reference, New), 
  method(Reference, Assign),
  method(Reference, Copy),
  method(Reference, Eq), 
  method(Reference, Hash),
  method(Reference, At),
  method(Reference, With),
  method(Reference, Show),
  methods_end(Reference)
};

var Reference_New(var self, va_list* args) {
  ReferenceData* rd = cast(self, Reference);
  rd->ref = va_arg(*args, var);
  return rd;
}

var Reference_Delete(var self) {
  return self;
}

void Reference_Assign(var self, var obj) {
  ReferenceData* rd0 = cast(self, Reference);
  ReferenceData* rd1 = cast(obj, Reference);
  rd0->ref = rd1->ref;
}

var Reference_Copy(var self) {
  ReferenceData* rd = cast(self, Reference);
  return new(Reference, rd->ref);
}

var Reference_Eq(var self, var obj) {
  ReferenceData* rd0 = cast(self, Reference);
  ReferenceData* rd1 = cast(obj, Reference);
  return (var)(intptr_t)(rd0->ref is rd1->ref);
}

long Reference_Hash(var self) {
  ReferenceData* rd = cast(self, Reference);
  return (long)(intptr_t)(rd->ref);
}

var Reference_At(var self, int i) {
  ReferenceData* rd = cast(self, Reference);
  return rd->ref;
}

void Reference_Set(var self, int i, var x) {
  ReferenceData* rd = cast(self, Reference);
  rd->ref = x;
}

void Reference_Exit(var self) {
  ReferenceData* rd = cast(self, Reference);
  delete(rd->ref);
}

int Reference_Show(var self, var output, int pos) {
  return print_to(output, pos, "<'Reference' at 0x%p (%$)>", self, at(self,0));
}

