#include "Cello.h"

static const char* Map_Name(void) {
  return "Map";
}

/* TODO */
static const char* Map_Brief(void) {
  return "";
}

/* TODO */
static const char* Map_Description(void) {
  return "";
}

/* TODO */
static const char* Map_Examples(void) {
  return "";
}

/* TODO */
static const char* Map_Methods(void) {
  return "";
}

struct Map {
  var root;
  var ktype;
  var vtype;
  size_t ksize;
  size_t vsize;
  size_t nitems;
};

static var* Map_Left(struct Map* m, var node) {
  return node + 0 * sizeof(var);
}

static var* Map_Right(struct Map* m, var node) {
  return node + 1 * sizeof(var);
}

static var* Map_Parent(struct Map* m, var node) {
  return node + 2 * sizeof(var);
}

static var Map_Key(struct Map* m, var node) {
  return node + 3 * sizeof(var) + sizeof(struct CelloHeader);
}

static var Map_Val(struct Map* m, var node) {
  return node + 3 * sizeof(var) + 
    sizeof(struct CelloHeader) + m->ksize +
    sizeof(struct CelloHeader);
}

static void Map_Set_Color(struct Map* m, var node, var value) {
  if (value) {
    CelloHeader_SetFlag(
      Map_Key(m, node) - sizeof(struct CelloHeader), CelloRed);
  } else {
    CelloHeader_RemFlag(
      Map_Key(m, node) - sizeof(struct CelloHeader), CelloRed);
  }
}

static var Map_Get_Color(struct Map* m, var node) {
  if (node is Terminal) {
    return False;
  } else {
    return CelloHeader_GetFlag(
      Map_Key(m, node) - sizeof(struct CelloHeader), CelloRed);
  }
}

static void Map_Set_Black(struct Map* m, var node) {
  Map_Set_Color(m, node, False);
}

static void Map_Set_Red(struct Map* m, var node) {
  Map_Set_Color(m, node, True);
}

static var Map_Is_Red(struct Map* m, var node) {
  return Map_Get_Color(m, node);
}

static var Map_Is_Black(struct Map* m, var node) {
  return bool_var(not Map_Get_Color(m, node));
}

static var Map_Alloc(struct Map* m) {
  var node = calloc(1, 3 * sizeof(var) + 
    sizeof(struct CelloHeader) + m->ksize + 
    sizeof(struct CelloHeader) + m->vsize);
  
#if CELLO_MEMORY_CHECK == 1
  if (node is None) {
    throw(OutOfMemoryError, "Cannot allocate Map entry, out of memory!");
  }
#endif
  
  var key = CelloHeader_Init(node + 3 * sizeof(var), m->ktype, CelloDataAlloc);
  var val = CelloHeader_Init(node + 3 * sizeof(var) + 
    sizeof(struct CelloHeader) + m->ksize, m->vtype, CelloDataAlloc);
  
  *Map_Left(m, node) = Terminal;
  *Map_Right(m, node) = Terminal;
  *Map_Parent(m, node) = Terminal;
  Map_Set_Red(m, node);
  
  return node;
}

static void Map_Debug(struct Map* m, var node, int depth) {
  
  if (node isnt Terminal) {
    putchar('\n');
    for (int i = 0; i < depth; i++) { putchar(' '); }
    print("| %c:%$:%$ >", 
      $C(Map_Is_Red(m, node) ? 'r' : 'b'),
      Map_Key(m, node), Map_Val(m, node));
    Map_Debug(m, *Map_Right(m, node), depth+4); 
    Map_Debug(m, *Map_Left(m, node), depth+4);
  }
  
}

static void Map_Set(var self, var key, var val);

static var Map_New(var self, var args) {
  struct Map* m = self;
  m->ktype = get(args, $I(0));
  m->vtype = get(args, $I(1));
  m->ksize = size(m->ktype);
  m->vsize = size(m->vtype);
  m->nitems = 0;
  m->root = Terminal;

  size_t nargs = len(args);
  if (nargs % 2 isnt 0) {
    return throw(FormatError, 
      "Received non multiple of two argument count to Map constructor.");
  }
  
  for(size_t i = 0; i < (nargs-2)/2; i++) {
    var key = get(args, $I(2+(i*2)+0));
    var val = get(args, $I(2+(i*2)+1));
    Map_Set(m, key, val);
  }
  
  return self;
}

