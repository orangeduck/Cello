#ifndef CelloFunction_h
#define CelloFunction_h

#include "Prelude.h"
#include "Type.h"

/** Call - can be called like a function */

class {
  var (*call_with)(var,var);
} Call;

#define call(x, ...) call_vl(x, var_list_new(__VA_ARGS__))
var call_vl(var self, var_list vl);
var call_with(var self, var args);

/*
** == Function ==
**
**  Simple Higher Level Functions
**  Must by of type (var(*)(var,var))
**  Can be constructed using "lambda"
**
**    lambda(hello_person, args) {
**      var name = cast(pop(args), String);
**      printf("Hello %s!", as_str(name) );
**      return None;
**    };
**
**    call(hello_person, $(String, "Dan"));
**
**
**	See 'Cello/Lambda.h' for more functional tools.
**
*/

global var Function;

#if defined(__clang__)

  data {
    var type;
    var (^func)(var);
  } FunctionData;

  #define lambda(name, args) \
    FunctionData* name = $(Function, NULL); \
    name->func = ^ var (var args)

#else

  data {
    var type;
    var (*func)(var);
  } FunctionData;

  #define lambda(name, args) \
    auto var __LambdaCello_##name(var); \
    var name = $(Function, __LambdaCello_##name); \
    var __LambdaCello_##name(var args)

#endif
  
var Function_New(var self, var_list vl);
var Function_Delete(var self);
size_t Function_Size(void);

var Function_Copy(var self);
void Function_Assign(var self, var obj);

var Function_Call(var self, var args);

instance(Function, New) = { Function_New, Function_Delete, Function_Size };
instance(Function, Copy) = { Function_Copy };
instance(Function, Assign) = { Function_Assign };
instance(Function, Call) = { Function_Call };

#endif
