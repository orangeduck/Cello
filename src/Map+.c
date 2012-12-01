#include "Map+.h"

#include "List+.h"
#include "Bool+.h"
#include "None+.h"

var Map = methods {
  methods_begin(Map),
  method(Map, New),
  method(Map, Assign),
  method(Map, Copy),
  method(Map, Eq),
  method(Map, Collection),
  method(Map, Dict),
  method(Map, Iter),
  methods_end(Map),
};

var Map_New(var self, va_list* args) {
  MapData* md = cast(self, Map);
  md->keys = new(List, 0);
  md->root = NULL;
  return self;
}

var Map_Delete(var self) {
  MapData* md = cast(self, Map);
  clear(self);
  delete(md->keys);
  return self;
}

void Map_Assign(var self, var obj) {
  MapData* other = cast(obj, Map);
  clear(self);
  
  foreach(other, key) {
    var val = get(other, key);
    put(self, key, val);
  }
}

var Map_Copy(var self) {
  var newmap = new(Map);
  foreach(self, key) {
    var val = get(self, key);
    put(newmap, key, val);
  }
  return newmap;
}

var Map_Eq(var self, var obj) {
	MapData* md = cast(self, Map);
  if (eq(type_of(obj), Map)) {
		var val;
    foreach(obj, key) {
			if ((val = get(self, key)) == Undefined) {
				return False;
			}
			if_neq(get(obj, key), val) {
				return False;
			}
		}
    
    foreach(self, key) {
      if ((val = get(obj, key)) == Undefined) {
				return False;
			}
			if_neq(get(self, key), val) {
				return False;
			}
    }
		return True;
		
  } else {
    return False;
  }
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

static bool inorder_opt = true;

static var Map_Next_Inorder(struct MapNode* node) {
  
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
        free(node);
        discard(md->keys, key);
        *parent = NULL;
        return;
      }
      
      if ((node->left is NULL) and
          not (node->right is NULL)) {
        free(node);
        discard(md->keys, key);
        *parent = node->right;
        return;
      }
      
      if ((node->right is NULL) and
          not (node->left is NULL)) {
        free(node);
        discard(md->keys, key);
        *parent = node->left;
        return;
      }
      
      if (not (node->right is NULL) and
          not (node->left is NULL)) {
        
        var inorder_key = Map_Next_Inorder(node);  
        var inorder_val = get(self, inorder_key);
        
        discard(self, inorder_key);
        
        node->leaf_key = inorder_key;
        node->leaf_val = inorder_val;
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
  
  return Undefined;
}

static struct MapNode* Map_Node_New(var key, var val) {
  
  struct MapNode* node = malloc(sizeof(struct MapNode));
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
  
  while(node != NULL) {
    
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
