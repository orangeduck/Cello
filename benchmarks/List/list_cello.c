#include "Cello.h"

int main(int argc, char** argv) {
  
  var x = new(Array, Int);
  
  int n = 10000;
  
  for (int i = 0; i < n; i++) {
    push(x, $I(gen_c_int()));
  }
  
  for (int i = 0; i < n; i++) {
    push_at(x, $I(gen_c_int()), $I(gen_c_int() % n));
  }
  
  for (int i = 0; i < n; i++) {
    pop_at(x, $I(gen_c_int() % n));
  }
  
  del(x);
  
  return 0;
  
}
