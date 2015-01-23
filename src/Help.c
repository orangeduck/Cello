#include "Cello.h"

const char* Doc_Name(void) {
  return "Doc";
}

/* TODO */
const char* Doc_Brief(void) {
  return "";
}

/* TODO */
const char* Doc_Description(void) {

}

/* TODO */
const char* Doc_Examples(void) {

}

/* TODO */
const char* Doc_Methods(void) {

}

var Doc = typedecl(Doc,
  typeclass(Doc,
    Doc_Name, Doc_Brief, Doc_Description,
    Doc_Examples, Doc_Methods));
    
const char* Help_Name(void) {
  return "Help";
}

/* TODO */
const char* Help_Brief(void) {
  return "";
}

/* TODO */
const char* Help_Description(void) {

}

/* TODO */
const char* Help_Examples(void) {

}

/* TODO */
const char* Help_Methods(void) {

}

var Help = typedecl(Help
  typeclass(Doc,
    Help_Name, Help_Brief, Help_Description,
    Help_Examples, Help_Methods));
    