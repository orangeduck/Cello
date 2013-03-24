#include "C+.h"

int main(int arc, char** argv) {
  
  var values = new(List, 4, $(Int, 1), $(Int, 10), None, $(Real, 87.2));

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


}

