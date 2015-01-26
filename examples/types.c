
#include "Cello.h"

/*
** This example shows dynamic creation of a Type on the heap.
** As the functions must be static there is some limitation,
** but with creative thinking could become a powerful tool.
*/

static var Hello;

struct Hello {
  int hello_val;
};

static var Hello_New(var self, var args) {
  struct Hello* hd = self;
  hd->hello_val = c_int(get(args, $(Int, 0)));
  return self;
}

static var Hello_Delete(var self) {
  return self;
}

static size_t Hello_Size(void) {
  return sizeof(struct Hello);
}

static var Hello_Eq(var self, var obj) {
  struct Hello* lhs = cast(self, Hello);
  struct Hello* rhs = cast(obj, Hello);
  if (lhs->hello_val == rhs->hello_val) {
    return True;
  } else {
    return False;
  }
}

int main(int argc, char** argv) {
  
  Hello = new(Type, $(String, "Hello"),
    $(New, Hello_New, Hello_Delete, Hello_Size),
    $(Eq, Hello_Eq));
  
  print("%s is a %s!\n", Hello, type_of(Hello));

  var hello_obj1 = new(Hello, $(Int, 1));
  var hello_obj2 = new(Hello, $(Int, 2));

  print("Equal? %d\n", eq(hello_obj1, hello_obj2));
  
  del(hello_obj1);
  del(hello_obj2);
  
  del(Hello);
  
  return 0;
}