static void Map_Clear_Entry(struct Map* m, var node) {
  if (node isnt Terminal) {
    Map_Clear_Entry(m, *Map_Left(m, node));
    Map_Clear_Entry(m, *Map_Right(m, node));
    destruct(Map_Key(m, node));
    destruct(Map_Val(m, node));
    free(node);
  }
}

static void Map_Clear(var self) {
  struct Map* m = self;
  Map_Clear_Entry(m, m->root);
  m->nitems = 0;
  m->root = Terminal;
}

static var Map_Del(var self) {
  struct Map* m = self;
  Map_Clear(self);
  return self;
}

static size_t Map_Size(void) {
  return sizeof(struct Map);
}

static var Map_Assign(var self, var obj) {
  Map_Clear(self);
  foreach (key in obj) {
    Map_Set(self, key, get(obj, key));
  }
  return self;
}

static var Map_Iter_Init(var self);
static var Map_Iter_Next(var self, var curr);

static var Map_Copy(var self) {
  struct Map* m = self;
  
  var r = new(Map, m->ktype, m->vtype);
  
  var curr = Map_Iter_Init(self);
  while (curr isnt Terminal) {
    var node = curr - sizeof(struct CelloHeader) - 3 * sizeof(var);
    set(r, Map_Key(m, node), Map_Val(m, node));
    curr = Map_Iter_Next(self, curr);
  }
  
  return r;
}

static var Map_Mem(var self, var key);
static var Map_Get(var self, var key);

static var Map_Eq(var self, var obj) {
  
  foreach (key in obj) {
    if (not Map_Mem(self, key)) { return False; }
    if_neq(get(obj, key), Map_Get(self, key)) { return False; }
  }
	
  foreach (key in self) {
    if (not mem(obj, key)) { return False; }
    if_neq(get(obj, key), Map_Get(self, key)) { return False; }
  }
	
  return True;
}

static size_t Map_Len(var self) {
  struct Map* m = self;
  return m->nitems;
}

static var Map_Mem(var self, var key) {
  struct Map* m = self;
  key = cast(key, m->ktype);
  
  var node = m->root;
  while (node isnt Terminal) { 
  
    if_eq(Map_Key(m, node), key) {
      return True;
    }
    
    node = lt(Map_Key(m, node), key) 
      ? *Map_Left(m, node) 
      : *Map_Right(m, node);
  }
  
  return False;
}

static var Map_Get(var self, var key) {
  struct Map* m = self;
  key = cast(key, m->ktype);
  
  var node = m->root;
  while (node isnt Terminal) {
  
    if_eq(Map_Key(m, node), key) {
      return Map_Val(m, node);
    }
    
    node = lt(Map_Key(m, node), key) 
      ? *Map_Left(m, node) 
      : *Map_Right(m, node);
  }
  
  return throw(KeyError, "Key %$ not in Map!", key);
}

static var Map_Maximum(struct Map* m, var node) {
  while (*Map_Right(m, node) isnt Terminal) {
    node = *Map_Right(m, node);
  }
  return node;
}

static var Map_Sibling(struct Map* m, var node) {
  
  if (node is Terminal or *Map_Parent(m, node) is Terminal) {
    return Terminal;
  }
  
  if (node is *Map_Left(m, *Map_Parent(m, node))) {
    return *Map_Right(m, *Map_Parent(m, node));
  } else {
    return *Map_Left(m, *Map_Parent(m, node));
  }
}

static var Map_Grandparent(struct Map* m, var node) {
  if ((node isnt Terminal) and (*Map_Parent(m, node) isnt Terminal)) {
    return *Map_Parent(m, *Map_Parent(m, node));
  } else {
    return Terminal;
  }
}

