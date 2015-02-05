#include "Cello.h"

static void test(void* x) {
  printf("Goodbye %p!\n", *(void**)x);
}

void* foo(void) {
  return 1;
}

int main( int argc, char** argv) {
  
  void* x = foo(); void* __attribute__((__cleanup__(test))) y = foo();
  
  printf("Hello!\n");
  
}

