
#include "Cello.h"

int main(int arc, char** argv) {
  
  var values = new(List, $(Int, 1), $(Int, 10), None, $(Real, 87.2));

  foreach(val in values) {
    
    var t = type_of(val);

    if (t is Int) {
      print("%s (%ld)\n", t, val);
    } else if (t is Real) {
      print("%s (%f)\n", t, val);
    } else {
      print("%s\n", t);
    }
  }

  delete(values);

  var x = $(Int, 0); 
  var y = $(Int, 0); 
  var z = $(Real, 0);
  var w = $(Real, 0);
  
  print("%$ %$ %$ %$\n", x, y, z, w);
  
  scan_from($(String, "5 10 0.21 0.11"), 0, "%$ %$ %$ %$", x, y, z, w);
  
  print("%$ %$ %$ %$\n", x, y, z, w);
  
  //scan("%$ %$ %$ %$", x, y, z, w);
  //print("%$ %$ %$ %$\n", x, y, z, w);
  
  var l = new(List, x, y, z, w);
  print("%$\n", l);
  delete(l);
  
  var t = new(Tree, String, Int);
  put(t, $(String, "Test"), $(Int, 0));
  put(t, $(String, "Ing"), $(Int, 5));
  put(t, $(String, "I am"), $(Int, 7));
  print("%$\n", t);
  delete(t);
  
}

