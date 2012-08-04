#ifndef FunctionPlus_h
#define FunctionPlus_h

#include "Prelude+.h"
#include "Type+.h"


/** Call - Can be called like a function */

class {
  var (*call_with)(var, var);
} Call;

#define call(self, ...) call_with_ptr(self, sizeof((var[]){ __VA_ARGS__ }) / sizeof(var), (var[]){ __VA_ARGS__ } );
var call_with(var self, var args);

// args must be NULL terminted
var call_with_ptr(var self, int argc, var* args);


/*
** == Function ==
**
**  + First Class Function Type.
**  + Functions have the absolute type (var(*)(var))
**  + They take a list of arguments and always return "var"
**  + These can be build nested or otherwise using "lambda"
*/

module Function;

data {
  var type;
  var(*func)(var);
} FunctionData;

#define lambda(name, args) auto var __Lambda##name(var args); \
  var name = $(Function, __Lambda##name ); \
  var __Lambda##name(var args)

var Function_Call(var func, var args);

instance(Function, Call) = { Function_Call };


#endif
