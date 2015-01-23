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

var Terminal  = type(Terminal
  type_class(Document,
    Terminal_Name,        Terminal_Brief,
    Terminal_Description, Terminal_Examples));