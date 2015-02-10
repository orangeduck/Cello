#include "Cello.h"

int main(int argc, char** argv) {

  var items = new(Array, Int, 
    $I( 8), $I( 5), $I(20), 
    $I(15), $I(16), $I(98));

  foreach (i in range($I(len(items)))) {
    print("Item Range %i is %i\n", i, get(items, i));
  }

  foreach (item in slice(items, $I(0), $I(len(items)), $I(2))) {
    print("Item Slice %i\n", item);
  }
  
  del(items);

  return 0;
}

