#include "Cello.h"

static var __Name   = typedecl(__Name);
static var __Parent = typedecl(__Parent);

static const char* Type_Name(void) {
  return "Type";
}

static const char* Type_Brief(void) {
  return "Metadata Object";
}

static const char* Type_Description(void) {
  return ""
    "`Type` is one of the most important Types in Cello. It provides metadata "
    "about an object including which classes it implements. One can get the "
    "type of an object using the `type_of` function."
    "\n\n"
    "To see if an object implements a class `implements` can be used. To "
    "call a member of a class with an object `method` can be used."
    "\n\n"
    "To see if a type implements a class `type_implements` can be used. To "
    "call a member of a class, implemented `type_method` can be used."
    "\n\n"
    "`cast` can be used to do runtime type checking. It checks a given object "
    "has a certain type and if so returns the given object, otherwise throws"
    "an exception.";
}

/* TODO: Example construction on heap */
/* TODO: Example construction statically */
/* TODO: Example using object functions */
static const char* Type_Examples(void) {
  return ""
    "__Usage__\n"
    "    \n"
    "    var t = type_of($(Int, 5));\n"
    "    show(t); /* Int */\n"
    "    \n"
    "    show(type_implements(t, New));  /* True  */\n"
    "    show(type_implements(t, Eq));   /* True  */\n"
    "    show(type_implements(t, Ord));  /* True  */\n"
    "    show(type_implements(t, Push)); /* False */\n"
    "    \n"
    "    show(type_method(t, Eq, eq, $(Int, 5), $(Int, 6))); /* False */\n"
    "    \n"
    "    \n"
    "__Casting__\n"
    "    \n"
    "    var x = $(Int, 10);\n"
    "    \n"
    "    x = cast(x, Int); /* Success */\n"
    "    x = cast(x, Real); /* Throws Exception */\n";
}

/* TODO: Finish */
static const char* Type_Methods(void) {
  return ""
    "\n"
    "    var type_of(var self);\n"
    "\n"
    "__Arguments__\n"
    "* `self` object to find type of\n"
    "\n"
    "__Returns__\n"
    "* Type of object `self`\n"
    "\n"
    "    var cast(var self, var type);\n"
    "__Arguments__\n"
    "* `self` object to check type of\n"
    "* `type` type to assert object is\n"
    "\n"
    "__Returns__\n"
    "* `self` if type is correct\n";
}

static var Type_New(var self, var args) {
  
  var name = get(args, $(Int, 0));
  
  struct CelloHeader* head = malloc(
    sizeof(struct CelloHeader) + 
    sizeof(struct Type) * (2 + len(args) - 1));
  
  head->type  = Type;
  head->flags = (var)CelloHeapAlloc;
  
  struct Type* body = (var)head + sizeof(struct CelloHeader);
  
  body[0] = (struct Type){ __Name, "__Name", (var)c_str(name) };
  body[1] = (struct Type){ __Parent, "__Parent", None };
  
  for(size_t i = 1; i < len(args); i++) {
    var ins = get(args, $I(i));
    body[1+i] = (struct Type){
      type_of(ins), 
      (var)c_str(type_of(ins)), 
      ins };
  }
  
  return body;
}

static var Type_Del(var self) {
  return self;
}

static size_t Type_Size(void) {
  return 0;
}

static var Type_Parent(var self) {

  struct Type* t = self;

  while(t->name) {
    if (t->cls is __Parent) { return t->inst; }
    if (t->cls is None and (strcmp(t->name, "__Parent") is 0)) {
      t->cls = __Parent;
      return t->inst;
    }
    t++;
  }
  
  return throw(ClassError,
    "Cannot find Class '__Parent' for object '%p' :: "
    "Was is correctly constructed?", self);
  
}

char* Type_C_Str(var self) {

  struct Type* t = self;
  
  while(t->name) {
    if (t->cls is __Name) { return t->inst; }
    if (t->cls is None and (strcmp(t->name, "__Name") is 0)) {
      t->cls = __Name;
      return (char*)t->inst;
    }
    t++;
  }
  
  return throw(ClassError,
    "Cannot find Class '__Name' for object '%p' :: "
    "Was is correctly constructed?", self);

}

var type_implements(var self, var cls) {
  
  cls = cast_exact(cls, Type);
  char* cls_name = Type_C_Str(cls);
  
  struct Type* t = self;
  while (t->name) {
    if (t->cls is cls) { return True; }
    if (t->cls is None and (strcmp(t->name, cls_name) is 0)) {
      t->cls = cls;
      return True;
    }
    t++;
  }
  
  var parent = Type_Parent(self);
  return bool_var(parent isnt None and type_implements(parent, cls));
  
}

