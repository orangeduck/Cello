#include "Cello.h"

/* Threaded Callback */
var say_hello(var args) {
  with (mutex in get(args, $I(0))) {
    println("Hello from %$!", current(Thread));
  }
  return NULL;
}

int main(int argc, char** argv) {

  /* Create a Mutex */
  var mutex = new(Mutex);
  
  /* Create Several Threads */
  var threads = new(Array, Thread,
    new(Thread, $(Function, say_hello)),
    new(Thread, $(Function, say_hello)),
    new(Thread, $(Function, say_hello)),
    new(Thread, $(Function, say_hello)));
  
  /* Call each Thread */
  foreach (t in threads) { call(t, mutex); }
  
  /* Wait for each Thread to finish */
  foreach (t in threads) { join(t); }
  
  return 0;
}
