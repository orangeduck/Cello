#include <stdlib.h>
#include <stdio.h>

#include "+Prelude.h"
#include "+String.h"
#include "+Number.h"
#include "+List.h"

int main(int argc, char** argv) {
  
  /* Strings */
  
  var hello1 = new(String, "Hello");
  var hello2 = new(String, "Hello");
  
  printf("String len %i Is: '%s'\n", len(hello1), as_str(hello1));
  
  if ( eq(hello1, hello2) ) {
    printf("Strings are equal!\n");
  }
  
  delete(hello1);
  delete(hello2);
  
  /* Numbers & Lists */
  
  var numbers = new(List, 3, new(Int, 1), new(Real, 10.12), new(Int, 6));
  
  foreach(numbers, num) {
    
    if (type_of(num) is Int) {
      printf("Int Number: %li\n", as_long(num));
    } elif (type_of(num) is Real) {
      printf("Real Number: %f\n", as_double(num));
    }
    
    delete(num);
    
  }
  
  delete(numbers);
  
  return 0;
}