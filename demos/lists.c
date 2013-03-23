#include "C+.h"

int main(int arc, char** argv) {
  
  var values = new(List, 4, $(Int, 1), $(Int, 10), None, $(Real, 87.2));

  foreach(val in values) {
    
    var t = type_of(val);

    if (t is Int) {
      printf("%s (%ld)\n", as_str(t), as_long(val));
    } else if (t is Real) {
      printf("%s (%f)\n", as_str(t), as_double(val));
    } else {
      printf("%s\n", as_str(t));
    }
  }

  delete(values);


}

