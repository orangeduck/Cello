#include "Cello.h"

static const char* C_Str_Name(void) {
  return "C_Str";
}

static const char* C_Str_Brief(void) {
  return "Interpret as C String";
}

static const char* C_Str_Description(void) {
  return
    "The `C_Str` class should be overridden by types which are representable "
    "as a C style String.";
}

/* TODO */
static const char* C_Str_Examples(void) {
  return "";
}

/* TODO */
static const char* C_Str_Methods(void) {
  return "";
}

var C_Str = Cello(C_Str,
  Instance(Doc,
    C_Str_Name, C_Str_Brief, C_Str_Description, 
    C_Str_Examples, C_Str_Methods));

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
  return method(self, C_Str, c_str);
}

int64_t c_int(var self) {
  return method(self, C_Int, c_int);
}

double c_float(var self) {
  return method(self, C_Float, c_float);
}

