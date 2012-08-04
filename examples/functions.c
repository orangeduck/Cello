
#include "Prelude+.h"
#include "Type+.h"
#include "Function+.h"
#include "NoneType+.h"
#include "String+.h"

int main(int argc, char** argv) {

  lambda(print_names, args) {
    foreach(args, arg) {
      printf("%s\n", as_str(arg));
    }
    return None;
  }
  
  call(print_names, $(String, "Dan"), $(String, "Chess"));

}