var type_method_at_offset(
  var self, var cls, size_t offset, const char* method_name) {
  
  cls = cast_exact(cls, Type);
  char* cls_name = Type_C_Str(cls);
  var inst = None;
  
  struct Type* t = self;
  while (t->name) {
    if (t->cls is cls) { inst = t->inst; break; }
    if (t->cls is None and (strcmp(t->name, cls_name) is 0)) {
      t->cls = cls;
      inst = t->inst;
      break;
    }
    t++;
  }
  
  if (inst isnt None 
  and (*(var*)(inst + offset)) isnt None) {
    return inst;
  }
  
  var parent = Type_Parent(self);
  if (parent isnt None) {
    return type_method_at_offset(parent, cls, offset, method_name);
  }
  
  if (inst is None) {
    return throw(ClassError,
      "Type '%s' does not implement class '%s'",
      self,  cls, $(String, (char*)method_name));
  } else {
    return throw(ClassError,
      "Type '%s' implements class '%s' but not the method '%s' required",
      self,  cls, $(String, (char*)method_name));  
  }
  
}

var type_implements_method_at_offset(
  var self, var cls, size_t offset, const char* method_name) {

  cls = cast_exact(cls, Type);
  char* cls_name = Type_C_Str(cls);
  var inst = None;
  
  struct Type* t = self;
  while (t->name) {
    if (t->cls is cls) { inst = t->inst; break; }
    if (t->cls is None and (strcmp(t->name, cls_name) is 0)) {
      t->cls = cls;
      inst = t->inst;
      break;
    }
    t++;
  }
  
  if (inst isnt None 
  and (*(var*)(inst + offset)) isnt None) {
    return True;
  }
  
  var parent = Type_Parent(self);
  return bool_var(parent isnt None and 
      type_implements_method_at_offset(parent, cls, offset, method_name));
  
}

var type_instance(var self, var cls) {
  
  cls = cast_exact(cls, Type);
  char* cls_name = Type_C_Str(cls);
  
  struct Type* t = self;
  while(t->name) {
    if (t->cls is cls) { return t->inst; }
    if (t->cls is None and (strcmp(t->name, cls_name) is 0)) {
      t->cls = cls;
      return t->inst;
    }
    t++;
  }
  
  var parent = Type_Parent(self);
  if (parent isnt None and type_implements(parent, cls)) {
    return type_instance(parent, cls);
  }
  
  return throw(ClassError,
    "Type '%s' does not implement class '%s'", self, cls);
      
}

static int Type_Show(var self, var output, int pos) {
  return print_to(output, pos, "%s", self);
}

static void Type_Inherit(var self, var parent) {
  
  struct Type* t = self;

  while(t->name) {
    if (strcmp(t->name, "__Parent") == 0) {
      t->cls = parent;
      return;
    }
    t++;
  }
  
  throw(ClassError,
    "Cannot find Class '__Parent' for object '%p' :: "
    "Was is correctly constructed?", self);
  
}

/* TODO */
static void Type_Help(var self) {
  print("HELP!");
}

var Type = typedecl(Type,
  typeclass(Doc,
    Type_Name, Type_Brief, Type_Description, Type_Examples, Type_Methods),
  typeclass(New,      Type_New, Type_Del, Type_Size),
  typeclass(C_Str,    Type_C_Str),
  typeclass(Show,     Type_Show, NULL),
  typeclass(Help,     Type_Help));
  
var type_of(var self) {
  
  /*
  **  The type of a Type object is just `Type` again. But because `Type` is 
  **  extern it isn't a constant expression. This means it cannot be set at 
  **  compile time.
  **
  **  But we really want to be able to construct types statically. So by 
  **  convention at compile time the type of a Type object is set to `None`.
  **  So if we access a statically allocated object and it tells us `None` 
  **  is the type, we assume the type is `Type`.
  */
  
  if (self is Undefined) {
    return throw(ValueError, "Received 'Undefined' as value to 'type_of'");
  }
  
  if (self is True)  { return Bool; }
  if (self is False) { return Bool; }

  struct CelloHeader* head = self - sizeof(struct CelloHeader);
  
  if (head->type is None and 
  ((int)(intptr_t)head->flags) & CelloStaticAlloc) {
    return Type;
  } else {
    return head->type;
  }

}

var cast(var self, var type) {
  
  var atype = type_of(self);
  
  do {
    if (atype is type) { return self; }
    atype = Type_Parent(atype);
  } while (atype isnt None);
  
  if (implements(self, Pointer)) {
    return cast(deref(self), type);
  }
  
  return throw(TypeError,
    "Cast Got Type '%s' :: Expected Type '%s'", type_of(self), type);
  
}

var cast_exact(var self, var type) {
  
  if (type_of(self) is type) {
    return self; 
  }
  
  return throw(TypeError,
    "Exact Cast Got Type '%s' :: Expected Type '%s'",
    type_of(self), type);
}

var instance(var self, var cls) {
  return type_instance(type_of(self), cls);
}

var implements(var self, var cls) {  
  return type_implements(type_of(self), cls);
}

var method_at_offset(
  var self, var cls, size_t offset, const char* method_name) { 
  return type_method_at_offset(type_of(self), cls, offset, method_name);
}

var implements_method_at_offset(
  var self, var cls, size_t offset, const char* method_name) {
  return type_implements_method_at_offset(
    type_of(self), cls, offset, method_name);
}
  
