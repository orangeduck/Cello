#include "Cello.h"

int main(int argc, char** argv) {
  
  var x = new(Array, Int);
  
  int n = 10000;
  
  for (int i = 0; i < n; i++) {
    push(x, $I(rand()));
  }
  
  for (int i = 0; i < n; i++) {
    push_at(x, $I(rand()), $I(rand() % n));
  }
  
  for (int i = 0; i < n; i++) {
    pop_at(x, $I(rand() % n));
  }
  
  del(x);
  
  return 0;
  
}
