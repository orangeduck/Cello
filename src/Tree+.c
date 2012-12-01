#include "Tree+.h"

#include "Array+.h"
#include "Bool+.h"
#include "None+.h"

var Tree = methods {
  methods_begin(Tree),
  method(Tree, New),
  method(Tree, Assign),
  method(Tree, Copy),
  method(Tree, Eq),
  method(Tree, Collection),
  method(Tree, Dict),
  method(Tree, Iter),
  methods_end(Tree),
};

var Tree_New(var self, va_list* args) {
  TreeData* td = cast(self, Tree);
  td->key_type = cast(va_arg(*args, var), Type);
  td->val_type = cast(va_arg(*args, var), Type);
  td->keys = new(Array, td->key_type, 0);
  td->root = NULL;
  return self;
}

var Tree_Delete(var self) {
  TreeData* td = cast(self, Tree);
  clear(self);
  delete(td->keys);
  return self;
}

void Tree_Assign(var self, var obj) {
  TreeData* other = cast(obj, Tree);
  clear(self);
  foreach(other, key) {
    var val = get(other, key);
    put(self, key, val);
  }
}

var Tree_Copy(var self) {
  TreeData* td = cast(self, Tree);
  
  var newtree = new(Tree, td->key_type, td->val_type);
  
  foreach(self, key) {
    var val = get(self, key);
    put(newtree, key, val);
  }
  return newtree;
}

var Tree_Eq(var self, var obj) {
  TreeData* td = cast(self, Tree);
  if (eq(type_of(obj), Tree)) {
		var val;
    foreach(obj, key) {
			if ((val = get(self, key)) is Undefined) {
				return False;
			}
			if_neq(get(obj, key), val) {
				return False;
			}
		}
    /* see if there exists key at the first object, which
     * doesn't exist at the second object. 
     */
    foreach(self, key) {
			if ((val = get(obj, key)) is Undefined) {
				return False;
			}
		}
		return True;
  } else {
    return False;
  }
}

int Tree_Len(var self) {
  TreeData* td = cast(self, Tree);
  return len(td->keys);
}

void Tree_Clear(var self) {
  TreeData* td = cast(self, Tree);
  while(not is_empty(self)) {
    discard(self, at(td->keys,0));
  }
}

var Tree_Contains(var self, var key) {
  TreeData* td = cast(self, Tree);
  return contains(td->keys, key);
}

static bool inorder_opt = true;

static var Tree_Next_Inorder(struct TreeNode* node) {
  
  inorder_opt = not inorder_opt;
  
  if (inorder_opt) {
    
    struct TreeNode* rnode = node->left;
    
    while(1) {
      if (rnode->right is NULL) return rnode->leaf_key;
      else rnode = rnode->right;
    }
  
  } else {

    struct TreeNode* lnode = node->right;
    
    while(1) {
      if (lnode->left is NULL) return lnode->leaf_key;
      else lnode = lnode->left;
    }
  
  }
  
}

static void Tree_Node_Delete(struct TreeNode* node) {
  destruct(node->leaf_key);
  destruct(node->leaf_val);
  free(node);
}

void Tree_Discard(var self, var key) {

  TreeData* td = cast(self, Tree);
  
  struct TreeNode** parent = &td->root;
  struct TreeNode* node = td->root;
  
  while(node != NULL) {
    
    if_eq(node->leaf_key, key) {
      
      if ((node->left is NULL) and 
          (node->right is NULL)) {
        *parent = NULL;
        Tree_Node_Delete(node);
        discard(td->keys, key);
        return;
      }
      
      if ((node->left is NULL) and
          not (node->right is NULL)) {
        *parent = node->right;
        Tree_Node_Delete(node);
        discard(td->keys, key);
        return;
      }
      
      if ((node->right is NULL) and
          not (node->left is NULL)) {
        *parent = node->left;
        Tree_Node_Delete(node);
        discard(td->keys, key);
        return;
      }
      
      if (not (node->right is NULL) and
          not (node->left is NULL)) {
        
        var inorder_key = allocate(td->key_type);
        var inorder_val = allocate(td->val_type);
        
        assign(inorder_key, Tree_Next_Inorder(node));  
        assign(inorder_val, get(self, inorder_key));
        
        discard(self, inorder_key);
        
        assign(node->leaf_key, inorder_key);
        assign(node->leaf_val, inorder_val);
        
        destruct(inorder_key);
        destruct(inorder_val);
        
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

var Tree_Get(var self, var key) {
  TreeData* td = cast(self, Tree);
  
  struct TreeNode* node = td->root;
  
  while(node != NULL) {
    if_eq(node->leaf_key, key) return node->leaf_val;
    if_lt(node->leaf_key, key) node = node->left;
    else node = node->right;
  }
  
  return Undefined;
}

static struct TreeNode* Tree_Node_New(var self, var key, var val) {
  TreeData* td = cast(self, Tree);
  
  struct TreeNode* node = malloc(sizeof(struct TreeNode));
  
  node->leaf_key = allocate(td->key_type);
  node->leaf_val = allocate(td->val_type);
  assign(node->leaf_key, key);
  assign(node->leaf_val, val);
  
  node->left = NULL;
  node->right = NULL;
  return node;    
}

void Tree_Put(var self, var key, var val) {
  TreeData* td = cast(self, Tree);
  
  struct TreeNode** parent = &td->root;
  struct TreeNode* node = td->root;
  
  while(node != NULL) {
    
    if_eq(node->leaf_key, key) {
      assign(node->leaf_val, val);
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
  
  *parent = Tree_Node_New(self, key, val);
  push(td->keys, key);
  return;
}

var Tree_Iter_Start(var self) {
  TreeData* td = cast(self, Tree);
  return iter_start(td->keys);
}

var Tree_Iter_End(var self) {
  TreeData* td = cast(self, Tree);
  return iter_end(td->keys);
}

var Tree_Iter_Next(var self, var curr) {
  TreeData* td = cast(self, Tree);
  return iter_next(td->keys, curr);
}
