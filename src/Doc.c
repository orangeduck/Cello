#include "Cello.h"

static const char* Doc_Name(void) {
  return "Doc";
}

static const char* Doc_Brief(void) {
  return "Provides Documentation";
}

static const char* Doc_Description(void) {
  return
    "The `Doc` class can be used to give documentation to a certain class or "
    "type. This documentation can then be accessed using the `help` function "
    "or by other tools used to generate documentation such as for the Cello "
    "website. Documentation can be written in Markdown."
    "\n\n"
    "The `examples` and `methods` entries should be provided as `NULL` "
    "terminated arrays allocated statically." ;
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

static struct Example* Doc_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "show($S(name(Int))); /* Int */\n"
      "show($S(brief(Int))); /* Integer Object */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

var Doc = Cello(Doc,
  Instance(Doc,
    Doc_Name,       Doc_Brief,    Doc_Description,
    Doc_Definition, Doc_Examples, Doc_Methods));
    
const char* name(var type) {
  struct Doc* doc = type_instance(type, Doc);
  if (doc->name) { return doc->name(); }
  return c_str(type);
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
  return "Usage information";
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
    "  int (*help_to)(var, int);\n"
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

static struct Example* Help_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "help(Int);\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

var Help = Cello(Help,
  Instance(Doc,
    Help_Name,       Help_Brief,    Help_Description,
    Help_Definition, Help_Examples, Help_Methods));

int help_to(var out, int pos, var self) {
  return method(self, Help, help_to, out, pos);
}

void help(var self) {
  help_to($(File, stdout), 0, self);
}
    
