#include "Cello.h"

static const char* C_Int_Name(void) {
  return "C_Int";
}

static const char* C_Int_Brief(void) {
  return "Interpret as C Integer";
}

static const char* C_Int_Description(void) {
  return
    "The `C_Int` class should be overridden by types which are representable "
    "as a C style Integer of the type `int64_t`.";
}

static const char* C_Int_Definition(void) {
  return
    "struct C_Int {\n"
    "  int64_t (*c_int)(var);\n"
    "};\n";
}

static struct Example* C_Int_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "printf(\"%li\", c_int($I(5))); /* 5 */\n"
      "printf(\"%li\", c_int($I(6))); /* 6 */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* C_Int_Methods(void) {
  
  static struct Method methods[] = {
    {
      "c_int", 
      "int64_t c_int(var self);",
      "Returns the object `self` represented as a `int64_t`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var C_Int = Cello(C_Int,
  Instance(Doc,
    C_Int_Name,       C_Int_Brief,    C_Int_Description, 
    C_Int_Definition, C_Int_Examples, C_Int_Methods));
    
static const char* C_Float_Name(void) {
  return "C_Float";
}

static const char* C_Float_Brief(void) {
  return "Interpret as C Float";
}

static const char* C_Float_Description(void) {
  return
    "The `C_Float` class should be overridden by types which are representable "
    "as a C style Float of the type `double`.";
}

static const char* C_Float_Definition(void) {
  return
    "struct C_Float {\n"
    "  double (*c_float)(var);\n"
    "};\n";
}

static struct Example* C_Float_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "printf(\"%f\", c_float($F(5.1))); /* 5.1 */\n"
      "printf(\"%f\", c_float($F(6.2))); /* 6.2 */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* C_Float_Methods(void) {
  
  static struct Method methods[] = {
    {
      "c_float", 
      "double c_float(var self);",
      "Returns the object `self` represented as a `double`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var C_Float = Cello(C_Float,
  Instance(Doc,
    C_Float_Name,       C_Float_Brief,    C_Float_Description, 
    C_Float_Definition, C_Float_Examples, C_Float_Methods));

int64_t c_int(var self) {
  
  if (type_of(self) is Int) {
    return ((struct Int*)self)->val;
  }
  
  return method(self, C_Int, c_int);
}

double c_float(var self) {
  
  if (type_of(self) is Float) {
    return ((struct Float*)self)->val;
  }
  
  return method(self, C_Float, c_float);
}

static const char* Int_Name(void) {
  return "Int";
}

static const char* Int_Brief(void) {
  return "Integer Object";
}

static const char* Int_Description(void) {
  return "64-bit signed integer Object.";
}

static const char* Int_Definition(void) {
  return
    "struct Int {\n"
    "  int64_t val;\n"
    "};\n";
}

static struct Example* Int_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var i0 = $(Int, 1);\n"
      "var i1 = new(Int, $I(24313));\n"
      "var i2 = copy(i0);\n"
      "\n"
      "show(i0); /*     1 */\n"
      "show(i1); /* 24313 */\n"
      "show(i2); /*     1 */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static void Int_Assign(var self, var obj) {
  struct Int* i = self;
  i->val = c_int(obj);
}

static int64_t Int_C_Int(var self) {
  struct Int* i = self;
  return i->val;
}

static int Int_Cmp(var self, var obj) {
  return (int)(Int_C_Int(self) - c_int(obj));
}

static uint64_t Int_Hash(var self) {
  return (uint64_t)c_int(self);
}

static int Int_Show(var self, var output, int pos) {
  return print_to(output, pos, "%li", self);
}

static int Int_Look(var self, var input, int pos) {
  return scan_from(input, pos, "%li", self);
}

var Int = Cello(Int,
  Instance(Doc,
    Int_Name, Int_Brief, Int_Description, Int_Definition, Int_Examples, NULL),
  Instance(Assign,  Int_Assign),
  Instance(Cmp,     Int_Cmp),
  Instance(Hash,    Int_Hash),
  Instance(C_Int,   Int_C_Int),
  Instance(Show,    Int_Show, Int_Look));

static const char* Float_Name(void) {
  return "Float";
}

static const char* Float_Brief(void) {
  return "Floating Point Object";
}

static const char* Float_Description(void) {
  return "64-bit double precision float point Object.";
}

static const char* Float_Definition(void) {
  return
    "struct Float {\n"
    "  double val;\n"
    "};\n";  
}

static struct Example* Float_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var f0 = $(Float, 1.0);\n"
      "var f1 = new(Float, $F(24.313));\n"
      "var f2 = copy(f0);\n"
      "\n"
      "show(f0); /*  1.000 */\n"
      "show(f1); /* 24.313 */\n"
      "show(f2); /*  1.000 */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static void Float_Assign(var self, var obj) {
  struct Float* f = self;
  f->val = c_float(obj);
}

static double Float_C_Float(var self) {
  struct Float* f = self;
  return f->val;
}

static int Float_Cmp(var self, var obj) {
  double c = Float_C_Float(self) - c_float(obj);
  return c > 0 ? 1 : c < 0 ? -1 : 0;
}

union interp_cast {
  double   as_flt;
  uint64_t as_int;
};

static uint64_t Float_Hash(var self) {
  union interp_cast ic;
  ic.as_flt = c_float(self);
  return ic.as_int;
}

int Float_Show(var self, var output, int pos) {
  return print_to(output, pos, "%f", self);
}

int Float_Look(var self, var input, int pos) {
  return scan_from(input, pos, "%f", self);
}

var Float = Cello(Float,
  Instance(Doc,
    Float_Name,       Float_Brief,    Float_Description, 
    Float_Definition, Float_Examples, NULL),
  Instance(Assign,  Float_Assign),
  Instance(Cmp,     Float_Cmp),
  Instance(Hash,    Float_Hash),
  Instance(C_Float, Float_C_Float),
  Instance(Show,    Float_Show, Float_Look));
