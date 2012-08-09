
#include "Prelude+.h"
#include "Function+.h"
#include "List+.h"
#include "String+.h"
#include "NoneType+.h"

int main(int argc, char** argv) {
  
  lambda(hello_name, args) {
    var name = cast(pop(args), String);
    printf("Hello %s!\n", as_str(name));
    return None;
  }
  
  var names = new(List, 3, $(String, "Dan"), $(String, "Robert"), $(String, "Chris"));
  
  foreach(names, name) {
    call(hello_name, name);
  }

  delete(names);
  
  return 0;
}