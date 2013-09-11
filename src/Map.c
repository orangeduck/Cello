#include "Cello/Map.h"

#include "Cello/List.h"
#include "Cello/Bool.h"
#include "Cello/None.h"
#include "Cello/Exception.h"

#include <string.h>

struct MapNode {
  var leaf_key;
  var leaf_val;
  struct MapNode* left;
  struct MapNode* right;
};

data {
  var type;
  var keys;
  struct MapNode* root;
} MapData;

var Map = type_data {
  type_begin(Map),
  type_entry(Map, New),
  type_entry(Map, Assign),
  type_entry(Map, Copy),
  type_entry(Map, Eq),
  type_entry(Map, Collection),
  type_entry(Map, Dict),
  type_entry(Map, Iter),
  type_entry(Map, Show),
  type_end(Map),
};

var Map_New(var self, var_list vl) {
  MapData* md = cast(self, Map);
  md->keys = new(List);
  md->root = NULL;
  return self;
}

var Map_Delete(var self) {
  MapData* md = cast(self, Map);
  clear(self);
  delete(md->keys);
  return self;
}

size_t Map_Size(void) {
  return sizeof(MapData);
}

void Map_Assign(var self, var obj) {
  MapData* other = cast(obj, Map);
  clear(self);
  
  foreach(key in other) {
    var val = get(other, key);
    put(self, key, val);
  }
}

var Map_Copy(var self) {
  var newmap = new(Map);
  foreach(key in self) {
    var val = get(self, key);
    put(newmap, key, val);
  }
  return newmap;
}

var Map_Eq(var self, var obj) {
	MapData* md = cast(self, Map);
  if_neq(type_of(obj), Map) { return False; }
  
  foreach(key in obj) {
		if (not contains(self, key)) { return False; }
		if_neq(get(obj, key), get(self, key)) { return False; }
	}
  
  foreach(key in self) {
    if (not contains(obj, key)) { return False; }
		if_neq(get(self, key), get(obj, key)) { return False; }
  }
  
	return True;
}

int Map_Len(var self) {
  MapData* md = cast(self, Map);
  return len(md->keys);
}

void Map_Clear(var self) {
  MapData* md = cast(self, Map);
  
  while(not is_empty(self)) {
    discard(self, at(md->keys,0));
  }
}

var Map_Contains(var self, var key) {
  MapData* md = cast(self, Map);
  return contains(md->keys, key);
}

local bool inorder_opt = true;

local var Map_Next_Inorder(struct MapNode* node) {
  
  inorder_opt = not inorder_opt;
  
  if (inorder_opt) {
    
    struct MapNode* rnode = node->left;
    
    while(1) {
      if (rnode->right is NULL) return rnode->leaf_key;
      else rnode = rnode->right;
    }
  
  } else {

    struct MapNode* lnode = node->right;
    
    while(1) {
      if (lnode->left is NULL) return lnode->leaf_key;
      else lnode = lnode->left;
    }
  
  }
  
}

void Map_Discard(var self, var key) {
  MapData* md = cast(self, Map);
  
  struct MapNode** parent = &md->root;
  struct MapNode* node = md->root;
  
  while(node != NULL) {
    
    if_eq(node->leaf_key, key) {
      
      if ((node->left is NULL) and 
          (node->right is NULL)) {
        *parent = NULL;
        free(node);
        discard(md->keys, key);
        return;
      }
      
      if ((node->left is NULL) and
          not (node->right is NULL)) {
        *parent = node->right;
        free(node);
        discard(md->keys, key);
        return;
      }
      
      if ((node->right is NULL) and
          not (node->left is NULL)) {
        *parent = node->left;
        free(node);
        discard(md->keys, key);
        return;
      }
      
      if (not (node->right is NULL) and
          not (node->left is NULL)) {
        var inorder_key = Map_Next_Inorder(node);  
        var inorder_val = get(self, inorder_key);
        
        discard(self, inorder_key);
        
        node->leaf_key = inorder_key;
        node->leaf_val = inorder_val;
        
        discard(md->keys, key);
        return;
      }
          
    }
    
    if_lt(node->leaf_key, key) {
      parent = &node->left;
      node = node->left;
    } else {
      parent = &node->right;
      node = node->right;
    }
    
  }
  
}

var Map_Get(var self, var key) {
  MapData* md = cast(self, Map);
  
  struct MapNode* node = md->root;
  
  while(node != NULL) {
    if_eq(node->leaf_key, key) return node->leaf_val;
    if_lt(node->leaf_key, key) node = node->left;
    else node = node->right;
  }
  
  return throw(KeyError, "Key '%$' not in Map!", key);
}

local struct MapNode* Map_Node_New(var key, var val) {
  struct MapNode* node = malloc(sizeof(struct MapNode));
  
  if (node == NULL) { throw(OutOfMemoryError, "Cannot create new Map Node. Out of memory!"); }
  
  node->leaf_key = key;
  node->leaf_val = val;
  node->left = NULL;
  node->right = NULL;
  return node;    
}

void Map_Put(var self, var key, var val) {
  MapData* md = cast(self, Map);
  
  struct MapNode** parent = &md->root;
  struct MapNode* node = md->root;
  
  while (node != NULL) {
    
    if_eq(node->leaf_key, key) {
      node->leaf_val = val;
      return;
    }
    
    if_lt(node->leaf_key, key) {
      parent = &node->left;
      node = node->left;
    } else {
      parent = &node->right;
      node = node->right;
    }
    
  }
  
  *parent = Map_Node_New(key, val);
  push(md->keys, key);
  return;
  
}

var Map_Iter_Start(var self) {
  MapData* md = cast(self, Map);
  return iter_start(md->keys);
}

var Map_Iter_End(var self) {
  MapData* md = cast(self, Map);
  return iter_end(md->keys);
}

var Map_Iter_Next(var self, var curr) {
  MapData* md = cast(self, Map);
  return iter_next(md->keys, curr);
}

int Map_Show(var self, var output, int pos) {
  MapData* md = cast(self, Map);
  
  pos = print_to(output, pos, "<'Map' At 0x%p {", self);
  
  for(int i = 0; i < len(self); i++) {
    var key = at(md->keys, i);
    var val = get(self, key);
    pos = print_to(output, pos, "%$:%$", key, get(self, key));
    if (i < len(self)-1) { pos = print_to(output, pos, ", "); }
  }
  
  pos = print_to(output, pos, "}>");
  
  return pos;
}

