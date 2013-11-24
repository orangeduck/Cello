/*
** Example libCello Program
*/

#include "Cello.h"

int main(int argc, char** argv) {

  /* Stack objects are created using "$" */
  var int_item = $(Int, 5);
  var float_item = $(Real, 2.4);
  var string_item = $(String, "Hello");

  /* Heap objects are created using "new" */
  var items = new(List, int_item, float_item, string_item);
	
  /* Collections can be looped over */
  foreach(item in items) {
    /* Types are also objects */
    var type = type_of(item);
    print("Type: '%$'\n", type);
  }
  
  /* Heap objects destroyed with "delete" */
  delete(items);
  
  /* Hashtables require "Eq" and "Hash" on key type */
  var prices = new(Table, String, Int);
  put(prices, $(String, "Apple"),  $(Int, 12)); 
  put(prices, $(String, "Banana"), $(Int,  6)); 
  put(prices, $(String, "Pear"),   $(Int, 55)); 
  
  var pear_price = get(prices, $(String, "Pear"));
  print("Price of a 'Pear' is %$\n", pear_price);

  /* Hashtable also supports iteration */
  foreach(key in prices) {
    var price = get(prices, key);
    print("Price of %$ is %$\n", key, price);
  }
  
  /* File-like objects can "open" and "close" */
  with(file in stream_open($(File, NULL), "prices.bin", "wb")) {
  
    /* First class function object */
    lambda(write_pair, args) {
      
      /* Run time type-checking with "cast" */
      var key = cast(at(args, 0), String);
      var val = cast(get(prices, key), Int);
      
      /* File implements "put/get" like Hashtable */
      print_to(file, 0, "%$ :: %$\n", key, val);      
      return None;
    };
    
    /* Higher order functions */
    map(prices, write_pair);
  
  }
  
  delete(prices);
  
}

