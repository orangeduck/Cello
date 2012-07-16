#include <stdlib.h>
#include <stdio.h>

#include "Prelude+.h"
#include "String+.h"
#include "Number+.h"
#include "List+.h"
#include "HashTable+.h"

int main(int argc, char** argv) {
  
  // String allocated on heap using constructor
  var hello1 = new(String, "Hello");
  
  // String allocated on stack using literal
  var hello2 = $(String, "Hello");
  
  // Show length and also extract c string
  printf("String len %i Is: '%s'\n", len(hello1), as_str(hello1));
  
  // Equality function does actual string comparison
  if ( eq(hello1, hello2) ) {
    printf("Strings are equal!\n");
  }
  
  delete(hello1);
  
  // Lists easy to create using literals.
  var numbers = new(List, 3, $(Int, 1), $(Real, 10.12), $(Int, 6));
  
  // Lists implement "Iter" so "foreach" can work on them.
  foreach(numbers, num) {
    
    // Types are data too.
    if (type_of(num) is Int) {
      printf("Int Number: %li\n", as_long(num));
    
    // "is" is just "==" i.e non overloaded equality.
    } elif (type_of(num) is Real) {
      printf("Real Number: %f\n", as_double(num));
    }
    
  }
  
  // Heap objects need freeing
  delete(numbers);
  
  var prices = new(HashTable);
  
  put(prices, new(String, "Apple"),  new(Int, 12)); 
  put(prices, new(String, "Banana"), new(Int,  6)); 
  put(prices, new(String, "Pear"),   new(Int, 55)); 
  
  var pear_price = get(prices, $(String, "Pear"));
  
  printf("Price of a 'Pear' is '%li'\n", as_long(pear_price));
  
  foreach(prices, key) {
    var price = get(prices, key);
    printf("Price of '%s' is '%li'\n", as_str(key), as_long(price));
  }
  
  // Stored keys and values are deleted with table.
  delete(prices);
  
  return 0;
}