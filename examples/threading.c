#include <unistd.h>

#include "Cello.h"

int main(int argc, char** argv) {

  var mut = new(Mutex);
  
  function (thread_function, args) {
    with(m in mut) {
      println("Hello from %$! with Arguments %$", current(Thread), args);
    }
    return Undefined;
  };
  
  var threads = new(Array, Box,
    new(Thread, thread_function),
    new(Thread, thread_function),
    new(Thread, thread_function),
    new(Thread, thread_function),
    new(Thread, thread_function));
  
  foreach(t in threads) {
    call(t);
  }
  
  foreach(t in threads) {
    join(t);
  }
  
  del(threads);
  del(mut);
  
  return 0;

}
