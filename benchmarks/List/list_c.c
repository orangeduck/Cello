#include "kvec.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
  
  kvec_t(int) x;
  kv_init(x);
  int n = 10000;
  
  for (int i = 0; i < n; i++) {
    kv_push(int, x, rand());
  }
  
  for (int i = 0; i < n; i++) {
    kv_push(int, x, 0);
    int index = rand() % n;
    memmove(
      &x.a[index+1], &x.a[index+0],
      sizeof(int) * ((x.n-1) - index));
    x.a[index] = rand();
  }
  
  for (int i = 0; i < n; i++) {
    int index = rand() % n;
    memmove(
      &x.a[index+0], &x.a[index+1],
      sizeof(int) * ((x.n-1) - index));
    kv_pop(x);
  }
  
  kv_destroy(x);
  
  return 0;
  
}
