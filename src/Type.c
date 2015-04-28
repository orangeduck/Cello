#include "Cello.h"

static const char* Cast_Name(void) {
  return "Cast";
}

static const char* Cast_Brief(void) {
  return "Runtime type checking";
}

static const char* Cast_Description(void) {
  return
    "The `Cast` class provides a rudimentary run-time type checking. By "
    "default it simply checks that the passed in object is of a given type "
    "but it can be overridden by types which have to do more complex checking "
    "to ensure the types are correct.";
}

static struct DocExample* Cast_Examples(void) {
  
  static struct DocExample examples[] = {
    {
      "Usage",
      "var x = $I(100);\n"
      "struct Int* y = cast(x, Int);\n"
      "show(y);\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct DocMethod* Cast_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "cast", 
      "var cast(var self, var type);",
      "Ensures the object `self` is of the given `type` and returns it if it "
      "is."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
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
  return 
    "The `Type` type is one of the most important types in Cello. It is the "
    "object which specifies the meta-data associated with a particular object "
    "most importantly this says what classes an object implements and what "
    "their instances are."
    "\n\n"
    "One can get the type of an object using the `type_of` function."
    "\n\n"
    "To see if an object implements a class `implements` can be used. To "
    "call a member of a class with an object `method` can be used."
    "\n\n"
    "To see if a type implements a class `type_implements` can be used. To "
    "call a member of a class, implemented `type_method` can be used.";
}

static struct DocExample* Type_Examples(void) {
  
  static struct DocExample examples[] = {
    {
      "Usage",
      "var t = type_of($I(5));\n"
      "show(t); /* Int */\n"
      "\n"
      "show($I(type_implements(t, New))); /* 1 */\n"
      "show($I(type_implements(t, Eq)));  /* 1 */\n"
      "show($I(type_implements(t, Ord))); /* 1 */\n"
      "\n"
      "show($I(type_method(t, Eq, eq, $I(5), $I(6)))); /* 0 */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

enum {
  CELLO_NBUILTINS = 2 + (CELLO_CACHE_NUM / 3),
  CELLO_MAX_INSTANCES = 256
};

static var Type_Alloc(void) {

  struct Header* head = calloc(1, 
    sizeof(struct Header) +
    sizeof(struct Type) * 
    (CELLO_NBUILTINS + 
     CELLO_MAX_INSTANCES + 1));
  
#if CELLO_MEMORY_CHECK == 1
  if (head is NULL) {
    throw(OutOfMemoryError, "Cannot create new 'Type', out of memory!");
  }
#endif
  
  return header_init(head, Type, AllocHeap);
}

static void Type_New(var self, var args) {
  
  struct Type* t = self;

  var name = get(args, $I(0));
  var size = get(args, $I(1));
  
#if CELLO_MEMORY_CHECK == 1
  if (len(args) - 2 > CELLO_MAX_INSTANCES) {
    throw(OutOfMemoryError,
      "Cannot construct 'Type' with %i instances, maximum is %i.",
      $I(len(args)), $I(CELLO_MAX_INSTANCES));
  }
#endif  
  
  size_t cache_entries = CELLO_CACHE_NUM / 3;
  for (size_t i = 0; i < cache_entries; i++) {
    t[i] = (struct Type){ NULL, NULL, NULL };
  }
  
  t[cache_entries+0] = (struct Type){ NULL, "__Name", (var)c_str(name) };
  t[cache_entries+1] = (struct Type){ NULL, "__Size", (var)c_int(size) };
  
  for(size_t i = 2; i < len(args); i++) {
    var ins = get(args, $I(i));
    t[CELLO_NBUILTINS-2+i] = (struct Type){
      NULL, (var)c_str(type_of(ins)), ins };
  }
  
  t[CELLO_NBUILTINS+len(args)-2] = (struct Type){ NULL, NULL, NULL };
}

static char* Type_Builtin_Name(struct Type* t) {
  return t[(CELLO_CACHE_NUM / 3)+0].inst;
}

static size_t Type_Builtin_Size(struct Type* t) {
  return (size_t)t[(CELLO_CACHE_NUM / 3)+1].inst;
}

static int Type_Show(var self, var output, int pos) {
  return format_to(output, pos, "%s", Type_Builtin_Name(self));
}

static int Type_Cmp(var self, var obj) {
  return (intptr_t)self - (intptr_t)obj;
}

static uint64_t Type_Hash(var self) {
  return (uint64_t)(uintptr_t)self;
}

static char* Type_C_Str(var self) {
  return Type_Builtin_Name(self);
}

/* TODO */
static void Type_Help(var self) {
  return;
}

var Type = CelloEmpty(Type,
  Instance(Doc,
    Type_Name, Type_Brief, Type_Description, Type_Examples, NULL),
  Instance(Alloc,    Type_Alloc, NULL),
  Instance(New,      Type_New, NULL),
  Instance(Cmp,      Type_Cmp),
  Instance(Hash,     Type_Hash),
  Instance(Show,     Type_Show, NULL),
  Instance(C_Str,    Type_C_Str),
  Instance(Help,     Type_Help));

static var Type_Scan(var self, var cls) {
 
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
  return Type_Scan(self, cls) isnt NULL;
}

bool type_implements(var self, var cls) {
  return Type_Implements(self, cls);
}

static var Type_Instance(var self, var cls);

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
  var inst = Type_Scan(self, cls);
  if (inst is NULL) { return false; }
  var meth = *((var*)(((char*)inst) + offset));
  if (meth is NULL) { return false; }
  return true;
}

bool type_implements_method_at_offset(var self, var cls, size_t offset) {
  return Type_Implements_Method_At_Offset(self, cls, offset);
}

/*
**  Doing the lookup of a class instances is fairly fast
**  but still too slow to be done inside a tight inner loop.
**  This is because there could be any number of instances 
**  and they could be in any order, so each time a linear 
**  search must be done to find the correct instance.
**
**  We can remove the need for a linear search by placing
**  some common class instances at known locations. These 
**  are the _Type Cache Entries_ and are located at some
**  preallocated space at the beginning of every type object.
**
**  The only problem is that these instances are not filled 
**  at compile type, so we must dynamically fill them if they
**  are empty. But this can be done with a standard call to 
**  `Type_Scan` the first time.
**
**  The main advantage of this method is that it gives the compiler
**  a better chance of inlining the code up to the call of the 
**  instance function pointer, and removes the overhead 
**  associated with setting up the call to `Type_Scan` which is 
**  too complex a call to be effectively inlined.
**
*/

#define Type_Cache_Entry(i, lit) \
  if (cls is lit) { \
    var inst = ((var*)self)[i]; \
    if (inst is NULL) { \
      inst = Type_Scan(self, lit); \
      ((var*)self)[i] = inst; \
    } \
    return inst; \
  }

static var Type_Instance(var self, var cls) {

#if CELLO_CACHE == 1
  Type_Cache_Entry( 0, Size);   Type_Cache_Entry( 1, Alloc);
  Type_Cache_Entry( 2, New);    Type_Cache_Entry( 3, Assign);
  Type_Cache_Entry( 4, Cmp);    Type_Cache_Entry( 5, Mark);
  Type_Cache_Entry( 6, Hash);   Type_Cache_Entry( 7, Len);
  Type_Cache_Entry( 8, Iter);   Type_Cache_Entry( 9, Push);
  Type_Cache_Entry(10, Concat); Type_Cache_Entry(11, Get);
  Type_Cache_Entry(12, C_Str);  Type_Cache_Entry(13, C_Int);
  Type_Cache_Entry(14, C_Float);
#endif
  
  return Type_Scan(self, cls);
}

#undef Type_Cache_Entry

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
  
