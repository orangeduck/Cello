
#include "C+.h"

static var g_pool = NULL;

static void table_fill(var x) {
  put(x, $(String, "First"),  $(Real, 0.0));
  put(x, $(String, "Second"), $(Real, 0.1));
  put(x, $(String, "Third"),  $(Real, 5.7));
  release(g_pool, x);
}

static void table_process(var x) {
  put(x, $(String, "First"), $(Real, -0.65));
  release(g_pool, x);
}

int main(int argc, char** argv) {
  
  g_pool = new(Pool);
  
  var x = retain(g_pool, new(Table, String, Real));
  
  table_fill(retain(g_pool, x));
  table_process(retain(g_pool, x));
  
  release(g_pool, x);
  
  delete(g_pool);
  
}

