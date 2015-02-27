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
  
  if (implements(self, Cast)) {
    return method(self, Cast, cast, type);
  } else {
    if (type_of(self) is type) {
      return self;
    } else {
      return throw(ValueError,
        "cast expected type %s, got type %s", type_of(self), type);
    }
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
  CELLO_NBUILTINS = 5
};

static var Type_Alloc(void) {
  return None;
}

static var Type_New(var self, var args) {
  
  var name = get(args, $I(0));
  var size = get(args, $I(1));
  var head = malloc(
    sizeof(struct CelloHeader) + 
    sizeof(struct Type) * (CELLO_NBUILTINS + len(args) - 2 + 1));
  
  struct Type* body = CelloHeader_Init(head, Type, CelloHeapAlloc);
  
  body[0] = (struct Type){ None, "__Name",      (var)c_str(name), {0} };
  body[1] = (struct Type){ None, "__Size",      (var)c_int(size), {0} };
  body[2] = (struct Type){ None, "__ModMask",   (var)1, {0} };
  body[3] = (struct Type){ None, "__HashMask0", (var)0, {0} };
  body[4] = (struct Type){ None, "__HashMask1", (var)0, {0} };
  
  for(size_t i = 2; i < len(args); i++) {
    var ins = get(args, $I(i));
    body[CELLO_NBUILTINS-2+i] = (struct Type){
      None, (var)c_str(type_of(ins)), ins, {0} };
  }
  
  body[CELLO_NBUILTINS+len(args)-2] = (struct Type){ None, None, None, {0} };
  
  return body;
}

static char* Type_Builtin_Name(struct Type* t) {
  return t[0].inst;
}

static size_t Type_Builtin_Size(struct Type* t) {
  return (size_t)t[1].inst;
}

static uintptr_t* Type_Builtin_ModMask(struct Type* t) {
  return (uintptr_t*)(&t[2].inst);
}

static uintptr_t* Type_Builtin_HashMask0(struct Type* t) {
  return (uintptr_t*)(&t[3].inst);
}

static uintptr_t* Type_Builtin_HashMask1(struct Type* t) {
  return (uintptr_t*)(&t[4].inst);
}

static uint64_t Type_Builtin_Hash(struct Type* t, var cls) {
  uintptr_t m0 = *Type_Builtin_ModMask(t);
  uintptr_t m1 = *Type_Builtin_HashMask0(t);
  uintptr_t m2 = *Type_Builtin_HashMask1(t);
  uintptr_t x = ((uintptr_t)cls)>>sizeof(var);
  x = ((x >> 16) ^ x) * (0x45d9f3b ^ m1);
  x = ((x >> 16) ^ x) * (0x45d9f3b ^ m2);
  return x % m0;
}

static void Type_Builtin_Hash_Inc(struct Type* t) {
  *Type_Builtin_HashMask0(t) = rand();
  *Type_Builtin_HashMask1(t) = rand();
}

static int Type_Show(var self, var output, int pos) {
  return format_to(output, pos, "%s", self);
}

static var Type_Eq(var self, var obj) {
  return bool_var(self is obj);
}

static char* Type_C_Str(var self) {
  return Type_Builtin_Name(self);
}

