#include "Cello.h"

static const char* Cast_Name(void) {
  return "Cast";
}

/* TODO */
static const char* Cast_Brief(void) {
  return "";
}

/* TODO */
static const char* Cast_Description(void) {
  return "";
}

/* TODO */
static const char* Cast_Examples(void) {
  return "";
}

/* TODO */
static const char* Cast_Methods(void) {
  return "";
}

var Cast = Cello(Cast, Instance(Doc, 
  Cast_Name, Cast_Brief, Cast_Description, Cast_Examples, Cast_Methods));

var cast(var self, var type) {
  
  struct Cast* c = instance(self, Cast);
  if (c and c->cast) {
    return c->cast(self, type);
  }
  
  if (type_of(self) is type) {
    return self;
  } else {
    return throw(ValueError,
      "cast expected type %s, got type %s", type_of(self), type);
  }
  
}

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

enum {
  CELLO_NBUILTINS = 2
};

static var Type_Alloc(void) {

  struct CelloHeader* head = calloc(1, sizeof(struct CelloHeader));
  
#if CELLO_MEMORY_CHECK == 1
  if (head is NULL) {
    throw(OutOfMemoryError, "Cannot create new 'Type', out of memory!");
  }
#endif
  
  return CelloHeader_Init(head, Type, CelloHeapAlloc);
}

static var Type_New(var self, var args) {
  
  var name = get(args, $I(0));
  var size = get(args, $I(1));
  
  var head = ((char*)self) - sizeof(struct CelloHeader);
  head = realloc(head, 
    sizeof(struct CelloHeader) + 
    sizeof(struct Type) * (CELLO_NBUILTINS + len(args) - 2 + 1));
  
#if CELLO_MEMORY_CHECK == 1
  if (head is NULL) {
    throw(OutOfMemoryError, "Cannot create new 'Type', out of memory!");
  }
#endif
  
  struct Type* body = CelloHeader_Init(head, Type, CelloHeapAlloc);
  
  body[0] = (struct Type){ NULL, "__Name", (var)c_str(name) };
  body[1] = (struct Type){ NULL, "__Size", (var)c_int(size) };
  
  for(size_t i = 2; i < len(args); i++) {
    var ins = get(args, $I(i));
    body[CELLO_NBUILTINS-2+i] = (struct Type){
      NULL, (var)c_str(type_of(ins)), ins };
  }
  
  body[CELLO_NBUILTINS+len(args)-2] = (struct Type){ NULL, NULL, NULL };
  
  return body;
}

static char* Type_Builtin_Name(struct Type* t) {
  return t[0].inst;
}

static size_t Type_Builtin_Size(struct Type* t) {
  return (size_t)t[1].inst;
}

static int Type_Show(var self, var output, int pos) {
  return format_to(output, pos, "%s", Type_Builtin_Name(self));
}

static bool Type_Eq(var self, var obj) {
  return self is obj;
}

/* TODO: Type Hash */

static char* Type_C_Str(var self) {
  return Type_Builtin_Name(self);
}

/* TODO */
static void Type_Help(var self) {
  return;
}

static var Type_Gen(void) {
  return (var[3]) {
    Int, Float, String
  }[gen_c_int() % 3];
}

var Type = CelloEmpty(Type,
  Instance(Doc,
    Type_Name, Type_Brief, Type_Description, Type_Examples, Type_Methods),
  Instance(Alloc,    Type_Alloc, NULL),
  Instance(New,      Type_New, NULL),
  Instance(Eq,       Type_Eq),
  Instance(Show,     Type_Show, NULL),
  Instance(C_Str,    Type_C_Str),
  Instance(Gen,      Type_Gen),
  Instance(Help,     Type_Help));

static var Type_Instance(var self, var cls) {
 
 #if CELLO_METHOD_CHECK == 1
  if (type_of(self) isnt Type) {
    return throw(TypeError, "Method call got non type '%s'", type_of(self));
  }
#endif
  
  struct Type* t;
  
  t = (struct Type*)self + CELLO_NBUILTINS; 
  while (t->name) { if (t->cls is cls) { return t->inst; } t++; }

  t = (struct Type*)self + CELLO_NBUILTINS; 
  while (t->name) {
    if (strcmp(t->name, Type_Builtin_Name(cls)) is 0) {
      t->cls = cls;
      return t->inst;
    }
    t++;
  }
  
  return NULL;
  
}

