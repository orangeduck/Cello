#include "Cello.h"

static const char* Call_Name(void) {
  return "Call";
}

static const char* Call_Brief(void) {
  return "Callable";
}

static const char* Call_Description(void) {
  return
    "The `Call` class is used by types which can be called as functions.";
}

static const char* Call_Definition(void) {
  return
    "struct Call {\n"
    "  var (*call_with)(var, var);\n"
    "};\n";
}

static struct Example* Call_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var increment(var args) {\n"
      "  struct Int* i = get(args, $I(0));\n"
      "  i->val++;\n"
      "  return NULL;\n"
      "}\n"
      "\n"
      "var x = $I(0);\n"
      "show(x); /* 0 */\n"
      "call($(Function, increment), x);\n"
      "show(x); /* 1 */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* Call_Methods(void) {
  
  static struct Method methods[] = {
    {
      "call", 
      "#define call(self, ...)\n"
      "var call_with(var self, var args);",
      "Call the object `self` with arguments `args`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Call = Cello(Call,
  Instance(Doc,
    Call_Name,       Call_Brief,    Call_Description, 
    Call_Definition, Call_Examples, Call_Methods));

var call_with(var self, var args) {
  return method(self, Call, call_with, args);
}

static const char* Function_Name(void) {
  return "Function";
}

static const char* Function_Brief(void) {
  return "Function Object";
}

static const char* Function_Description(void) {
  return
    "The `Function` type allows C function pointers to be treated as "
    "Cello objects. They can be passed around, stored, and manipulated. Only C "
    "functions of the type `var(*)(var)` can be stored as a `Function` type "
    "and when called the arguments will be wrapped into an iterable and passed "
    "as the first argument, typically in the form of a `tuple`.";
}

static struct Example* Function_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var increment(var args) {\n"
      "  struct Int* i = get(args, $I(0));\n"
      "  i->val++;\n"
      "  return NULL;\n"
      "}\n"
      "\n"
      "var x = $I(0);\n"
      "show(x); /* 0 */\n"
      "call($(Function, increment), x);\n"
      "show(x); /* 1 */\n"
    }, {
      "Usage 2",
      "var hello_person(var args) {\n"
      "  print(\"Hello %$!\", get(args, $I(0)));\n"
      "  return NULL;\n"
      "}\n"
      "\n"
      "call($(Function, hello_person), $S(\"Dan\"));\n"
    }, {
      "Usage 3",
      "var add_print(var args) {\n"
      "  int64_t fst = c_int(get(args, $I(0)));\n"
      "  int64_t snd = c_int(get(args, $I(1)));\n"
      "  println(\"%i + %i = %i\", $I(fst), $I(snd), $I(fst+snd));\n"
      "  return NULL;\n"
      "}\n"
      "\n"
      "call($(Function, add_print), $I(10), $I(21));\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static const char* Function_Definition(void) {
  return
    "struct Function {\n"
    "  var (*func)(var);\n"
    "};\n";
}

static var Function_Call(var self, var args) {
  struct Function* f = self;
  return f->func(args);
}

var Function = Cello(Function,
  Instance(Doc,
    Function_Name,       Function_Brief,    Function_Description,
    Function_Definition, Function_Examples, NULL),
  Instance(Call, Function_Call));