static var Map_Uncle(struct Map* m, var node) {
  var gpar = Map_Grandparent(m, node);
  if (gpar is Terminal) { return Terminal; }
  if (*Map_Parent(m, node) is *Map_Left(m, gpar)) {
    return *Map_Right(m, gpar);
  } else {
    return *Map_Left(m, gpar);
  }
}

void Map_Replace(struct Map* m, var oldn, var newn) {
  if (*Map_Parent(m, oldn) is Terminal) {
    m->root = newn;
  } else {
    if (oldn is *Map_Left(m, *Map_Parent(m, oldn))) {
      *Map_Left(m, *Map_Parent(m, oldn)) = newn;
    } else {
      *Map_Right(m, *Map_Parent(m, oldn)) = newn;
    }
  }
  if (newn isnt Terminal) {
    *Map_Parent(m, newn) = *Map_Parent(m, oldn);
  }
}

static void Map_Rotate_Left(struct Map* m, var node) {
  var r = *Map_Right(m, node);
  Map_Replace(m, node, r);
  *Map_Right(m, node) = *Map_Left(m, r);
  if (*Map_Left(m, r) isnt Terminal) {
      *Map_Parent(m, *Map_Left(m, r)) = node;
  }
  *Map_Left(m, r) = node;
  *Map_Parent(m, node) = r;
}

static void Map_Rotate_Right(struct Map* m, var node) {
  var l = *Map_Left(m, node);
  Map_Replace(m, node, l);
  *Map_Left(m, node) = *Map_Right(m, l);
  if (*Map_Right(m, l) isnt Terminal) {
    *Map_Parent(m, *Map_Right(m, l)) = node;
  }
  *Map_Right(m, l) = node;
  *Map_Parent(m, node) = l;
}

static void Map_Set_Fix(struct Map* m, var node) {
  
  while (True) {
  
    if (*Map_Parent(m, node) is Terminal) {
      Map_Set_Black(m, node);
      return;
    }
    
    if (Map_Is_Black(m, *Map_Parent(m, node))) { return; }
    
    if ((Map_Uncle(m, node) isnt Terminal)
    and (Map_Is_Red(m, Map_Uncle(m, node)))) {
      Map_Set_Black(m, *Map_Parent(m, node));
      Map_Set_Black(m, Map_Uncle(m, node));
      Map_Set_Red(m, Map_Grandparent(m, node));
      node = Map_Grandparent(m, node);
      continue;
    }
    
    if ((node is *Map_Right(m, *Map_Parent(m, node))) 
    and (*Map_Parent(m, node) is *Map_Left(m, Map_Grandparent(m, node)))) {
      Map_Rotate_Left(m, *Map_Parent(m, node));
      node = *Map_Left(m, node);
    }
    
    else
    
    if ((node is *Map_Left(m, *Map_Parent(m, node))) 
    and (*Map_Parent(m, node) is *Map_Right(m, Map_Grandparent(m, node)))) {
      Map_Rotate_Right(m, *Map_Parent(m, node));
      node = *Map_Right(m, node);
    }
    
    Map_Set_Black(m, *Map_Parent(m, node));
    Map_Set_Red(m, Map_Grandparent(m, node));
    
    if (node is *Map_Left(m, *Map_Parent(m, node))) {
      Map_Rotate_Right(m, Map_Grandparent(m, node));
    } else {
      Map_Rotate_Left(m, Map_Grandparent(m, node));
    }
    
    return;
  
  }
  
}