static bool Type_Implements(var self, var cls) {
  return Type_Instance(self, cls) isnt NULL;
}


bool type_implements(var self, var cls) {
  return Type_Implements(self, cls);
}

static var Type_Method_At_Offset(
  var self, var cls, size_t offset, const char* method_name) {

  var inst = Type_Instance(self, cls);
  
#if CELLO_METHOD_CHECK == 1
  if (inst is NULL) {
    return throw(ClassError,
      "Type '%s' does not implement class '%s'",
      self,  cls);
  }
#endif
  
#if CELLO_METHOD_CHECK == 1  
  var meth = *((var*)(((char*)inst) + offset));

  if (meth is NULL) {
    return throw(ClassError,
      "Type '%s' implements class '%s' but not the method '%s' required",
      self,  cls, $(String, (char*)method_name));  
  }
#endif
  
  return inst;
  
}

var type_method_at_offset(
  var self, var cls, size_t offset, const char* method_name) {
  return Type_Method_At_Offset(self, cls, offset, method_name);  
}

static bool Type_Implements_Method_At_Offset(var self, var cls, size_t offset) {
  var inst = Type_Instance(self, cls);
  if (inst is NULL) { return false; }
  var meth = *((var*)(((char*)inst) + offset));
  if (meth is NULL) { return false; }
  return true;
}

bool type_implements_method_at_offset(var self, var cls, size_t offset) {
  return Type_Implements_Method_At_Offset(self, cls, offset);
}

var type_instance(var self, var cls) {
  return Type_Instance(self, cls);
}
  
static var Type_Of(var self) {

  /*
  **  The type of a Type object is just `Type` again. But because `Type` is 
  **  extern it isn't a constant expression. This means it cannot be set at 
  **  compile time.
  **
  **  But we really want to be able to construct types statically. So by 
  **  convention at compile time the type of a Type object is set to `NULL`.
  **  So if we access a statically allocated object and it tells us `NULL` 
  **  is the type, we assume the type is `Type`.
  */

#if CELLO_NULL_CHECK == 1  
    if (self is NULL) {
      return throw(ValueError, "Received NULL as value to 'type_of'");
    }
#endif
  
  struct CelloHeader* head = 
    (struct CelloHeader*)((char*)self - sizeof(struct CelloHeader));

#if CELLO_MAGIC_CHECK == 1
  if (head->magic isnt ((var)CELLO_MAGIC_NUM)) {
    throw(ValueError, "Pointer '%p' passed to 'type_of' "
      "has bad magic number, it wasn't allocated by Cello.", self);
  }
#endif
    
  if (head->type is NULL) { head->type = Type; }
  
  return head->type;

}
  
var type_of(var self) {
  return Type_Of(self);
}

var instance(var self, var cls) {
  return Type_Instance(Type_Of(self), cls);
}

bool implements(var self, var cls) {  
  return Type_Implements(Type_Of(self), cls);
}

var method_at_offset(
  var self, var cls, size_t offset, const char* method_name) { 
  return Type_Method_At_Offset(Type_Of(self), cls, offset, method_name);
}

bool implements_method_at_offset(var self, var cls, size_t offset) {
  return Type_Implements_Method_At_Offset(Type_Of(self), cls, offset);
}

static const char* Size_Name(void) {
  return "Size";
}

/* TODO */
static const char* Size_Brief(void) {
  return "";
}

/* TODO */
static const char* Size_Description(void) {
  return "";
}

/* TODO */
static const char* Size_Examples(void) {
  return "";
}

/* TODO */
static const char* Size_Methods(void) {
  return "";
}

var Size = Cello(Size, Instance(Doc, 
  Size_Name, Size_Brief, Size_Description, Size_Examples, Size_Methods));

size_t size(var type) {

  struct Size* s = type_instance(type, Size);
  if (s and s->size) {
    return s->size();
  }
  
  size_t builtinsize = Type_Builtin_Size(type);
  
#if CELLO_ALLOC_CHECK == 1
  if (builtinsize is 0) {
    throw(TypeError, "Type %s is an empty type and so has no size.", type);
  }
#endif
  
  return builtinsize;
}
  
