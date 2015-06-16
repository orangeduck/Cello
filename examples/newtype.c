#include "Cello.h"

/* Type Variable */
static var ExampleType;

/* Type Struct */
struct ExampleType {
  int value;
};

/* Constructor Function */
static void ExampleType_New(var self, var args) {
  struct ExampleType* h = self;
  h->value = c_int(get(args, $I(0)));
}

/* Comparison Function */
static int ExampleType_Cmp(var self, var obj) {
  struct ExampleType* lhs = self;
  struct ExampleType* rhs = cast(obj, ExampleType);
  return lhs->value - rhs->value;
}

int main(int argc, char** argv) {
  
  /* Construct `ExampleType` type dynamically */
  ExampleType = new_root(Type,
    $S("ExampleType"),              /* Type Name */ 
    $I(sizeof(struct ExampleType)), /* Type Size */ 
    $(New, ExampleType_New, NULL),  /* Type Interfaces */
    $(Cmp, ExampleType_Cmp));       /* ... */
  
  print("%$ is a %$!\n", ExampleType, type_of(ExampleType));

  /* We can now make `ExampleType` objects */
  var obj0 = new(ExampleType, $I(1));
  var obj1 = new(ExampleType, $I(2));
  
  /* Test for comparison etc... */
  print("Is %$ less Than %$? %s\n", 
    obj0, obj1, lt(obj0, obj1) ? $S("Yes") : $S("No"));
  
  /* Type objects must remain around longer than their instances */
  del(obj0); del(obj1);
  
  del_root(ExampleType);
  
  return 0;
  
}