/* TODO */
static void Type_Help(var self) {
  print("HELP!", NULL);
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
  
static var Type_Build_Hash(struct Type* t) {
  
  size_t i = 0;
  struct Type* ti = t;
  while (ti->name) { i++; ti++; }
  size_t nentries = i - CELLO_NBUILTINS;
  
  srand((unsigned int)(uintptr_t)ti);
  
  *Type_Builtin_ModMask(t) = nentries;
  
  var valid = False;
  var* overlap = malloc(sizeof(var) * nentries);
  
  size_t iterations = 0;
  while (not valid) {
    
    if (iterations > 100000) {
      *Type_Builtin_ModMask(t) = 1;
      *Type_Builtin_HashMask0(t) = 0;
      *Type_Builtin_HashMask1(t) = 0;
      return False;
    }
    
    valid = True;
    memset(overlap, 0, sizeof(var) * nentries);
    
    for (size_t i = 0; i < nentries; i++) {
      var cls = t[CELLO_NBUILTINS+i].cls;
      if (cls is None) { continue; }
      
      uint64_t initial = Type_Builtin_Hash(t, cls);
      
      if (overlap[initial]) {
        valid = False;
        Type_Builtin_Hash_Inc(t);
        break;
      }
      
      overlap[initial] = cls;
    }
    
    iterations++;
  }
  
  for (size_t i = 0; i < nentries; i++) {
    if (overlap[i] is None) { continue; }
    for (size_t j = 0; j < nentries; j++) {
      if (t[CELLO_NBUILTINS+j].cls is overlap[i] and i isnt j) {
        struct Type tmp = t[i+CELLO_NBUILTINS];
        t[i+CELLO_NBUILTINS] = t[j+CELLO_NBUILTINS];
        t[j+CELLO_NBUILTINS] = tmp;
        break;
      }
    }
  }
  
  free(overlap);
  return True;
  
}

static void Type_Scan(
  var self, var cls, size_t offset, var* inst, var* meth) {

  struct Type* t = self;
  
#if CELLO_METHOD_CHECK == 1
  if (type_of(self) isnt Type) {
    throw(TypeError, "Method call got non type '%s'", type_of(self));
    return;
  }
#endif
  
  uint64_t initial = Type_Builtin_Hash(t, cls) + CELLO_NBUILTINS;
  
  if (t[initial].cls is cls) {
    *inst = ((char*)t[initial].blank) + sizeof(struct CelloHeader);
    *meth = *(var*)((char*)(*inst) + offset);
    return;
  }
  
  t += CELLO_NBUILTINS;
  
  while (t->name) {
    
    if (strcmp(t->name, Type_Builtin_Name(cls)) is 0) {
      t->cls = cls;
      CelloHeader_Init((struct CelloHeader*)t->blank, cls, CelloDataAlloc);
      size_t clssize = Type_Builtin_Size(cls);
#if CELLO_ALLOC_CHECK == 1
      if (clssize > CELLO_BLANK_INSTANCE_NUM * 
        sizeof(var) - 
        sizeof(struct CelloHeader)) {
        fprintf(stderr, 
          "Cello Fatal Error: Instance of %s is too big to be copied into "
          "Type %s for caching. Consider splitting it into multiple classes "
          "or increasing the CELLO_BLANK_INSTANCE variable.\n",
          (char*)t->name, 
          (char*)Type_Builtin_Name(self));
        abort();
      }
#endif
      memcpy(((char*)t->blank) + sizeof(struct CelloHeader), t->inst, clssize);
      if (Type_Build_Hash(self)) {
        Type_Scan(self, cls, offset, inst, meth);
      } else {
        *inst = t->inst;
		    *meth = *(var*)((char*)(*inst) + offset);
      }
      return;
    }
    
    t++;
  }
  
  *inst = None;
  *meth = None;
  
}

static var Type_Implements(var self, var cls) {
  var inst = None, meth = None;
  Type_Scan(self, cls, 0, &inst, &meth);
  return bool_var(inst isnt None);
}


var type_implements(var self, var cls) {
  return Type_Implements(self, cls);
}

static var Type_Method_At_Offset(
  var self, var cls, size_t offset, const char* method_name) {

  var inst = None, meth = None;
  Type_Scan(self, cls, offset, &inst, &meth);
  
#if CELLO_METHOD_CHECK == 1
  if (inst is None) {
    return throw(ClassError,
      "Type '%s' does not implement class '%s'",
      self,  cls);
  }
  
  if (meth is None) {
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

static var Type_Implements_Method_At_Offset(
  var self, var cls, size_t offset) {
  var inst = None, meth = None;
  Type_Scan(self, cls, offset, &inst, &meth);
  return bool_var(inst and meth);
}

var type_implements_method_at_offset(var self, var cls, size_t offset) {
  return Type_Implements_Method_At_Offset(self, cls, offset);
}

static var Type_Instance(var self, var cls) {

  var inst = None, meth = None;
  Type_Scan(self, cls, 0, &inst, &meth);
  
#if CELLO_METHOD_CHECK == 1
  if (inst is None) {  
    return throw(ClassError,
      "Type '%s' does not implement class '%s'", self, cls);
  }
#endif
  
  return inst;

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
  **  convention at compile time the type of a Type object is set to `None`.
  **  So if we access a statically allocated object and it tells us `None` 
  **  is the type, we assume the type is `Type`.
  */

  struct CelloHeader* head;
  
  switch ((uintptr_t)self) {
    case 0: return Bool;
    case 1: return Bool;
    default:
    
      head = (struct CelloHeader*)((char*)self - sizeof(struct CelloHeader));
    
#if CELLO_UNDEF_CHECK == 1  
      if (self is Undefined) {
        return throw(ValueError, "Received 'Undefined' as value to 'type_of'");
      }
#endif

#if CELLO_MAGIC_CHECK == 1
      if (head isnt False 
      and head isnt True
      and head->magic isnt ((var)0xCe110)) {
        throw(ValueError, "Pointer '%p' passed to 'type_of' "
          "has bad magic number, it wasn't allocated by Cello.\n", self);
      }
#endif
    
    return head->type is None ? Type : head->type;
  }

}
  
var type_of(var self) {
  return Type_Of(self);
}

var instance(var self, var cls) {
  return Type_Instance(Type_Of(self), cls);
}

var implements(var self, var cls) {  
  return Type_Implements(Type_Of(self), cls);
}

var method_at_offset(
  var self, var cls, size_t offset, const char* method_name) { 
  return Type_Method_At_Offset(Type_Of(self), cls, offset, method_name);
}

var implements_method_at_offset(var self, var cls, size_t offset) {
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
  if (type_implements(type, Size)) {
    return type_method(type, Size, size);
  }
  
  size_t builtinsize = Type_Builtin_Size(type);
  
#if CELLO_ALLOC_CHECK == 1
  if (builtinsize is 0) {
    throw(TypeError, "Type %s is an empty type and so has no size.", type);
  }
#endif
  
  return builtinsize;
}
  
