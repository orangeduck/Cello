
#include "Prelude+.h"
#include "String+.h"
#include "Number+.h"
#include "File+.h"

int main(int arc, char** argv) {
  
  with(file in open($(File, NULL), "test.bin", "w")) {
    
    put(file, String, $(String, "Hello"));
    put(file, Int, $(Int, 1));
    put(file, Int, $(Int, 22));
  
  }
  
  var f = $(File, NULL);
  open(f, "test.bin", "r");
    
    var string = get(f, String);
    var first = get(f, Int);
    var second = get(f, Int);
    
    printf("Got String: '%s'\n", as_str(string));
    printf("Got Numbers: '%ld' and '%ld'", as_long(first), as_long(second));
    
    delete(string);
    delete(first);
    delete(second);
    
  close(f);
  
}
