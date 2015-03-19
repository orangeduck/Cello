#include "Cello.h"

const char* Doc_Name(void) {
  return "Doc";
}

const char* Doc_Brief(void) {
  return "Has Documentation";
}

/* TODO: Update? */
const char* Doc_Description(void) {
  return
    "The `Doc` class can be used to give documentation to a certain class or "
    "type. This documentation can then be accessed using the `help` function "
    "or by other tools used to generate documentation in external formats."
    "\n\n"
    "Documentation is written in Markdown.";
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
  Instance(Doc,
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
  Instance(Doc,
    Help_Name, Help_Brief, Help_Description,
    Help_Examples, Help_Methods));
    
static int Help_Title(var out, int pos, int length, char type) {
  int spos = pos;
  for(int i = 0; i < length; i++) { pos += print_to(out, pos, "%c", $I(type)); }
  pos += print_to(out, pos, "\n\n");
  return spos - pos;
}

/* TODO: This should be specific to the type instance of Help */

int help_to(var out, int pos, var self) {
  
  int spos = pos;
  
  pos += print_to(out, pos, "\n%s - %s\n", 
    $S((char*)type_method(self, Doc, name)),
    $S((char*)type_method(self, Doc, brief)));
    
  pos += Help_Title(out, pos,
    strlen(type_method(self, Doc, name)) +
    strlen(type_method(self, Doc, brief)) + 3, '=');
  
  pos += print_to(out, pos, "Description\n");
  pos += Help_Title(out, pos, strlen("Description"), '-');
  pos += print_to(out, pos, "%s", $S((char*)type_method(self, Doc, description)));
  
  pos += print_to(out, pos, "\n\nMethods\n");
  pos += Help_Title(out, pos, strlen("Methods"), '-');
  pos += print_to(out, pos, "%s", $S((char*)type_method(self, Doc, methods)));
   
  pos += print_to(out, pos, "\n\nExamples\n");
  pos += Help_Title(out, pos, strlen("Examples"), '-');
  pos += print_to(out, pos, "%s", $S((char*)type_method(self, Doc, examples)));
  
  return pos - spos;
}

void help(var self) {
  help_to($(File, stdout), 0, self);
}
    
