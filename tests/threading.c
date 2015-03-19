#include "Cello.h"
#include "ptest.h"

#ifdef CELLO_UNIX
#include <unistd.h>
#endif

void cello_sleep(int ms) {
#if defined(CELLO_UNIX)
  usleep(ms * 1000);
#elif defined(CELLO_WINDOWS)
  Sleep(ms);
#endif
}

static var set_one(var args) {
  assign(get(args, $I(0)), $I(1));
  return NULL;
}

PT_FUNC(test_new) {
  
  var in_func = $I(0);
  
  var t = new(Thread, $(Function, set_one));
  
  PT_ASSERT(not c_int(in_func));
  call(t, in_func);
  join(t);
  PT_ASSERT(c_int(in_func));
  
  del(t);

}

static var set_inside(var args) {
  var inside = get(args, $I(0));
  var index  = get(args, $I(1));
  set(inside, index, $I(1));
  return NULL;
}  

PT_FUNC(test_multiple) {
  
  var inside = new(Array, Int, $I(0), $I(0), $I(0), $I(0), $I(0));
  
  var threads = new(Array, Box,
    new(Thread, $(Function, set_inside)), new(Thread, $(Function, set_inside)),
    new(Thread, $(Function, set_inside)), new(Thread, $(Function, set_inside)),
    new(Thread, $(Function, set_inside)));
  
  var args = new(Array, Int,
    $I(0), $I(1), $I(2),
    $I(3), $I(4));
  
  foreach(i in range($I(len(threads)))) {
    call(deref(get(threads, i)), inside, get(args, i));
  }
  
  foreach(t in threads) {
    join(deref(t));
  }
  
  foreach(i in inside) {
    PT_ASSERT(c_int(i));
  }
  
  del(args);
  del(threads);
  del(inside);
  
}

static var increment(var args) {
  var mut = get(args, $I(0));
  var tot = get(args, $I(1));
  with (m in mutex) {
    assign(tot, $I(c_int(tot)+1));
  }
  return NULL;
}

PT_FUNC(test_mutex) {
  
  var mutex = new(Mutex);
  var total = $I(0);
  
  var threads = new(Array, Box,
    new(Thread, $(Function, increment)), new(Thread, $(Function, increment)),
    new(Thread, $(Function, increment)), new(Thread, $(Function, increment)),
    new(Thread, $(Function, increment)));
  
  PT_ASSERT(eq(total, $I(0)));
  
  foreach(t in threads) {
    call(deref(t), mutex, total);
  }
  
  foreach(t in threads) {
    join(deref(t));
  }
  
  PT_ASSERT(eq(total, $I(5)));
  
  del(threads);
  del(mutex);
  
}

static var exception_sleep(var args) {
  try {
    cello_sleep(20);
    PT_ASSERT(exception_depth() is 1);
  } catch(e) { }
  return NULL;
}

PT_FUNC(test_exception) {
  
  var t = new(Thread, $(Function, exception_sleep));
  
  call(t);
  cello_sleep(10);
  PT_ASSERT(exception_depth() is 0);
  join(t);
  
  del(t);
  
}

PT_SUITE(suite_threading) {
  
  PT_REG(test_new);
  PT_REG(test_multiple);
  PT_REG(test_mutex);
  PT_REG(test_exception);
  
};
