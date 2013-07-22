
#include "Cello.h"

int main(int arc, char** argv) {
  
  with(file in stream_open($(File, NULL), "test.bin", "w")) {
    
    put(file, Int, $(Int, 1));
    put(file, Int, $(Int, 22));
  
  }
  
  var f = $(File, NULL);
  stream_open(f, "test.bin", "r");
    
    var first = get(f, Int);
    var second = get(f, Int);
    
    print("Got Numbers: '%i' and '%i'\n", first, second);
    
    delete(first);
    delete(second);
    
  stream_close(f);
  
}
