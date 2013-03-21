#ifndef ReferencePlus_h
#define ReferencePlus_h

#include "Prelude+.h"

module Reference;

data {
  var type;
  var ref;
} ReferenceData;

/** Dictionary_New(var self, var obj); */
var Reference_New(var self, va_list* args);
var Reference_Delete(var self);
void Reference_Assign(var self, var obj);
var Reference_Copy(var self);
var Reference_Eq(var self, var obj);
long Reference_Hash(var self);
var Reference_At(var self, int i);
void Reference_Set(var self, int i, var x);

instance(Reference, New) = { sizeof(ReferenceData), Reference_New, Reference_Delete };
instance(Reference, Assign) = { Reference_Assign };
instance(Reference, Copy) = { Reference_Copy };
instance(Reference, Eq) = { Reference_Eq };
instance(Reference, Hash) = { Reference_Hash };
instance(Reference, At) = { Reference_At, Reference_Set };

#endif