
#include "Cello.h"

int main(int arc, char** argv) {
  
  with(file in sopen(file(), "test.bin", "w")) {
    
    put(file, Int, $(Int, 1));
    put(file, Int, $(Int, 22));
  
  }
  
  var f = file();
  sopen(f, "test.bin", "r");
    
    var first = get(f, Int);
    var second = get(f, Int);
    
    print("Got Numbers: '%i' and '%i'\n", first, second);
    
    delete(first);
    delete(second);
    
  sclose(f);
  
}
