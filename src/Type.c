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

static var __Name   = Cello(__Name);
static var __Parent = Cello(__Parent);

static var Type_New(var self, var args) {
  
  var name = get(args, $I(0));
  var head = malloc(
    sizeof(struct CelloHeader) + 
    sizeof(struct Type) * (2 + len(args) - 1 + 1));
  
  struct Type* body = CelloHeader_Init(head, Type, CelloHeapAlloc);
  
  body[0] = (struct Type){ __Name, "__Name", (var)c_str(name) };
  body[1] = (struct Type){ __Parent, "__Parent", None };
  
  for(size_t i = 1; i < len(args); i++) {
    var ins = get(args, $I(i));
    body[1+i] = (struct Type){
      type_of(ins), 
      (var)c_str(type_of(ins)), 
      ins };
  }
  
  body[2+len(args)-1] = (struct Type){ None, None, None };
  
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
  return t[1].inst;
}

char* Type_C_Str(var self) {
  struct Type* t = self;
  return t[0].inst;
}

struct Type_Cache_Entry {
  var type;
  var cls;
  var inst;
};

enum {
  TYPE_CACHE_SIZE = 5
};

static int Type_Cache_Id = 0;
static struct Type_Cache_Entry Type_Cache[TYPE_CACHE_SIZE];

void Type_Scan(var self, var cls, size_t offset, var* inst, var* meth) {
  
#if CELLO_METHOD_CHECK == 1
  if (type_of(self) isnt Type) {
    throw(TypeError, "Method call got non type '%s'", type_of(self));
    return;
  }
#endif
  
  /* Search Cache */
  
  for (int i = 0; i < TYPE_CACHE_SIZE; i++) {
    int id = (Type_Cache_Id + i) % TYPE_CACHE_SIZE;
    struct Type_Cache_Entry te = Type_Cache[id];
    if (te.type is self
    and te.cls  is  cls) {
      *inst = te.inst;
      *meth = *(var*)((te.inst) + offset);
      return;
    }
  }
  
  /* Search Type Entry */
  
  char* cls_name = Type_C_Str(cls);
  
  struct Type* t = self;
  while (t->name) {
    
    if (t->cls is cls) {
      *inst =  t->inst;
      *meth = *(var*)((t->inst) + offset);
      
      /* Add to cache */      
      Type_Cache_Id--;
      Type_Cache_Id = Type_Cache_Id == -1 ? TYPE_CACHE_SIZE-1 : Type_Cache_Id;
      Type_Cache[Type_Cache_Id].type = self;
      Type_Cache[Type_Cache_Id].cls  =  cls;
      Type_Cache[Type_Cache_Id].inst = t->inst;
      
      return;
    }
    
    if (t->cls is None and (strcmp(t->name, cls_name) is 0)) {
      t->cls = cls;
      *inst =  t->inst;
      *meth = *(var*)((t->inst) + offset);
      
      /* Add to cache */
      Type_Cache_Id--;
      Type_Cache_Id = Type_Cache_Id == -1 ? TYPE_CACHE_SIZE-1 : Type_Cache_Id;
      Type_Cache[Type_Cache_Id].type = self;
      Type_Cache[Type_Cache_Id].cls  =  cls;
      Type_Cache[Type_Cache_Id].inst = t->inst;
      
      return;
    }
    
    t++;
  }
  
  *inst = None;
  *meth = None;
  
}

var type_implements(var self, var cls) {
  var inst, meth;
  Type_Scan(self, cls, 0, &inst, &meth);
  return bool_var(inst isnt None);
}

var type_method_at_offset(
  var self, var cls, size_t offset, const char* method_name) {
  
  var inst, meth;
  Type_Scan(self, cls, offset, &inst, &meth);
  
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
  
  return inst;
  
}

var type_implements_method_at_offset(var self, var cls, size_t offset) {
  var inst, meth;
  Type_Scan(self, cls, offset, &inst, &meth);
  return bool_var(inst and meth);
}

var type_instance(var self, var cls) {
  
  var inst, meth;
  Type_Scan(self, cls, 0, &inst, &meth);
  
  if (inst is None) {  
    return throw(ClassError,
      "Type '%s' does not implement class '%s'", self, cls);
  } else {
    return inst;
  }
      
}

static int Type_Show(var self, var output, int pos) {
  return format_to(output, pos, "%s", Type_C_Str(self));
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
  Member(Show,     Type_Show, NULL),
  Member(Help,     Type_Help));
  
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
      if (head isnt False and head isnt True and head->magic isnt ((var)0xCe110)) {
        fprintf(stderr,
          "Cello Fatal Error: Pointer '%p' passed to 'type_of' "
          "has bad magic number - it wasn't allocated by Cello.\n", self);
        abort();
      }
#endif
    
    return head->type is None ? Type : head->type;
  }

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

var implements_method_at_offset(var self, var cls, size_t offset) {
  return type_implements_method_at_offset(type_of(self), cls, offset);
}
  
