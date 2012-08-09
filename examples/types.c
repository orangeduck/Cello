
#include "Prelude+.h"
#include "Type+.h"

/*
** This example shows dynamic creation of a Type on the heap.
** As the functions must be static there is some limitation,
** but with creative thinking could become a powerful tool.
*/

static Type Hello;

data {
  var type;
  int hello_val;
} HelloData

static var Hello_New(var self, va_list* args) {
  HelloData* hd = cast(self, Hello);
  hd->hello_val = va_arg(*args, int);
  return self;
}

static var Hello_Delete(var self) {
  return self;
}

static bool Hello_Eq(var self, var obj) {
  HelloData* lhs = cast(self, Hello);
  HelloData* rhs = cast(obj, Hello);
  return (lhs->hello_val == rhs->hello_val);
}

int main(int argc, char** argv) {
  
  instance(Hello, New) = { sizeof(HelloData), Hello_New, Hello_Delete };
  instance(Hello, Eq) = { Hello_Eq };
 
  Hello = new(Type, "Hello", 2, 
    (var[]){ method(Hello, New), method(Hello, Eq) }, 
    ((const char*)[]){ "New", "Eq" } );

  printf("%s!", as_str(Hello));
 
  return 0;
}