  struct Header* head = 
    (struct Header*)((char*)self - sizeof(struct Header));

#if CELLO_MAGIC_CHECK == 1
  if (head->magic is (var)0xDeadCe110) {
    throw(ValueError, "Pointer '%p' passed to 'type_of' "
      "has bad magic number, it looks like it was already deallocated.", self);    
  }

  if (head->magic isnt ((var)CELLO_MAGIC_NUM)) {
    throw(ValueError, "Pointer '%p' passed to 'type_of' "
      "has bad magic number, perhaps it wasn't allocated by Cello.", self);
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

static const char* Size_Brief(void) {
  return "Object has some Size";
}

static const char* Size_Description(void) {
  return
    "The `Size` class is a very important class in Cello because it gives the "
    "size in bytes you can expect an object of a given type to be. This is "
    "used by many methods to allocate, assign, or compare various objects."
    "\n\n"
    "By default this size is automatically found and recorded by the `Cello` "
    "macro, but if the type does it's own allocation, or the size cannot be "
    "found naturally then it may be necessary to override this method.";
}

static struct DocExample* Size_Examples(void) {
  
  static struct DocExample examples[] = {
    {
      "Usage",
      "show($I(size(Int)));\n"
      "show($I(size(Float)));\n"
      "show($I(size(Array)));\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct DocMethod* Size_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "size", 
      "size_t size(var type);",
      "Returns the associated size of a given `type` in bytes."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Size = Cello(Size, Instance(Doc, 
  Size_Name, Size_Brief, Size_Description, Size_Examples, Size_Methods));

size_t size(var type) {

  struct Size* s = type_instance(type, Size);
  if (s and s->size) {
    return s->size();
  }
  
  return Type_Builtin_Size(type);
}
  
