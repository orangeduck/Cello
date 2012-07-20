#ifndef NoneTypePlus_h
#define NoneTypePlus_h

#include "Prelude+.h"
#include "Type+.h"

module NoneType;

#define None &NoneType

const char* NoneType_AsStr(var);

instance(NoneType, AsStr) = { NoneType_AsStr };

#endif