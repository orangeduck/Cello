/*
** Example libCello Program
*/

#include "Cello.h"

int main(int argc, char** argv) {

  var items = new(Table, String, Int,
    $(String, "test0"), $(Int, 0),
    $(String, "test1"), $(Int, 1));	
  
  srand(53124213);
  
  for (int i = 0; i < 10000; i++) {
    char strspace[100];
    itoa(rand(), strspace, 10);
    set(items, $(String, strspace), $(Int, rand()));
  }
  
  del(items);
  
  return 0;
  
  /* Collections can be looped over */
  foreach (item in items) {
    /* Types are also objects */
    var type = type_of(deref(item));
    print("Type: '%$'\n", type);
  }
  
  /* Heap objects destroyed with "delete" */
  del(items);
  
  /* Hashtables require "Eq" and "Hash" on key type */
  var prices = new(Table, String, Int);
  set(prices, $(String, "Apple"),  $(Int, 12)); 
  set(prices, $(String, "Banana"), $(Int,  6)); 
  set(prices, $(String, "Pear"),   $(Int, 55)); 
  
  var pear_price = get(prices, $(String, "Pear"));
  print("Price of a 'Pear' is %$\n", pear_price);

  /* Hashtable also supports iteration */
  foreach (key in prices) {
    show(key);
    var price = get(prices, key);
    show(price);
    print("Price of %$ is %$\n", key, price);
  }

  return 0;  
  
  /* File-like objects can "open" and "close" */
  with (file in sopen(file(), $(String, "prices.txt"), $(String, "w"))) {
  
    /* First class function object */
    function (write_pair, args) {
      
      /* Run time type-checking with "cast" */
      var key = cast(get(args, $(Int, 0)), String);
      var val = cast(get(prices, key), Int);
      
      print_to(file, 0, "%$ :: %$\n", key, val);      
      return None;
    };
    
    /* Higher order functions */
    map(prices, write_pair);
  
  }
  
  del(prices);
  
}

