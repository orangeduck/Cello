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

PT_FUNC(test_new) {
  
  var in_func = $(Ref, False);
  
  fun (f, args) {
    ref(in_func, True);
    return None;
  };
  
  var t = new(Thread, f);
  
  PT_ASSERT(not deref(in_func));
  call(t);
  join(t);
  PT_ASSERT(deref(in_func));
  
  del(t);

}

PT_FUNC(test_multiple) {
  
  var inside = new(Array, Ref, False, False, False, False, False);
  
  fun (f, a) {
    set(inside, get(a, $I(0)), True);
    return None;
  };
  
  var threads = new(Array, Box,
    new(Thread, f), new(Thread, f),
    new(Thread, f), new(Thread, f),
    new(Thread, f));
  
  var args = new(Array, Int,
    $(Int, 0), $(Int, 1), $(Int, 2),
    $(Int, 3), $(Int, 4));
  
  foreach(i in range(len(threads))) {
    call(deref(get(threads, i)), get(args, i));
  }
  
  foreach(t in threads) {
    join(deref(t));
  }
  
  foreach(i in inside) {
    PT_ASSERT(deref(i));
  }
  
  del(args);
  del(threads);
  del(inside);
  
}

PT_FUNC(test_mutex) {
  
  var mutex = new(Mutex);
  var total = $(Int, 0);
  
  fun (f, args) {
    with(m in mutex) {
      minc(total);
    }
    return None;
  };
  
  var threads = new(Array, Box,
    new(Thread, f), new(Thread, f),
    new(Thread, f), new(Thread, f),
    new(Thread, f));
  
  PT_ASSERT(eq(total, $(Int, 0)));
  
  foreach(t in threads) {
    call(deref(t));
  }
  
  foreach(t in threads) {
    join(deref(t));
  }
  
  PT_ASSERT(eq(total, $(Int, 5)));
  
  del(threads);
  del(mutex);
  
}

PT_FUNC(test_exception) {
  
  fun (f, args) {
    try {
      cello_sleep(20);
      PT_ASSERT(exception_depth() is 1);
    } catch(e) { }
    return None;
  };
  
  var t = new(Thread, f);
  
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
