#include "Cello.h"

int main(int argc, char** argv) {
  
  /* Property that object is Equal to Self */
  fun (test_eq_id, args) {
    var x = get(args, $I(0));
    return (var)eq(x, x);
  };
  
  /* Quickcheck property for some Types */
  quickcheck(test_eq_id, Int);
  quickcheck(test_eq_id, String);
  quickcheck(test_eq_id, Map);
  quickcheck(test_eq_id, Array);
  
  return 0;
}

