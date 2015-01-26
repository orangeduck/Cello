#include "Cello.h"

/* TODO */
static const char* Terminal_Name(void) {
  return "Terminal";
}

/* TODO */
static const char* Terminal_Brief(void) {
  return "";
}

/* TODO */
static const char* Terminal_Description(void) {
  return "";
}

/* TODO */
static const char* Terminal_Examples(void) {
  return "";
}

/* TODO */
static const char* Terminal_Methods(void) {
  return "";
}

var Terminal  = typedecl(Terminal,
  typeclass(Doc,
    Terminal_Name,        Terminal_Brief,
    Terminal_Description, Terminal_Examples,
    Terminal_Methods));
