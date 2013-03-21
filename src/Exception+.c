
#include "Exception+.h"
#include "Type+.h"
#include "None+.h"

var Exception = methods {
  methods_begin(Exception),
  method(Exception, AsStr),
  methods_end(Exception)
};

const char* Exception_AsStr(var e) {
  ExceptionData* ed = cast(e, Exception);
  return ed->name;
}

__exc_record  __exc_root = { 0 };
__exc_record* __exc_top = &__exc_root;

var __exc_obj = NULL;
const char* __exc_msg = "";
const char* __exc_file = "";
const char* __exc_func = "";
unsigned int __exc_lineno = 0;

void __exc_record_push(__exc_record* er) {
  er->prev = __exc_top;
  __exc_top = er;
}

static bool __exc_record_error(void)  {
  
  /* TODO: Deal with when object doesn't implement "as_str" */
  
  fprintf(stderr, "\n");
  fprintf(stderr, "!!\t\n");
  fprintf(stderr, "!!\tUncaught '%s' at (%s:%s:%i) \n", as_str(__exc_obj), __exc_file, __exc_func, __exc_lineno);
  fprintf(stderr, "!!\t\n");
  fprintf(stderr, "!!\t\t '%s'\n", __exc_msg);
  fprintf(stderr, "!!\t\n");
  exit(EXIT_FAILURE);
  return false;
  
}

bool __exc_record_pop(__exc_record* er, var* exc, ...) {
  
  if (er->prev == NULL) { return __exc_record_error(); }
  
  va_list va;
  va_start(va, exc);
  var e = va_arg(va, var);
  
  while (e != Undefined) {
  
    if_eq(e, __exc_obj) {
      __exc_top = __exc_top->prev;
      *exc = e;
      return true;
    }
    
    e = va_arg(va, var);
  }
  
  return __exc_record_error();
  
}