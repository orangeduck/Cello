#include "Cello/Tree.h"

#include "Cello/Array.h"
#include "Cello/Bool.h"
#include "Cello/None.h"
#include "Cello/Exception.h"

#include <string.h>

struct TreeNode {
  var leaf_key;
  var leaf_val;
  struct TreeNode* left;
  struct TreeNode* right;
};

data {
  var type;
  var key_type;
  var val_type;
  var keys;
  struct TreeNode* root;
} TreeData;

var Tree = type_data {
  type_begin(Tree),
  type_entry(Tree, New),
  type_entry(Tree, Assign),
  type_entry(Tree, Copy),
  type_entry(Tree, Eq),
  type_entry(Tree, Collection),
  type_entry(Tree, Dict),
  type_entry(Tree, Iter),
  type_entry(Tree, Show),
  type_end(Tree),
};

var Tree_New(var self, var_list vl) {
  TreeData* td = cast(self, Tree);
  td->key_type = cast(var_list_get(vl), Type);
  td->val_type = cast(var_list_get(vl), Type);
  td->keys = new(Array, td->key_type);
  td->root = NULL;
  return self;
}

var Tree_Delete(var self) {
  TreeData* td = cast(self, Tree);
  clear(self);
  delete(td->keys);
  return self;
}

size_t Tree_Size(void) {
  return sizeof(TreeData);
}

void Tree_Assign(var self, var obj) {
  TreeData* other = cast(obj, Tree);
  clear(self);
  foreach(key in other) {
    var val = get(other, key);
    put(self, key, val);
  }
}

var Tree_Copy(var self) {
  TreeData* td = cast(self, Tree);
  
  var newtree = new(Tree, td->key_type, td->val_type);
  
  foreach(key in self) {
    var val = get(self, key);
    put(newtree, key, val);
  }
  return newtree;
}

var Tree_Eq(var self, var obj) {
  TreeData* td = cast(self, Tree);
  if_neq(type_of(obj), Tree) { return False; }
  
  foreach(key in obj) {
    if (not contains(self, key)) { return False; }
    if_neq(get(obj, key), get(self, key)) { return False; }
  }
	
  foreach(key in self) {
    if (not contains(obj, key)) { return False; }
    if_neq(get(obj, key), get(self, key)) { return False; }
  }
	
  return True;
	
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

local bool inorder_opt = true;

local var Tree_Next_Inorder(struct TreeNode* node) {
  
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

local void Tree_Node_Delete(struct TreeNode* node) {
  delete(node->leaf_key);
  delete(node->leaf_val);
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
        
        discard(td->keys, key);
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
  
  return throw(KeyError, "Key '%$' not in Tree!", key);
}

local struct TreeNode* Tree_Node_New(var self, var key, var val) {
  TreeData* td = cast(self, Tree);
  
  struct TreeNode* node = malloc(sizeof(struct TreeNode));
  
  if (node == NULL) { throw(OutOfMemoryError, "Cannot create Tree Node. Out of memory!"); }
  
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

int Tree_Show(var self, var output, int pos) {
  TreeData* td = cast(self, Tree);
  
  pos = print_to(output, pos, "<'Tree' At 0x%p {", self);
  
  for(int i = 0; i < len(self); i++) {
    var key = at(td->keys, i);
    var val = get(self, key);
    pos = print_to(output, pos, "%$:%$", key, get(self, key));
    if (i < len(self)-1) { pos = print_to(output, pos, ", "); }
  }
  
  pos = print_to(output, pos, "}>");
  
  return pos;
}