static void Map_Set(var self, var key, var val) {
  struct Map* m = self;
  key = cast(key, m->ktype);
  val = cast(val, m->vtype);
  
  var node = m->root;
  
  if (node is Terminal) {
    var node = Map_Alloc(m);
    assign(Map_Key(m, node), key);
    assign(Map_Val(m, node), val);
    m->root = node;
    m->nitems++;
    Map_Set_Fix(m, node);
    return;
  }
  
  while(True) {
    
    if_eq (Map_Key(m, node), key) {
      assign(Map_Key(m, node), key);
      assign(Map_Val(m, node), val);
      return;
    }
    
    if_lt (Map_Key(m, node), key) {
    
      if (*Map_Left(m, node) is Terminal) {
        var newn = Map_Alloc(m);
        assign(Map_Key(m, newn), key);
        assign(Map_Val(m, newn), val);
        *Map_Left(m, node) = newn;
        *Map_Parent(m, newn) = node;
        Map_Set_Fix(m, newn);
        m->nitems++;
        return;
      }
      
      node = *Map_Left(m, node);
      
    } else {
    
      if (*Map_Right(m, node) is Terminal) {
        var newn = Map_Alloc(m);
        assign(Map_Key(m, newn), key);
        assign(Map_Val(m, newn), val);
        *Map_Right(m, node) = newn;
        *Map_Parent(m, newn) = node;
        Map_Set_Fix(m, newn);
        m->nitems++;
        return;
      }
      
      node = *Map_Right(m, node);
      
    }
    
  }
  
}

static void Map_Rem_Fix(struct Map* m, var node) {
 
  while (True) {
  
    if (*Map_Parent(m, node) is Terminal) { return; }
    
    if (Map_Is_Red(m, Map_Sibling(m, node))) {
      Map_Set_Red(m, *Map_Parent(m, node));
      Map_Set_Black(m, Map_Sibling(m, node));
      if (node is *Map_Left(m, *Map_Parent(m, node))) {
          Map_Rotate_Left(m, *Map_Parent(m, node));
      } else {
          Map_Rotate_Right(m, *Map_Parent(m, node));
      }
    }
    
    if (Map_Is_Black(m, *Map_Parent(m, node))
    and Map_Is_Black(m, Map_Sibling(m, node))
    and Map_Is_Black(m, *Map_Left(m, Map_Sibling(m, node)))
    and Map_Is_Black(m, *Map_Right(m, Map_Sibling(m, node)))) {
      Map_Set_Red(m, Map_Sibling(m, node));
      node = *Map_Parent(m, node);
      continue;
    }
    
    if (Map_Is_Red(m, *Map_Parent(m, node))
    and Map_Is_Black(m, Map_Sibling(m, node))
    and Map_Is_Black(m, *Map_Left(m, Map_Sibling(m, node)))
    and Map_Is_Black(m, *Map_Right(m, Map_Sibling(m, node)))) {
      Map_Set_Red(m, Map_Sibling(m, node));
      Map_Set_Black(m, *Map_Parent(m, node));
      return;
    }
    
    if (Map_Is_Black(m, Map_Sibling(m, node))) {
    
      if (node is *Map_Left(m, *Map_Parent(m, node))
      and Map_Is_Red(m, *Map_Left(m, Map_Sibling(m, node)))
      and Map_Is_Black(m, *Map_Right(m, Map_Sibling(m, node)))) {
        Map_Set_Red(m, Map_Sibling(m, node));
        Map_Set_Black(m, *Map_Left(m, Map_Sibling(m, node)));
        Map_Rotate_Right(m, Map_Sibling(m, node));
      }
      
      else 
      
      if (node is *Map_Right(m, *Map_Parent(m, node))
      and Map_Is_Red(m, *Map_Right(m, Map_Sibling(m, node)))
      and Map_Is_Black(m, *Map_Left(m, Map_Sibling(m, node)))) {
        Map_Set_Red(m, Map_Sibling(m, node));
        Map_Set_Black(m, *Map_Right(m, Map_Sibling(m, node)));
        Map_Rotate_Left(m, Map_Sibling(m, node));
      }
    
    }
    
    Map_Set_Color(m, 
      Map_Sibling(m, node),  
      Map_Get_Color(m, *Map_Parent(m, node)));
    
    Map_Set_Black(m, *Map_Parent(m, node));
    
    if (node is *Map_Left(m, *Map_Parent(m, node))) {
        Map_Set_Black(m, *Map_Right(m, Map_Sibling(m, node)));
        Map_Rotate_Left(m, *Map_Parent(m, node));
    } else {
        Map_Set_Black(m, *Map_Left(m, Map_Sibling(m, node)));
        Map_Rotate_Right(m, *Map_Parent(m, node));
    }
    
    return;
  }

}

