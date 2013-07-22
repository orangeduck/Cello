#include <unistd.h>

#include "Cello.h"

int main(int argc, char** argv) {
  
  srand(time(NULL));
  
  var mut = new(Mutex);
  
  lambda(thread_function, args) {
    with(m in mut) {
      println("Hello from %$! with Arguments %$", current(Thread), args);
    }
  }
  
  var threads = new(List, 5,
    new(Thread, thread_function),
    new(Thread, thread_function),
    new(Thread, thread_function),
    new(Thread, thread_function),
    new(Thread, thread_function));
  
  foreach(t in threads) {
    int a0 = rand();
    int a1 = rand();
    call(t, $(Int, a0), $(Int, a1));
    sleep(1);
  }
  
  foreach(t in threads) {
    join(t);
    delete(t);
  }
  
  delete(threads);
  delete(mut);
  
  return 0;

}
