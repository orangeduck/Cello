#ifndef LambdaCurryPlus_h
#define LambdaCurryPlus_h

/* Thread as functional pipe */

#define lambda_pipe_thread0(name) \
  lambda(name, args) { return None; }

#define lambda_pipe_thread1(name, f0) \
  lambda(name, args) { return call_with(f0, args); }

#define lambda_pipe_thread2(name, f0, f1) \
  lambda(name, args) { return call(f1, call_with(f0, args)); }

#define lambda_pipe_thread3(name, f0, f1, f2) \
  lambda(name, args) { return call(f2, call(f1, call_with(f0, args))); }

#define lambda_pipe_thread4(name, f0, f1, f2, f3) \
  lambda(name, args) { return call(f3, call(f2, call(f1, call_with(f0, args)))); }

#define lambda_pipe_thread5(name, f0, f1, f2, f3, f4) \
  lambda(name, args) { return call(f4, call(f3, call(f2, call(f1, call_with(f0, args))))); }

#define lambda_pipe_thread6(name, f0, f1, f2, f3, f4, f5) \
  lambda(name, args) { return call(f5, call(f4, call(f3, call(f2, call(f1, call_with(f0, args)))))); }

#define lambda_pipe_thread7(name, f0, f1, f2, f3, f4, f5, f6) \
  lambda(name, args) { return call(f6, call(f5, call(f4, call(f3, call(f2, call(f1, call_with(f0, args))))))); }

#define lambda_pipe_thread8(name, f0, f1, f2, f3, f4, f5, f6, f7) \
  lambda(name, args) { return call(f7, call(f6, call(f5, call(f4, call(f3, call(f2, call(f1, call_with(f0, args)))))))); }

#define lambda_pipe_thread9(name, f0, f1, f2, f3, f4, f5, f6, f7, f8) \
  lambda(name, args) { return call(f8, call(f6, call(f5, call(f4, call(f3, call(f2, call(f1, call_with(f0, args)))))))); }

/* Thread first arguments through several functions */

#define lambda_method_thread0(name) lambda(name, args) { return None; }
  
#define lambda_method_thread1(name, f0) \
  lambda(name, args) { call_with(f0, args); return None; }
  
#define lambda_method_thread2(name, f0, f1) \
  lambda(name, args) { call_with(f0, args); call_with(f1, args); return None; }
  
#define lambda_method_thread3(name, f0, f1, f2) \
  lambda(name, args) { call_with(f0, args); call_with(f1, args); call_with(f2, args); return None; }
  
#define lambda_method_thread4(name, f0, f1, f2, f3) \
  lambda(name, args) { call_with(f0, args); call_with(f1, args); call_with(f2, args); call_with(f3, args); return None; }
  
#define lambda_method_thread5(name, f0, f1, f2, f3, f4) \
  lambda(name, args) { call_with(f0, args); call_with(f1, args); call_with(f2, args); call_with(f3, args); call_with(f4, args); return None; }
  
#define lambda_method_thread6(name, f0, f1, f2, f3, f4, f5) \
  lambda(name, args) { call_with(f0, args); call_with(f1, args); call_with(f2, args); call_with(f3, args); call_with(f4, args); call_with(f5, args); return None; }
  
#define lambda_method_thread7(name, f0, f1, f2, f3, f4, f5, f6) \
  lambda(name, args) { call_with(f0, args); call_with(f1, args); call_with(f2, args); call_with(f3, args); call_with(f4, args); call_with(f5, args); call_with(f6, args); return None; }
  
#define lambda_method_thread8(name, f0, f1, f2, f3, f4, f5, f6, f7) \
  lambda(name, args) { call_with(f0, args); call_with(f1, args); call_with(f2, args); call_with(f3, args); call_with(f4, args); call_with(f5, args); call_with(f6, args); call_with(f7, args); return None; }
  
#define lambda_method_thread9(name, f0, f1, f2, f3, f4, f5, f6, f7, f8) \
  lambda(name, args) { call_with(f0, args); call_with(f1, args); call_with(f2, args); call_with(f3, args); call_with(f4, args); call_with(f5, args); call_with(f6, args); call_with(f7, args); call_with(f8, args); return None; }
  
#endif