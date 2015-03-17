#include "Cello.h"

static var Hello;

struct Hello {
  int hello_val;
};

static var Hello_New(var self, var args) {
  struct Hello* h = self;
  h->hello_val = c_int(get(args, $(Int, 0)));
  return self;
}

static size_t Hello_Size(void) {
  return sizeof(struct Hello);
}

static bool Hello_Eq(var self, var obj) {
  struct Hello* lhs = self;
  struct Hello* rhs = cast(obj, Hello);
  return lhs->hello_val is rhs->hello_val;
}

int main(int argc, char** argv) {
  
  Hello = new(Type, $S("Hello"), $I(sizeof(struct Hello)),
    $(Size, Hello_Size),
    $(New, Hello_New, NULL),
    $(Eq, Hello_Eq));
  
  print("%$ is a %$!\n", Hello, type_of(Hello));

  var hello_obj1 = new(Hello, $I(1));
  var hello_obj2 = new(Hello, $I(2));

  print("Equal? %d\n", $I(eq(hello_obj1, hello_obj2)));
  
  del(hello_obj1);
  del(hello_obj2);
  del(Hello);
  
  return 0;
}

