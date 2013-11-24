#include <unistd.h>

#include "Cello.h"

int main(int argc, char** argv) {

  var mut = new(Mutex);
  
  lambda(thread_function, args) {
    with(m in mut) {
      println("Hello from %$! with Arguments %$", current(Thread), args);
    }
    return None;
  };
  
  var threads = new(List,
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
    delete(t);
  }
  
  delete(threads);
  delete(mut);
  
  return 0;

}
