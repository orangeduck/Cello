#include "Cello.h"

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

static var Int_New(var self, var args) {
  struct Int* i = self;
  i->val = c_int(get(args, $(Int, 0)));
  return self;
}

static var Int_Del(var self) {
  return self;
}

static size_t Int_Size(void) {
  return sizeof(struct Int);
}

static var Int_Assign(var self, var obj) {
  struct Int* i = self;
  i->val = c_int(obj);
  return self;
}

static var Int_Copy(var self) {
  return new(Int, self);
}

static int64_t Int_C_Int(var self) {
  struct Int* i = self;
  return i->val;
}

static var Int_Eq(var self, var obj) {
  return bool_var(Int_C_Int(self) is c_int(obj));
}

static var Int_Gt(var self, var other) {
  return bool_var(Int_C_Int(self) > c_int(other));
}

static var Int_Lt(var self, var other) {
  return bool_var(Int_C_Int(self) < c_int(other));
}

static uint64_t Int_Hash(var self) {
  return (uint64_t)c_int(self);
}

static void Int_Add(var self, var other) {
  struct Int* i = self;
  i->val += c_int(other);
}

static void Int_Sub(var self, var other) {
  struct Int* i = self;
  i->val -= c_int(other);
}

static void Int_Mul(var self, var other) {
  struct Int* i = self;
  i->val *= c_int(other);
}

static void Int_Div(var self, var other) {
  struct Int* i = self;
  i->val /= c_int(other);
}

static void Int_Pow(var self, var other) {
  struct Int* i = self;
  i->val = pow(i->val, c_int(other));
}

static void Int_Mod(var self, var other) {
  struct Int* i = self;
  i->val %= c_int(other);
}

static void Int_Neg(var self) {
  struct Int* i = self;
  i->val = -i->val;
}

static void Int_Abs(var self) {
  struct Int* i = self;
 i->val = abs(i->val);
}

static void Int_Exp(var self) {
  struct Int* i = self;
 i->val = exp(i->val);
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
  Member(Doc,
    Int_Name, Int_Brief, Int_Description, Int_Examples, Int_Methods),
  Member(New,     Int_New, Int_Del, Int_Size),
  Member(Assign,  Int_Assign),
  Member(Copy,    Int_Copy),
  Member(Eq,      Int_Eq),
  Member(Ord,     Int_Gt, Int_Lt),
  Member(Hash,    Int_Hash),
  Member(C_Int,   Int_C_Int),
  Member(Math,
    Int_Add, Int_Sub, Int_Mul, Int_Div, Int_Pow, 
    Int_Mod, Int_Neg, Int_Abs, Int_Exp),
  Member(Show, Int_Show, Int_Look));

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

static var Float_New(var self, var args) {
  struct Float* f = self;
  f->val = c_float(get(args, $(Int, 0)));
  return self;
}

static var Float_Del(var self) {
  return self;
}

static size_t Float_Size(void) {
  return sizeof(struct Float);
}

static var Float_Assign(var self, var obj) {
  struct Float* f = self;
  f->val = c_float(obj);
  return self;
}

static var Float_Copy(var self) {
  return new(Float, self);
}

static double Float_C_Float(var self) {
  struct Float* f = self;
  return f->val;
}

static var Float_Eq(var self, var obj) {
  return bool_var(Float_C_Float(self) is c_float(obj));
}

static var Float_Gt(var self, var obj) {
  return bool_var(Float_C_Float(self) > c_float(obj));
}

static var Float_Lt(var self, var obj) {
  return bool_var(Float_C_Float(self) < c_float(obj));
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

static void Float_Add(var self, var other) {
  struct Float* f = self;
  f->val += c_float(other);
}

static void Float_Sub(var self, var other) {
  struct Float* f = self;
  f->val -= c_float(other);
}

static void Float_Mul(var self, var other) {
  struct Float* f = self;
  f->val *= c_float(other);
}

static void Float_Div(var self, var other) {
  struct Float* f = self;
  f->val /= c_float(other);
}

static void Float_Pow(var self, var other) {
  struct Float* f = self;
  f->val = pow(f->val, c_float(other));
}

static void Float_Mod(var self, var other) {
  struct Float* f = self;
  f->val = fmod(f->val, c_float(other));
}

static void Float_Neg(var self) {
  struct Float* f = self;
  f->val = -f->val;
}

static void Float_Abs(var self) {
  struct Float* f = self;
  f->val = fabs(f->val);
}

static void Float_Exp(var self) {
  struct Float* f = self;
  f->val = exp(f->val);
}

int Float_Show(var self, var output, int pos) {
  return print_to(output, pos, "%f", self);
}

int Float_Look(var self, var input, int pos) {
  return scan_from(input, pos, "%f", self);
}

var Float = Cello(Float,
  Member(Doc,
    Float_Name, Float_Brief, Float_Description, Float_Examples, Float_Methods),
  Member(New,     Float_New, Float_Del, Float_Size),
  Member(Assign,  Float_Assign),
  Member(Copy,    Float_Copy),
  Member(Eq,      Float_Eq),
  Member(Ord,     Float_Gt, Float_Lt),
  Member(Hash,    Float_Hash),
  Member(C_Float, Float_C_Float),
  Member(Math,
    Float_Add, Float_Sub, Float_Mul, Float_Div, Float_Pow,
    Float_Mod, Float_Neg, Float_Abs, Float_Exp),
  Member(Show, Float_Show, Float_Look));


static const char* Math_Name(void) {
  return "Math";
}

/* TODO */
static const char* Math_Brief(void) {
  return "";
}

/* TODO */
static const char* Math_Description(void) {
  return "";
}

/* TODO */
static const char* Math_Examples(void) {
  return "";
}

/* TODO */
static const char* Math_Methods(void) {
  return "";
}

var Math = Cello(Math,
  Member(Doc,
    Math_Name, Math_Brief, Math_Description, Math_Examples, Math_Methods));

void madd(var self, var obj) { method(self, Math, madd, obj); }
void msub(var self, var obj) { method(self, Math, msub, obj); }
void mmul(var self, var obj) { method(self, Math, mmul, obj); }
void mdiv(var self, var obj) { method(self, Math, mdiv, obj); }
void mpow(var self, var obj) { method(self, Math, mpow, obj); }
void mmod(var self, var obj) { method(self, Math, mmod, obj); }
void mneg(var self) { method(self, Math, mneg); }
void mabs(var self) { method(self, Math, mabs); }
void mexp(var self) { method(self, Math, mexp); }

void minc(var self) { madd(self, $I(1)); }
void mdec(var self) { msub(self, $I(1)); }

