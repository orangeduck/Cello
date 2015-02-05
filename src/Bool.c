#include "Cello.h"

static const char* Bool_Name(void) {
  return "Bool";
}

static const char* Bool_Brief(void) {
  return "Rich Boolean Type";
}

static const char* Bool_Description(void) {
  return ""
    "Bool is a Cello wrapper of a standard C bool. It is defined as follows."
    ""
    "    local var True  = (var)1;"
    "    local var False = (var)0;"
    "    "
    "These evaluate to true and false correctly in `if` statements and can be "
    "returned, passed around, and manipulated using the standard logical "
    "operators. Bools can be cast to c `bool` and back again without issue."
    ""
    "There is no data object for Bool. This is because they are hard coded "
    "into `type_of`. This also means they cannot be stored in data structures "
    "or allocated on the Heap.";
}

static const char* Bool_Examples(void) {
  return ""
    "__Logic__\n"
    "    \n"
    "    var x = True;\n"
    "    var y = False;\n"
    "    \n"
    "    show(x and y); /* False */\n"
    "    show(x or y);  /* True */\n"
    "    show(x or y);  /* True */\n"
    "    show(x and not y);  /* True */\n"
    "    show(x or not y);   /* True */\n"
    "    show(not (x or y)); /* False */\n"
    "    \n"
    "__Properties__\n"
    "    \n"
    "    var x = True;\n"
    "    var y = False;\n"
    "    \n"
    "    show(gt(x, y));   /* True */\n"
    "    show($(Int, c_long(x)));   /* 1 */\n"
    "    show($(String, c_str(x))); /* \"True\" */\n";
}

static const char* Bool_Methods(void) {
  return ""
    "\n"
    "    var bool_var(inptr_t x);\n"
    "\n"
    "__Arguments__\n"
    "* `x` C style boolean value, usually result of boolean operations\n"
    "\n"
    "__Returns__\n"
    "* Cello style boolean of type `var`\n";
}

static var Bool_Eq(var self, var obj) {
  return bool_var(self is obj);
}

static var Bool_Gt(var self, var obj) {
  return bool_var(self > obj);
}

static var Bool_Lt(var self, var obj) {
  return bool_var(self < obj);
}

static uint64_t Bool_Hash(var self) {
  return (uint64_t)(self is True);
}

static char Bool_C_Char(var self) {
  return (char)(self is True);
}

static char* Bool_C_Str(var self) {
  return self ? (char*)"True" : (char*)"False";
}

int64_t Bool_C_Int(var self) {
  return (int64_t)(self is True);
}

double Bool_C_Float(var self) {
  return (double)(self is True);
}

int Bool_Show(var self, var output, int pos) {
  if (self) {
    return print_to(output, pos, "True");
  } else {
    return print_to(output, pos, "False");
  }
}

var Bool = Cello(Bool,
  Member(Doc,
    Bool_Name,     Bool_Brief, Bool_Description, 
    Bool_Examples, Bool_Methods),
  Member(Eq,       Bool_Eq),
  Member(Ord,      Bool_Gt, Bool_Lt),
  Member(Hash,     Bool_Hash),
  Member(C_Char,   Bool_C_Char),
  Member(C_Int,    Bool_C_Int),
  Member(C_Float,  Bool_C_Float),
  Member(C_Str,    Bool_C_Str),
  Member(Show,     Bool_Show, NULL));

var bool_var(intptr_t x) {
  return (var)(intptr_t)(((var)(intptr_t)x) is True);
}
