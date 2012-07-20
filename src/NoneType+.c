#include "NoneType+.h"

var NoneType = methods {
  methods_begin(NoneType),
  method(NoneType, AsStr),
  methods_end(NoneType)
};

static const char* none_str = "None";

const char* NoneType_AsStr(var self) {
  self = cast(self, NoneType);
  return none_str;
}