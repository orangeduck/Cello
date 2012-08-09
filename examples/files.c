
#include "Prelude+.h"
#include "String+.h"
#include "Number+.h"
#include "File+.h"

int main(int arc, char** argv) {
  
  var tmp = $(File, NULL);
  
  with_as( open(tmp, "test.bin", "w"), f) {
    
    put(f, String, $(String, "Hello"));
    put(f, Int, $(Int, 1));
    put(f, Int, $(Int, 22));
  
  };
  
  with_as( open(tmp, "test.bin", "r"), f) {
    
    var string = get(f, String);
    var first = get(f, Int);
    var second = get(f, Int);
    
    printf("Got String: '%s'\n", as_str(string));
    printf("Got Numbers: '%ld' and '%ld'", as_long(first), as_long(second));
    
    delete(string);
    delete(first);
    delete(second);
    
  }
  
}