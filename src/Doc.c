#include "Cello.h"

static const char* Doc_Name(void) {
  return "Doc";
}

static const char* Doc_Brief(void) {
  return "Has Documentation";
}

/* TODO: Update? */
static const char* Doc_Description(void) {
  return
    "The `Doc` class can be used to give documentation to a certain class or "
    "type. This documentation can then be accessed using the `help` function "
    "or by other tools used to generate documentation in external formats."
    "\n\n"
    "Documentation is written in Markdown.";
}

static const char* Doc_Definition(void) {
  return
    "struct Example {\n"
    "  const char* name;\n"
    "  const char* body;\n"
    "};\n"
    "\n"
    "struct Method {\n"
    "  const char* name;\n"
    "  const char* definition;\n"
    "  const char* description;\n"
    "};\n"
    "\n"
    "struct Doc {\n"
    "  const char* (*name)(void);\n"
    "  const char* (*brief)(void);\n"
    "  const char* (*description)(void);\n"
    "  const char* (*definition)(void);\n"
    "  struct Example* (*examples)(void);\n"
    "  struct Method* (*methods)(void);\n"
    "};\n";
}

static struct Method* Doc_Methods(void) {
  
  static struct Method methods[] = {
    {
      "name", 
      "const char* name(var type);",
      "Return the name of a given `type`."
    }, {
      "brief", 
      "const char* brief(var type);",
      "Return a brief description of a given `type`."
    }, {
      "description", 
      "const char* description(var type);",
      "Return a longer description of a given `type`."
    }, {
      "definition", 
      "const char* definition(var type);",
      "Return the C definition of a given `type`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

/* TODO: Examples */

var Doc = Cello(Doc,
  Instance(Doc,
    Doc_Name,       Doc_Brief, Doc_Description,
    Doc_Definition, NULL,      Doc_Methods));
    
const char* name(var type) {
  return type_method(type, Doc, name);
}
  
const char* brief(var type) {
  return type_method(type, Doc, brief);
}

const char* description(var type) {
  return type_method(type, Doc, description);
}

const char* definition(var type) {
  return type_method(type, Doc, definition);
}
    
static const char* Help_Name(void) {
  return "Help";
}

static const char* Help_Brief(void) {
  return "Object gives usage information";
}

static const char* Help_Description(void) {
  return
    "The `Help` class can be implemented to let an object provide helpful "
    "information about itself. In the standard library this class is "
    "implemented by `Type` and it prints out the documentation provided "
    "by the `Doc` class in a friendly way.";
}

static const char* Help_Definition(void) {
  return
    "struct Help {\n"
    "  void (*help)(var);\n"
    "};\n";
}

static struct Method* Help_Methods(void) {
  
  static struct Method methods[] = {
    {
      "help", 
      "void help(var self);\n"
      "int help_to(var out, int pos, var self);",
      "Print help information about the object `self` either to `stdout` or "
      "to the object `out` at some position `pos`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

/* TODO: Examples */

var Help = Cello(Help,
  Instance(Doc,
    Help_Name,       Help_Brief, Help_Description,
    Help_Definition, NULL,       Help_Methods));
    
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
    
