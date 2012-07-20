#include <stdlib.h>
#include <stdio.h>

#include "Prelude+.h"
#include "String+.h"
#include "Char+.h"
#include "Number+.h"
#include "List+.h"
#include "NoneType+.h"
#include "HashTable+.h"

int main(int argc, char** argv) {
  
  // String allocated on heap using constructor
  var hello1 = new(String, "Hello");
  
  // String allocated on stack using literal
  var hello2 = $(String, "Hello");
  
  // polymorphic len & as_str functions
  printf("String len %i Is: '%s'\n", len(hello1), as_str(hello1));
  
  // Equality function overloaded for actual string comparison
  if ( eq(hello1, hello2) ) {
    printf("Strings are equal!\n");
  }
  
  delete(hello1);
  
  // Lists easy to create.
  var numbers = new(List, 3, $(Int, 1), $(Real, 10.12), $(Int, 6));
  
  // Lists implement "Iter" so "foreach" can work on them.
  foreach(numbers, num) {
    
    // Types are data too.
    // "is" is just "==" i.e non overloaded equality.
    // Similar semantics to python "is"
    
    if (type_of(num) is Int) {
      printf("Int Number: %li\n", as_long(num));
    } elif (type_of(num) is Real) {
      printf("Real Number: %f\n", as_double(num));
    }
    
  }
  
  // Heap objects need freeing
  delete(numbers);
  
  var prices = new(HashTable);
  
  put(prices, $(String, "Apple"),  $(Int, 12)); 
  put(prices, $(String, "Banana"), $(Int,  6)); 
  put(prices, $(String, "Pear"),   $(Int, 55)); 
  
  var pear_price = get(prices, $(String, "Pear"));
  
  printf("Price of a 'Pear' is '%li'\n", as_long(pear_price));
  
  foreach(prices, key) {
    var price = get(prices, key);
    
    if ( not contains(key, $(String, "na")) ) {
      printf("Price of '%s' is '%li'\n", as_str(key), as_long(price));
    }
  }
  
  delete(prices);
  
  // Stack allocation really it is quite flexible
  // Obviously this object cannot shrink and grow
  // But it can still be interated over etc
  var counting = $(List, 3, 3, (var[]){ $(Int, 1), $(Real, 2.0), $(Char, 'a') });
  
  foreach(counting, item) {
    // Types are rich objects too e.g overloaded as_str
    printf("Type: %s\n", as_str(type_of(item)));
  }
  
  printf("Type of '%s' is '%s'\n", as_str(None), as_str(type_of(None)));
  
  var testlist = new(List, 0);
  for(int i = 0; i < 10000; i++) {
    push(testlist, $(Int, 1));
  }
  for(int i = 0; i < 10000; i++) {
    var item = pop(testlist);
  }
  
  
  return 0;
}