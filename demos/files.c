
#include "Cello.h"

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
    
    print("Got String: '%s'\n", string);
    print("Got Numbers: '%i' and '%i'\n", first, second);
    
    delete(string);
    delete(first);
    delete(second);
    
  close(f);
  
}
