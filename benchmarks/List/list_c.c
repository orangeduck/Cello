#include "kvec.h"
#include <stdlib.h>

int main(int argc, char** argv) {
  
  kvec_t(int) x;
  kv_init(x);
  int n = 10000;
  
  for (int i = 0; i < n; i++) {
    kv_push(int, x, rand());
  }
  
  for (int i = 0; i < n; i++) {
    kv_a(int, x, rand() % n) = rand();
  }
  
  /*
  for (int i = 0; i < n; i++) {
    x.erase(x.begin()+(rand() % n));
  }
  */
  
  kv_destroy(x);
  
  return 0;
  
}
