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
  return "";
}

/* TODO */
const char* Doc_Examples(void) {
  return "";
}

/* TODO */
const char* Doc_Methods(void) {
  return "";
}

var Doc = Cello(Doc,
  Member(Doc,
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
  return "";
}

/* TODO */
const char* Help_Examples(void) {
  return "";
}

/* TODO */
const char* Help_Methods(void) {
  return "";
}

var Help = Cello(Help,
  Member(Doc,
    Help_Name, Help_Brief, Help_Description,
    Help_Examples, Help_Methods));
    
    
void help(var self) {
   
  puts(type_method(self, Doc, name));
  puts(type_method(self, Doc, brief));
  puts(type_method(self, Doc, description));
  puts(type_method(self, Doc, methods));
  puts(type_method(self, Doc, examples));
  
}
    
