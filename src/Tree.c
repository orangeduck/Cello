#include "Cello.h"

static const char* Tree_Name(void) {
  return "Tree";
}

static const char* Tree_Brief(void) {
  return "Balanced Binary Tree";
}

static const char* Tree_Description(void) {
  return
    "The `Tree` type is a self balancing binary tree implemented as a red-black "
    "tree. It provides key-value access and requires the `Cmp` class to be "
    "defined on the key type."
    "\n\n"
    "Element lookup and insertion are provided as an `O(log(n))` operation. "
    "This means in general a `Tree` is slower than a `Table` but it has several "
    "other nice properties such as being able to iterate over the items in "
    "order and not having large pauses for rehashing on some insertions."
    "\n\n"
    "This is largely equivalent to the C++ construct "
    "[std::map](http://www.cplusplus.com/reference/map/map/)";
}

static struct Example* Tree_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var prices = new(Tree, String, Int);\n"
      "set(prices, $S(\"Apple\"),  $I(12));\n"
      "set(prices, $S(\"Banana\"), $I( 6));\n"
      "set(prices, $S(\"Pear\"),   $I(55));\n"
      "\n"
      "foreach (key in prices) {\n"
      "  var price = get(prices, key);\n"
      "  println(\"Price of %$ is %$\", key, price);\n"
      "}\n"
    }, {
      "Manipulation",
      "var t = new(Tree, String, Int);\n"
      "set(t, $S(\"Hello\"), $I(2));\n"
      "set(t, $S(\"There\"), $I(5));\n"
      "\n"
      "show($I(len(t))); /* 2 */\n"
      "show($I(mem(t, $S(\"Hello\")))); /* 1 */\n"
      "\n"
      "rem(t, $S(\"Hello\"));\n"
      "\n"
      "show($I(len(t))); /* 1 */\n"
      "show($I(mem(t, $S(\"Hello\")))); /* 0 */\n"
      "show($I(mem(t, $S(\"There\")))); /* 1 */\n"
      "\n"
      "resize(t, 0);\n"
      "\n"
      "show($I(len(t))); /* 0 */\n"
      "show($I(mem(t, $S(\"Hello\")))); /* 0 */\n"
      "show($I(mem(t, $S(\"There\")))); /* 0 */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

struct Tree {
  var root;
  var ktype;
  var vtype;
  size_t ksize;
  size_t vsize;
  size_t nitems;
};

static bool Tree_Is_Red(struct Tree* m, var node);

static var* Tree_Left(struct Tree* m, var node) {
  return (var*)((char*)node + 0 * sizeof(var));
}

static var* Tree_Right(struct Tree* m, var node) {
  return (var*)((char*)node + 1 * sizeof(var));
}

static var Tree_Get_Parent(struct Tree* m, var node) {
  var ptr = *(var*)((char*)node + 2 * sizeof(var));
  return (var)(((uintptr_t)ptr) & (~1));
}

static void Tree_Set_Parent(struct Tree* m, var node, var ptr) {
  if (Tree_Is_Red(m, node)) {
    *(var*)((char*)node + 2 * sizeof(var)) = (var)(((uintptr_t)ptr) | 1);
  } else {
    *(var*)((char*)node + 2 * sizeof(var)) = ptr;
  }
}

static var Tree_Key(struct Tree* m, var node) {
  return (char*)node + 3 * sizeof(var) + sizeof(struct Header);
}

static var Tree_Val(struct Tree* m, var node) {
  return (char*)node + 3 * sizeof(var) +
    sizeof(struct Header) + m->ksize +
    sizeof(struct Header);
}

static void Tree_Set_Color(struct Tree* m, var node, bool col) {
  var ptr = Tree_Get_Parent(m, node);
  if (col) {
    *(var*)((char*)node + 2 * sizeof(var)) = (var)(((uintptr_t)ptr) | 1);
  } else {
    *(var*)((char*)node + 2 * sizeof(var)) = ptr;
  }
}

static bool Tree_Get_Color(struct Tree* m, var node) {
  if (node is NULL) { return 0; }
  var ptr = *(var*)((char*)node + 2 * sizeof(var));
  return ((uintptr_t)ptr) & 1;
}

static void Tree_Set_Black(struct Tree* m, var node) {
  Tree_Set_Color(m, node, false);
}

static void Tree_Set_Red(struct Tree* m, var node) {
  Tree_Set_Color(m, node, true);
}

static bool Tree_Is_Red(struct Tree* m, var node) {
  return Tree_Get_Color(m, node);
}

static bool Tree_Is_Black(struct Tree* m, var node) {
  return not Tree_Get_Color(m, node);
}

static var Tree_Alloc(struct Tree* m) {
  var node = calloc(1, 3 * sizeof(var) + 
    sizeof(struct Header) + m->ksize + 
    sizeof(struct Header) + m->vsize);
  
#if CELLO_MEMORY_CHECK == 1
  if (node is NULL) {
    throw(OutOfMemoryError, "Cannot allocate Tree entry, out of memory!");
  }
#endif
  
  var key = header_init((struct Header*)(
    (char*)node + 3 * sizeof(var)), m->ktype, AllocData);
  var val = header_init((struct Header*)(
    (char*)node + 3 * sizeof(var) +
    sizeof(struct Header) + m->ksize), m->vtype, AllocData);
  
  *Tree_Left(m, node) = NULL;
  *Tree_Right(m, node) = NULL;
  Tree_Set_Parent(m, node, NULL);
  Tree_Set_Red(m, node);
  
  return node;
}

static void Tree_Set(var self, var key, var val);

static void Tree_New(var self, var args) {
  struct Tree* m = self;
  m->ktype = get(args, $I(0));
  m->vtype = get(args, $I(1));
  m->ksize = size(m->ktype);
  m->vsize = size(m->vtype);
  m->nitems = 0;
  m->root = NULL;

  size_t nargs = len(args);
  if (nargs % 2 isnt 0) {
    throw(FormatError, 
      "Received non multiple of two argument count to Tree constructor.");
  }
  
  for(size_t i = 0; i < (nargs-2)/2; i++) {
    var key = get(args, $I(2+(i*2)+0));
    var val = get(args, $I(2+(i*2)+1));
    Tree_Set(m, key, val);
  }
  
}

static void Tree_Clear_Entry(struct Tree* m, var node) {
  if (node isnt NULL) {
    Tree_Clear_Entry(m, *Tree_Left(m, node));
    Tree_Clear_Entry(m, *Tree_Right(m, node));
    destruct(Tree_Key(m, node));
    destruct(Tree_Val(m, node));
    free(node);
  }
}

static void Tree_Clear(var self) {
  struct Tree* m = self;
  Tree_Clear_Entry(m, m->root);
  m->nitems = 0;
  m->root = NULL;
}

static void Tree_Del(var self) {
  struct Tree* m = self;
  Tree_Clear(self);
}

static void Tree_Assign(var self, var obj) {
  struct Tree* m = self;
  Tree_Clear(self);
  m->ktype = implements_method(obj, Get, key_type) ? key_type(obj) : Ref;
  m->vtype = implements_method(obj, Get, val_type) ? val_type(obj) : Ref;
  m->ksize = size(m->ktype);
  m->vsize = size(m->vtype);
  foreach (key in obj) {
    Tree_Set(self, key, get(obj, key));
  }
}

static var Tree_Iter_Init(var self);
static var Tree_Iter_Next(var self, var curr);

static bool Tree_Mem(var self, var key);
static var Tree_Get(var self, var key);

static int Tree_Cmp(var self, var obj) {
  
  int c;
  var item0 = Tree_Iter_Init(self);
  var item1 = iter_init(obj);
  
  while (true) {
    if (item0 is Terminal and item1 is Terminal) { return 0; }
    if (item0 is Terminal) { return -1; }
    if (item1 is Terminal) { return  1; }
    c = cmp(item0, item1);
    if (c < 0) { return -1; }
    if (c > 0) { return  1; }
    c = cmp(Tree_Get(self, item0), get(obj, item1));
    if (c < 0) { return -1; }
    if (c > 0) { return  1; }
    item0 = Tree_Iter_Next(self, item0);
    item1 = iter_next(obj, item1);
  }
  
  return 0;
  
}

static uint64_t Tree_Hash(var self) {
  struct Tree* m = self;
  uint64_t h = 0;
  
  var curr = Tree_Iter_Init(self);
  while (curr isnt Terminal) {
    var node = (char*)curr - sizeof(struct Header) - 3 * sizeof(var);
    h = h ^ hash(Tree_Key(m, node)) ^ hash(Tree_Val(m, node));
    curr = Tree_Iter_Next(self, curr);
  }
  
  return h;
}

static size_t Tree_Len(var self) {
  struct Tree* m = self;
  return m->nitems;
}

static bool Tree_Mem(var self, var key) {
  struct Tree* m = self;
  key = cast(key, m->ktype);
  
  var node = m->root;
  while (node isnt NULL) { 
    int c = cmp(Tree_Key(m, node), key);
    if (c is 0) { return true; }
    node = c < 0 ? *Tree_Left(m, node) : *Tree_Right(m, node);
  }
  
  return false;
}

static var Tree_Get(var self, var key) {
  struct Tree* m = self;
  key = cast(key, m->ktype);
  
  var node = m->root;
  while (node isnt NULL) {
    int c = cmp(Tree_Key(m, node), key);
    if (c is 0) { return Tree_Val(m, node); }
    node = c < 0 ? *Tree_Left(m, node) : *Tree_Right(m, node);
  }
  
  return throw(KeyError, "Key %$ not in Tree!", key);
}

static var Tree_Key_Type(var self) {
  struct Tree* m = self;  
  return m->ktype;
}

static var Tree_Val_Type(var self) {
  struct Tree* m = self;  
  return m->vtype;
}

static var Tree_Maximum(struct Tree* m, var node) {
  while (*Tree_Right(m, node) isnt NULL) {
    node = *Tree_Right(m, node);
  }
  return node;
}

static var Tree_Sibling(struct Tree* m, var node) {
  
  if (node is NULL or Tree_Get_Parent(m, node) is NULL) {
    return NULL;
  }
  
  if (node is *Tree_Left(m, Tree_Get_Parent(m, node))) {
    return *Tree_Right(m, Tree_Get_Parent(m, node));
  } else {
    return *Tree_Left(m, Tree_Get_Parent(m, node));
  }
}

static var Tree_Grandparent(struct Tree* m, var node) {
  if ((node isnt NULL) and (Tree_Get_Parent(m, node) isnt NULL)) {
    return Tree_Get_Parent(m, Tree_Get_Parent(m, node));
  } else {
    return NULL;
  }
}

static var Tree_Uncle(struct Tree* m, var node) {
  var gpar = Tree_Grandparent(m, node);
  if (gpar is NULL) { return NULL; }
  if (Tree_Get_Parent(m, node) is *Tree_Left(m, gpar)) {
    return *Tree_Right(m, gpar);
  } else {
    return *Tree_Left(m, gpar);
  }
}

void Tree_Replace(struct Tree* m, var oldn, var newn) {
  if (Tree_Get_Parent(m, oldn) is NULL) {
    m->root = newn;
  } else {
    if (oldn is *Tree_Left(m, Tree_Get_Parent(m, oldn))) {
      *Tree_Left(m, Tree_Get_Parent(m, oldn)) = newn;
    } else {
      *Tree_Right(m, Tree_Get_Parent(m, oldn)) = newn;
    }
  }
  if (newn isnt NULL) {
    Tree_Set_Parent(m, newn, Tree_Get_Parent(m, oldn));
  }
}

static void Tree_Rotate_Left(struct Tree* m, var node) {
  var r = *Tree_Right(m, node);
  Tree_Replace(m, node, r);
  *Tree_Right(m, node) = *Tree_Left(m, r);
  if (*Tree_Left(m, r) isnt NULL) {
      Tree_Set_Parent(m, *Tree_Left(m, r), node);
  }
  *Tree_Left(m, r) = node;
  Tree_Set_Parent(m, node, r);
}

static void Tree_Rotate_Right(struct Tree* m, var node) {
  var l = *Tree_Left(m, node);
  Tree_Replace(m, node, l);
  *Tree_Left(m, node) = *Tree_Right(m, l);
  if (*Tree_Right(m, l) isnt NULL) {
    Tree_Set_Parent(m, *Tree_Right(m, l), node);
  }
  *Tree_Right(m, l) = node;
  Tree_Set_Parent(m, node, l);
}

static void Tree_Set_Fix(struct Tree* m, var node) {
  
  while (true) {
  
    if (Tree_Get_Parent(m, node) is NULL) {
      Tree_Set_Black(m, node);
      return;
    }
    
    if (Tree_Is_Black(m, Tree_Get_Parent(m, node))) { return; }
    
    if ((Tree_Uncle(m, node) isnt NULL)
    and (Tree_Is_Red(m, Tree_Uncle(m, node)))) {
      Tree_Set_Black(m, Tree_Get_Parent(m, node));
      Tree_Set_Black(m, Tree_Uncle(m, node));
      Tree_Set_Red(m, Tree_Grandparent(m, node));
      node = Tree_Grandparent(m, node);
      continue;
    }
    
    if ((node is *Tree_Right(m, Tree_Get_Parent(m, node))) 
    and (Tree_Get_Parent(m, node) is *Tree_Left(m, Tree_Grandparent(m, node)))) {
      Tree_Rotate_Left(m, Tree_Get_Parent(m, node));
      node = *Tree_Left(m, node);
    }
    
    else
    
    if ((node is *Tree_Left(m, Tree_Get_Parent(m, node))) 
    and (Tree_Get_Parent(m, node) is *Tree_Right(m, Tree_Grandparent(m, node)))) {
      Tree_Rotate_Right(m, Tree_Get_Parent(m, node));
      node = *Tree_Right(m, node);
    }
    
    Tree_Set_Black(m, Tree_Get_Parent(m, node));
    Tree_Set_Red(m, Tree_Grandparent(m, node));
    
    if (node is *Tree_Left(m, Tree_Get_Parent(m, node))) {
      Tree_Rotate_Right(m, Tree_Grandparent(m, node));
    } else {
      Tree_Rotate_Left(m, Tree_Grandparent(m, node));
    }
    
    return;
  
  }
  
}

static void Tree_Set(var self, var key, var val) {
  struct Tree* m = self;
  key = cast(key, m->ktype);
  val = cast(val, m->vtype);
  
  var node = m->root;
  
  if (node is NULL) {
    var node = Tree_Alloc(m);
    assign(Tree_Key(m, node), key);
    assign(Tree_Val(m, node), val);
    m->root = node;
    m->nitems++;
    Tree_Set_Fix(m, node);
    return;
  }
  
  while (true) {
    
    int c = cmp(Tree_Key(m, node), key);
    
    if (c is 0) {
      assign(Tree_Key(m, node), key);
      assign(Tree_Val(m, node), val);
      return;
    }
    
    if (c < 0) {
    
      if (*Tree_Left(m, node) is NULL) {
        var newn = Tree_Alloc(m);
        assign(Tree_Key(m, newn), key);
        assign(Tree_Val(m, newn), val);
        *Tree_Left(m, node) = newn;
        Tree_Set_Parent(m, newn, node);
        Tree_Set_Fix(m, newn);
        m->nitems++;
        return;
      }
      
      node = *Tree_Left(m, node);
    }
      
    if (c > 0) {
    
      if (*Tree_Right(m, node) is NULL) {
        var newn = Tree_Alloc(m);
        assign(Tree_Key(m, newn), key);
        assign(Tree_Val(m, newn), val);
        *Tree_Right(m, node) = newn;
        Tree_Set_Parent(m, newn, node);
        Tree_Set_Fix(m, newn);
        m->nitems++;
        return;
      }
      
      node = *Tree_Right(m, node);
    }
    
  }
  
}

static void Tree_Rem_Fix(struct Tree* m, var node) {
 
  while (true) {
  
    if (Tree_Get_Parent(m, node) is NULL) { return; }
    
    if (Tree_Is_Red(m, Tree_Sibling(m, node))) {
      Tree_Set_Red(m, Tree_Get_Parent(m, node));
      Tree_Set_Black(m, Tree_Sibling(m, node));
      if (node is *Tree_Left(m, Tree_Get_Parent(m, node))) {
          Tree_Rotate_Left(m, Tree_Get_Parent(m, node));
      } else {
          Tree_Rotate_Right(m, Tree_Get_Parent(m, node));
      }
    }
    
    if (Tree_Is_Black(m, Tree_Get_Parent(m, node))
    and Tree_Is_Black(m, Tree_Sibling(m, node))
    and Tree_Is_Black(m, *Tree_Left(m, Tree_Sibling(m, node)))
    and Tree_Is_Black(m, *Tree_Right(m, Tree_Sibling(m, node)))) {
      Tree_Set_Red(m, Tree_Sibling(m, node));
      node = Tree_Get_Parent(m, node);
      continue;
    }
    
    if (Tree_Is_Red(m, Tree_Get_Parent(m, node))
    and Tree_Is_Black(m, Tree_Sibling(m, node))
    and Tree_Is_Black(m, *Tree_Left(m, Tree_Sibling(m, node)))
    and Tree_Is_Black(m, *Tree_Right(m, Tree_Sibling(m, node)))) {
      Tree_Set_Red(m, Tree_Sibling(m, node));
      Tree_Set_Black(m, Tree_Get_Parent(m, node));
      return;
    }
    
    if (Tree_Is_Black(m, Tree_Sibling(m, node))) {
    
      if (node is *Tree_Left(m, Tree_Get_Parent(m, node))
      and Tree_Is_Red(m, *Tree_Left(m, Tree_Sibling(m, node)))
      and Tree_Is_Black(m, *Tree_Right(m, Tree_Sibling(m, node)))) {
        Tree_Set_Red(m, Tree_Sibling(m, node));
        Tree_Set_Black(m, *Tree_Left(m, Tree_Sibling(m, node)));
        Tree_Rotate_Right(m, Tree_Sibling(m, node));
      }
      
      else 
      
      if (node is *Tree_Right(m, Tree_Get_Parent(m, node))
      and Tree_Is_Red(m, *Tree_Right(m, Tree_Sibling(m, node)))
      and Tree_Is_Black(m, *Tree_Left(m, Tree_Sibling(m, node)))) {
        Tree_Set_Red(m, Tree_Sibling(m, node));
        Tree_Set_Black(m, *Tree_Right(m, Tree_Sibling(m, node)));
        Tree_Rotate_Left(m, Tree_Sibling(m, node));
      }
    
    }
    
    Tree_Set_Color(m, 
      Tree_Sibling(m, node),  
      Tree_Get_Color(m, Tree_Get_Parent(m, node)));
    
    Tree_Set_Black(m, Tree_Get_Parent(m, node));
    
    if (node is *Tree_Left(m, Tree_Get_Parent(m, node))) {
        Tree_Set_Black(m, *Tree_Right(m, Tree_Sibling(m, node)));
        Tree_Rotate_Left(m, Tree_Get_Parent(m, node));
    } else {
        Tree_Set_Black(m, *Tree_Left(m, Tree_Sibling(m, node)));
        Tree_Rotate_Right(m, Tree_Get_Parent(m, node));
    }
    
    return;
  }

}

static void Tree_Rem(var self, var key) {
  struct Tree* m = self;
  
  key = cast(key, m->ktype);
  
  bool found = false;
  var node = m->root;
  while (node isnt NULL) {
    int c = cmp(Tree_Key(m, node), key);
    if (c is 0) { found = true; break; }
    node = c < 0 ? *Tree_Left(m, node) : *Tree_Right(m, node);
  }
  
  if (not found) {
    throw(KeyError, "Key %$ not in Tree!", key);
    return;
  }
  
  destruct(Tree_Key(m, node));
  destruct(Tree_Val(m, node));
  
  if ((*Tree_Left(m, node) isnt NULL) 
  and (*Tree_Right(m, node) isnt NULL)) {
    var pred = Tree_Maximum(m, *Tree_Left(m, node));
    bool ncol = Tree_Get_Color(m, node);
    memcpy((char*)node + 3 * sizeof(var), (char*)pred + 3 * sizeof(var),
      sizeof(struct Header) + m->ksize +
      sizeof(struct Header) + m->vsize);
    Tree_Set_Color(m, node, ncol);
    node = pred;
  }

  var chld = *Tree_Right(m, node) is NULL 
    ? *Tree_Left(m, node) 
    : *Tree_Right(m, node);
    
  if (Tree_Is_Black(m, node)) {
    Tree_Set_Color(m, node, Tree_Get_Color(m, chld));
    Tree_Rem_Fix(m, node);
  }
    
  Tree_Replace(m, node, chld);
  
  if ((Tree_Get_Parent(m, node) is NULL) and (chld isnt NULL)) {
    Tree_Set_Black(m, chld);
  }
  
  m->nitems--;
  free(node);
  
}

static var Tree_Iter_Init(var self) {
  struct Tree* m = self;
  if (m->nitems is 0) { return Terminal; }
  var node = m->root;
  while (*Tree_Left(m, node) isnt NULL) {
    node = *Tree_Left(m, node);
  }
  return Tree_Key(m, node);
}

static var Tree_Iter_Next(var self, var curr) {
  struct Tree* m = self;
  
  var node = (char*)curr - sizeof(struct Header) - 3 * sizeof(var);
  var prnt = Tree_Get_Parent(m, node);
  
  if (*Tree_Right(m, node) isnt NULL) {
    node = *Tree_Right(m, node);
    while (*Tree_Left(m, node) isnt NULL) {
      node = *Tree_Left(m, node);
    }
    return Tree_Key(m, node);
  }
  
  while (true) {
    if (prnt is NULL) { return Terminal; }
    if (node is *Tree_Left(m, prnt)) { return Tree_Key(m, prnt); }
    if (node is *Tree_Right(m, prnt)) {
      prnt = Tree_Get_Parent(m, prnt);
      node = Tree_Get_Parent(m, node);
    }
  }
  
  return Terminal;
}

static var Tree_Iter_Last(var self) {
  struct Tree* m = self;
  if (m->nitems is 0) { return Terminal; }
  var node = m->root;
  while (*Tree_Right(m, node) isnt NULL) {
    node = *Tree_Right(m, node);
  }
  return Tree_Key(m, node);
}

static var Tree_Iter_Prev(var self, var curr) {
  struct Tree* m = self;
  
  var node = (char*)curr - sizeof(struct Header) - 3 * sizeof(var);
  var prnt = Tree_Get_Parent(m, node);
  
  if (*Tree_Left(m, node) isnt NULL) {
    node = *Tree_Left(m, node);
    while (*Tree_Right(m, node) isnt NULL) {
      node = *Tree_Right(m, node);
    }
    return Tree_Key(m, node);
  }
  
  while (true) {
    if (prnt is NULL) { return Terminal; }
    if (node is *Tree_Right(m, prnt)) { return Tree_Key(m, prnt); }
    if (node is *Tree_Left(m, prnt)) {
      prnt = Tree_Get_Parent(m, prnt);
      node = Tree_Get_Parent(m, node);
    }
  }
  
  return Terminal;
}

static var Tree_Iter_Type(var self) {
  struct Tree* m = self;
  return m->ktype;
}

static int Tree_Show(var self, var output, int pos) {
  struct Tree* m = self;  
  
  pos = print_to(output, pos, "<'Tree' At 0x%p {", self);
  
  var curr = Tree_Iter_Init(self);
  
  while (curr isnt Terminal) {
    var node = (char*)curr - sizeof(struct Header) - 3 * sizeof(var);
    pos = print_to(output, pos, "%$:%$",
      Tree_Key(m, node), Tree_Val(m, node));
    curr = Tree_Iter_Next(self, curr);
    if (curr isnt Terminal) { pos = print_to(output, pos, ", "); }
  }
  
  return print_to(output, pos, "}>");
}

static void Tree_Mark(var self, var gc, void(*f)(var,void*)) {  
  struct Tree* m = self;

  var curr = Tree_Iter_Init(self);
  
  while (curr isnt Terminal) {
    var node = (char*)curr - sizeof(struct Header) - 3 * sizeof(var);
    f(gc, Tree_Key(m, node));
    f(gc, Tree_Val(m, node));
    curr = Tree_Iter_Next(self, curr);
  }
  
}

static void Tree_Resize(var self, size_t n) {
  
  if (n is 0) {
    Tree_Clear(self);
  } else {
    throw(FormatError, 
      "Cannot resize Tree to %li items. Trees can only be resized to 0 items.", 
      $I(n));
  }
  
}

var Tree = Cello(Tree,
  Instance(Doc,
    Tree_Name, Tree_Brief,    Tree_Description,
    NULL,      Tree_Examples, NULL),
  Instance(New,     Tree_New, Tree_Del),
  Instance(Assign,  Tree_Assign),
  Instance(Mark,    Tree_Mark),
  Instance(Cmp,     Tree_Cmp),
  Instance(Hash,    Tree_Hash),
  Instance(Len,     Tree_Len),
  Instance(Get, 
    Tree_Get, Tree_Set, Tree_Mem, Tree_Rem, 
    Tree_Key_Type,  Tree_Val_Type),
  Instance(Resize,  Tree_Resize),
  Instance(Iter, 
    Tree_Iter_Init, Tree_Iter_Next, 
    Tree_Iter_Last, Tree_Iter_Prev, Tree_Iter_Type),
  Instance(Show,    Tree_Show, NULL));