static void Map_Rem(var self, var key) {
  struct Map* m = self;
  
  key = cast(key, m->ktype);
  
  var found = False;
  var node = m->root;
  while (node isnt Terminal) {
  
    if_eq(Map_Key(m, node), key) {
      found = True;
      break;
    }
    
    node = lt(Map_Key(m, node), key) 
      ? *Map_Left(m, node) 
      : *Map_Right(m, node);
  }
  
  if (not found) {
    throw(KeyError, "Key %$ not in Map!", key);
    return;
  }
  
  destruct(Map_Key(m, node));
  destruct(Map_Val(m, node));
  
  if ((*Map_Left(m, node) isnt Terminal) 
  and (*Map_Right(m, node) isnt Terminal)) {
    var pred = Map_Maximum(m, *Map_Left(m, node));
    var ncol = Map_Get_Color(m, node);
    memcpy(node + 3 * sizeof(var), pred + 3 * sizeof(var),
      sizeof(struct CelloHeader) + m->ksize +
      sizeof(struct CelloHeader) + m->vsize);
    Map_Set_Color(m, node, ncol);
    node = pred;
  }

  var chld = *Map_Right(m, node) is Terminal 
    ? *Map_Left(m, node) 
    : *Map_Right(m, node);
    
  if (Map_Is_Black(m, node)) {
    Map_Set_Color(m, node, Map_Get_Color(m, chld));
    Map_Rem_Fix(m, node);
  }
    
  Map_Replace(m, node, chld);
  
  if ((*Map_Parent(m, node) is Terminal) and (chld isnt Terminal)) {
    Map_Set_Black(m, chld);
  }
  
  m->nitems--;
  free(node);
  
}

static var Map_Iter_Init(var self) {
  struct Map* m = self;
  if (m->nitems is 0) { return Terminal; }
  var node = m->root;
  while (*Map_Left(m, node) isnt Terminal) {
    node = *Map_Left(m, node);
  }
  return Map_Key(m, node);
}

static var Map_Iter_Next(var self, var curr) {
  struct Map* m = self;
  
  var node = curr - sizeof(struct CelloHeader) - 3 * sizeof(var);
  var prnt = *Map_Parent(m, node);
  
  if (*Map_Right(m, node) isnt Terminal) {
    node = *Map_Right(m, node);
    while (*Map_Left(m, node) isnt Terminal) {
      node = *Map_Left(m, node);
    }
    return Map_Key(m, node);
  }
  
  while (True) {
    if (prnt is Terminal) { return Terminal; }
    if (node is *Map_Left(m, prnt)) { return Map_Key(m, prnt); }
    if (node is *Map_Right(m, prnt)) {
      prnt = *Map_Parent(m, prnt);
      node = *Map_Parent(m, node);
    }
  }
  
  return Terminal;
}

static int Map_Show(var self, var output, int pos) {
  struct Map* m = self;
  
  pos = print_to(output, pos, "<'Map' At 0x%p {", self);
  
  var curr = Map_Iter_Init(self);
  
  while (curr isnt Terminal) {
    var node = curr - sizeof(struct CelloHeader) - 3 * sizeof(var);
    pos = print_to(output, pos, "%$:%$",
      Map_Key(m, node), Map_Val(m, node));
    curr = Map_Iter_Next(self, curr);
    if (curr isnt Terminal) { pos = print_to(output, pos, ", "); }
  }
  
  pos = print_to(output, pos, "}>");
  
  return pos;
}

var Map = Cello(Map,
  Member(Doc,
    Map_Name, Map_Brief, Map_Description,
    Map_Examples, Map_Methods),
  Member(New,     Map_New, Map_Del, Map_Size),
  Member(Assign,  Map_Assign),
  Member(Copy,    Map_Copy),
  Member(Eq,      Map_Eq),
  Member(Len,     Map_Len),
  Member(Get,     Map_Get, Map_Set, Map_Mem, Map_Rem),
  Member(Clear,   Map_Clear),
  Member(Iter,    Map_Iter_Init, Map_Iter_Next),
  Member(Show,    Map_Show, NULL));


