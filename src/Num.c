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

/* TODO */
static const char* C_Int_Examples(void) {
  return "";
}

/* TODO */
static const char* C_Int_Methods(void) {
  return "";
}

var C_Int = Cello(C_Int,
  Instance(Doc,
    C_Int_Name, C_Int_Brief, C_Int_Description, 
    C_Int_Examples, C_Int_Methods));
    
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

/* TODO */
static const char* C_Float_Examples(void) {
  return "";
}

/* TODO */
static const char* C_Float_Methods(void) {
  return "";
}

var C_Float = Cello(C_Float,
  Instance(Doc,
    C_Float_Name, C_Float_Brief, C_Float_Description, 
    C_Float_Examples, C_Float_Methods));

char* c_str(var self) {
  
  if (type_of(self) is String) {
    return ((struct String*)self)->val;
  }
  
  return method(self, C_Str, c_str);
}

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
  return "64-bit unsigned integer Object.";
}

static const char* Int_Examples(void) {
  return ""
    "__Usage__\n"
    "    \n"
    "    var i0 = $(Int, 1);\n"
    "    var i1 = new$(Int, 24313);\n"
    "    var i2 = copy(i0);\n"
    "    \n"
    "    show(i0); /* 1 */\n"
    "    show(i1); /* 24313 */\n"
    "    show(i2); /* 1 */\n"
    "    \n"
    "    del(i1);\n"
    "    del(i2);\n"
    "    \n"
    "__Maths__\n"
    "    \n"
    "    var i0 = $(Int, 0);\n"
    "    \n"
    "    show(i0); /* 0 */\n"
    "    \n"
    "    madd(i0, $(Int, 10)); /* 10 */\n"
    "    mmul(i0, $(Int, 3));  /* 30 */\n"
    "    mdiv(i0, $(Int, 2));  /* 15 */\n"
    "    \n"
    "    if_gt(i0, $(Int, 10)) {\n"
    "        print(\"%i is greater than 10!\", i0);\n"
    "    }\n";
}

static const char* Int_Methods(void) {
  return "";
}

static void Int_Assign(var self, var obj) {
  struct Int* i = self;
  i->val = c_int(obj);
}

static int64_t Int_C_Int(var self) {
  struct Int* i = self;
  return i->val;
}

static bool Int_Eq(var self, var obj) {
  return Int_C_Int(self) is c_int(obj);
}

static bool Int_Gt(var self, var obj) {
  return Int_C_Int(self) > c_int(obj);
}

static bool Int_Lt(var self, var obj) {
  return Int_C_Int(self) < c_int(obj);
}

static int Int_Cmp(var self, var obj) {
  return Int_C_Int(self) - c_int(obj);
}

static uint64_t Int_Hash(var self) {
  return (uint64_t)c_int(self);
}

static int Int_Show(var self, var output, int pos) {
  struct Int* i = self;
  return format_to(output, pos, "%li", i->val);
}

static int Int_Look(var self, var input, int pos) {
  struct Int* i = self;
  int off = 0;
  int err = format_from(input, pos, "%li%n", &i->val, &off);
  return pos + off;
}

var Int = Cello(Int,
  Instance(Doc,
    Int_Name, Int_Brief, Int_Description, Int_Examples, Int_Methods),
  Instance(Assign,  Int_Assign),
  Instance(Eq,      Int_Eq),
  Instance(Ord,     Int_Gt, Int_Lt, Int_Cmp),
  Instance(Hash,    Int_Hash),
  Instance(C_Int,   Int_C_Int),
  Instance(Show,    Int_Show, Int_Look));

static const char* Float_Name(void) {
  return "Float";
}

static const char* Float_Brief(void) {
  return "Float Point Object";
}

static const char* Float_Description(void) {
  return "64-bit double precision float point Object.";
}

static const char* Float_Examples(void) {
  return ""
    "__Usage__"
    "    "
    "    var f0 = $(Float, 1.0);\n"
    "    var f1 = new$(Float, 24.313);\n"
    "    var f2 = copy(f0);\n"
    "    \n"
    "    show(f0); /* 1.0 */\n"
    "    show(f1); /* 24.313 */\n"
    "    show(f2); /* 1.0 */\n"
    "    \n"
    "    del(f1);\n"
    "    del(f2);\n"
    "    \n"
    "__Maths__\n"
    "    \n"
    "    var f0 = $(Float, 0.0);\n"
    "    \n"
    "    show(f0); /* 0 */\n"
    "    \n"
    "    add(f0, $(Float, 10.1)); /* 10.1 */\n"
    "    mul(f0, $(Float, 3.5));  /* 35.35 */\n"
    "    div(f0, $(Float, 2.0));  /* 17.675 */\n"
    "    \n"
    "    if_gt(f0, $(Float, 11.1)) {\n"
    "        print(\"%f is greater than 11.1!\", i0);\n"
    "    }\n";
}

static const char* Float_Methods(void) {
  return "";
}

static void Float_Assign(var self, var obj) {
  struct Float* f = self;
  f->val = c_float(obj);
}

static double Float_C_Float(var self) {
  struct Float* f = self;
  return f->val;
}

static bool Float_Eq(var self, var obj) {
  return Float_C_Float(self) is c_float(obj);
}

static bool Float_Gt(var self, var obj) {
  return Float_C_Float(self) > c_float(obj);
}

static bool Float_Lt(var self, var obj) {
  return Float_C_Float(self) < c_float(obj);
}

static int Float_Cmp(var self, var obj) {
  return Float_C_Float(self) - c_float(obj);
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
    Float_Name, Float_Brief, Float_Description, Float_Examples, Float_Methods),
  Instance(Assign,  Float_Assign),
  Instance(Eq,      Float_Eq),
  Instance(Ord,     Float_Gt, Float_Lt, Float_Cmp),
  Instance(Hash,    Float_Hash),
  Instance(C_Float, Float_C_Float),
  Instance(Show,    Float_Show, Float_Look));
