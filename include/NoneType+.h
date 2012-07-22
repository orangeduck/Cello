/*
** == NoneType ==
**
**  + Singleton called None
**  + Useful to replace NULL
**  + Gives readable errors (instead of crashes)
*/

#ifndef NoneTypePlus_h
#define NoneTypePlus_h

#include "Prelude+.h"
#include "Type+.h"

module NoneType;

#define None &NoneType

const char* NoneType_AsStr(var self);

instance(NoneType, AsStr) = { NoneType_AsStr };

#endif