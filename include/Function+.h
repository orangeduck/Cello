#ifndef FunctionPlus_h
#define FunctionPlus_h

module Function;

data {
  var type;
  var (*func_ptr)(var);
  int arg_count;
} FunctionData;

#define call_arg_count(...) ( sizeof(var[]{__VA_ARGS__}) / sizeof(var) )

#define call(x, ...) call_args(x, $(List, \
  call_arg_count(__VA_ARGS__), \
  call_arg_count(__VA_ARGS__), \
  var[]{ __VA_ARGS__ }, 0))
  
var call_args(var self, var args);



#endif