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

var Cast = Cello(Cast, Member(Doc, 
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

static var __Name = Cello(__Name);
static var __Size = Cello(__Size);
static var __ModMask = Cello(__ModMask);
static var __HashMask0 = Cello(__HashMask0);
static var __HashMask1 = Cello(__HashMask1);

static var Type_New(var self, var args) {
  
  var name = get(args, $I(0));
  var size = get(args, $I(1));
  var head = malloc(
    sizeof(struct CelloHeader) + 
    sizeof(struct Type) * (CELLO_NBUILTINS + len(args) - 2 + 1));
  
  struct Type* body = CelloHeader_Init(head, Type, CelloHeapAlloc);
  
  body[0] = (struct Type){ __Name, "__Name", (var)c_str(name) };
  body[1] = (struct Type){ __Size, "__Size", (var)c_int(size) };
  body[2] = (struct Type){ __ModMask, "__ModMask", (var)1 };
  body[3] = (struct Type){ __HashMask0, "__HashMask0", (var)0 };
  body[4] = (struct Type){ __HashMask1, "__HashMask1", (var)0 };
  
  for(size_t i = 2; i < len(args); i++) {
    var ins = get(args, $I(i));
    body[CELLO_NBUILTINS-2+i] = (struct Type){
      type_of(ins), 
      (var)c_str(type_of(ins)), 
      ins };
  }
  
  body[CELLO_NBUILTINS+len(args)-2] = (struct Type){ None, None, None };
  
  return body;
}

static var Type_Del(var self) {
  return self;
}

static size_t Type_Size(void) {
  return 0;
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

static char* Type_C_Str(var self) {
  return Type_Builtin_Name(self);
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
  //*Type_Builtin_HashMask0(t) += 1;
  //if (*Type_Builtin_HashMask0(t) == sizeof(var)*8) {
  //  *Type_Builtin_HashMask0(t) = 0;
  //  *Type_Builtin_HashMask1(t) += 1;
  //  if (*Type_Builtin_HashMask1(t) == sizeof(var) * 8) {
  //    *Type_Builtin_HashMask1(t) = 0;
  //  }
  //}
  
  *Type_Builtin_HashMask0(t) = rand();
  *Type_Builtin_HashMask1(t) = rand();
}

static void Type_Debug(struct Type* t) {
  
  printf("{ Type %s }\n", Type_Builtin_Name(t));
  while (t->name) {
    printf("{ %p, %s, %p }\n", t->cls, (char*)t->name, t->inst);
    t++;
  } 
  printf("{ End }\n");
  
}

static var Type_Build_Hash(struct Type* t) {
  
  //printf("Building Hash for %s\n", Type_Builtin_Name(t));
  
  //Type_Debug(t);
    
  size_t i = 0;
  struct Type* ti = t;
  while (ti->name) { i++; ti++; }
  size_t nentries = i - CELLO_NBUILTINS;
  
  srand((uintptr_t)ti);
  
  //printf("Nentries: %li\n", nentries);
  
  *Type_Builtin_ModMask(t) = nentries;
  
  var valid = False;
  var overlap[nentries];
  
  /* Find Valid Mask */
  
  size_t iterations = 0;
  while (not valid) {
    
    if (iterations > 100000) {
      *Type_Builtin_ModMask(t) = 1;
      *Type_Builtin_HashMask0(t) = 0;
      *Type_Builtin_HashMask1(t) = 0;
      return False;
    }
    
    //printf("Trying Masks %li %li\n", 
    //  *Type_Builtin_HashMask0(t),
    //  *Type_Builtin_HashMask1(t));
    
    valid = True;
    memset(overlap, 0, sizeof(var) * nentries);
    
    for (size_t i = 0; i < nentries; i++) {
      var cls = t[CELLO_NBUILTINS+i].cls;
      if (cls is None) { continue; }
      
      size_t initial = Type_Builtin_Hash(t, cls);
      
      //printf("Fitting Class: %s %p - %li\n", Type_Builtin_Name(cls), cls, initial);
      
      if (overlap[initial]) {
        valid = False;
        Type_Builtin_Hash_Inc(t);
        break;
      }
      
      overlap[initial] = cls;
    }
    
    iterations++;
  }
  
  //printf("Success in %li iterations: %li %li\n",
  //  iterations, 
  //  *Type_Builtin_HashMask0(t),
  //  *Type_Builtin_HashMask1(t));
  
  //Type_Debug(t);
  
  /* Swap Entries */
  
  for (size_t i = 0; i < nentries; i++) {
    if (overlap[i] is None) { continue; }
    for (size_t j = 0; j < nentries; j++) {
      if (t[CELLO_NBUILTINS+j].cls is overlap[i] and i isnt j) {
        //printf("Swapping Entries %li and %li\n", i, j);
        struct Type tmp = t[i+CELLO_NBUILTINS];
        t[i+CELLO_NBUILTINS] = t[j+CELLO_NBUILTINS];
        t[j+CELLO_NBUILTINS] = tmp;
        break;
      }
    }
  }
  
  //Type_Debug(t);
  
  return True;
  
}

static inline void Type_Scan(
  var self, var cls, size_t offset, var* inst, var* meth) {

  struct Type* t = self;
  
#if CELLO_METHOD_CHECK == 1
  if (type_of(self) isnt Type) {
    throw(TypeError, "Method call got non type '%s'", type_of(self));
    return;
  }
#endif
  
  /* Fast Path */
  
  size_t initial = Type_Builtin_Hash(t, cls) + CELLO_NBUILTINS;
  
  if (t[initial].cls is cls) {
    *inst = t[initial].inst;
    *meth = *(var*)(t[initial].inst + offset);
    return;
  }
  
  /* Slow Path */
  
  t += CELLO_NBUILTINS;
  
  while (t->name) {
    
    if (strcmp(t->name, Type_Builtin_Name(cls)) is 0) {
      t->cls = cls;
      if (Type_Build_Hash(self)) {
        Type_Scan(self, cls, offset, inst, meth);
      } else {
        *inst = t->inst;
        *meth = *(var*)(t->inst + offset);
      }
      return;
    }
    
    t++;
  }
  
  *inst = None;
  *meth = None;
  
}

static inline var Type_Implements(var self, var cls) {
  var inst = None, meth = None;
  Type_Scan(self, cls, 0, &inst, &meth);
  return bool_var(inst isnt None);
}


var type_implements(var self, var cls) {
  return Type_Implements(self, cls);
}

static inline var Type_Method_At_Offset(
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

static inline var Type_Implements_Method_At_Offset(
  var self, var cls, size_t offset) {
  var inst = None, meth = None;
  Type_Scan(self, cls, offset, &inst, &meth);
  return bool_var(inst and meth);
}

var type_implements_method_at_offset(var self, var cls, size_t offset) {
  return Type_Implements_Method_At_Offset(self, cls, offset);
}

static inline var Type_Instance(var self, var cls) {

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

static int Type_Show(var self, var output, int pos) {
  return format_to(output, pos, "%s", Type_C_Str(self));
}

static var Type_Eq(var self, var obj) {
  return bool_var(self is obj);
}

/* TODO */
static void Type_Help(var self) {
  print("HELP!");
}

var Type = Cello(Type,
  Member(Doc,
    Type_Name, Type_Brief, Type_Description, Type_Examples, Type_Methods),
  Member(New,      Type_New, Type_Del, Type_Size),
  Member(C_Str,    Type_C_Str),
  Member(Eq,       Type_Eq),
  Member(Show,     Type_Show, NULL),
  Member(Help,     Type_Help));
  
static inline var Type_Of(var self) {

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
  
  switch ((intptr_t)self) {
    case 0: return Bool;
    case 1: return Bool;
    default:
    
      head = self - sizeof(struct CelloHeader);
    
#if CELLO_UNDEF_CHECK == 1  
      if (self is Undefined) {
        return throw(ValueError, "Received 'Undefined' as value to 'type_of'");
      }
#endif

#if CELLO_MAGIC_CHECK == 1
      if (head isnt False 
      and head isnt True
      and head->magic isnt ((var)0xCe110)) {
        fprintf(stderr,
          "Cello Fatal Error: Pointer '%p' passed to 'type_of' "
          "has bad magic number - it wasn't allocated by Cello.\n", self);
        abort();
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
  
