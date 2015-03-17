#include "Cello.h"

int main(int argc, char** argv) {

  var mutex = new(Mutex);
  
  fun (f, args) {
    with (m in mutex) {
      println("Hello from %$!", current(Thread));
    }
    return NULL;
  };
  
  var threads = new(Array, Box,
    new(Thread, f), new(Thread, f),
    new(Thread, f), new(Thread, f),
    new(Thread, f));
  
  foreach(t in threads) { call(t); }
  foreach(t in threads) { join(t); }
  
  del(threads);
  del(mutex);
  
  return 0;
}
