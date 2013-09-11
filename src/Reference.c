#include "Cello/Reference.h"

#include "Cello/Bool.h"
#include "Cello/Type.h"
#include "Cello/Exception.h"

#include <string.h>

var Reference = type_data {
  type_begin(Reference),
  type_entry(Reference, New), 
  type_entry(Reference, Assign),
  type_entry(Reference, Copy),
  type_entry(Reference, Eq), 
  type_entry(Reference, Hash),
  type_entry(Reference, At),
  type_entry(Reference, With),
  type_entry(Reference, Show),
  type_end(Reference)
};

var Reference_New(var self, var_list vl) {
  ReferenceData* rd = cast(self, Reference);
  rd->ref = var_list_get(vl);
  return rd;
}

var Reference_Delete(var self) {
  return self;
}

size_t Reference_Size(void) {
  return sizeof(ReferenceData);
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
  return bool_var(rd0->ref is rd1->ref);
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

