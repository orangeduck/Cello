#ifndef CelloLambdaCurry_h
#define CelloLambdaCurry_h

/* Uncurring lambda macros */

#define lambda_uncurry0(name, func_ptr) \
  lambda(name, args) { return func_ptr(); };

#define lambda_uncurry1(name, func_ptr) \
  lambda(name, args) { return func_ptr(at(args,0)); };

#define lambda_uncurry2(name, func_ptr) \
  lambda(name, args) { return func_ptr(at(args,0), at(args,1)); };
  
#define lambda_uncurry3(name, func_ptr) \
  lambda(name, args) { return func_ptr(at(args,0), at(args,1), at(args,2)); };
  
#define lambda_uncurry4(name, func_ptr) \
  lambda(name, args) { return func_ptr(at(args,0), at(args,1), at(args,2), at(args,3)); };
  
#define lambda_uncurry5(name, func_ptr) \
  lambda(name, args) { return func_ptr(at(args,0), at(args,1), at(args,2), at(args,3), at(args,4)); };
  
#define lambda_uncurry6(name, func_ptr) \
  lambda(name, args) { return func_ptr(at(args,0), at(args,1), at(args,2), at(args,3), at(args,4), at(args,5)); };
  
#define lambda_uncurry7(name, func_ptr) \
  lambda(name, args) { return func_ptr(at(args,0), at(args,1), at(args,2), at(args,3), at(args,4), at(args,5), at(args,6)); };
  
#define lambda_uncurry8(name, func_ptr) \
  lambda(name, args) { return func_ptr(at(args,0), at(args,1), at(args,2), at(args,3), at(args,4), at(args,5), at(args,6), at(args,7)); };
  
#define lambda_uncurry9(name, func_ptr) \
  lambda(name, args) { return func_ptr(at(args,0), at(args,1), at(args,2), at(args,3), at(args,4), at(args,5), at(args,6), at(args,7), at(args,8)); };
  
/* And for when returning void */
  
#define lambda_void_uncurry0(name, func_ptr) \
  lambda(name, args) { func_ptr(); return None; };

#define lambda_void_uncurry1(name, func_ptr) \
  lambda(name, args) { func_ptr(at(args,0)); return None; };

#define lambda_void_uncurry2(name, func_ptr) \
  lambda(name, args) { func_ptr(at(args,0), at(args,1)); return None; };
  
#define lambda_void_uncurry3(name, func_ptr) \
  lambda(name, args) { func_ptr(at(args,0), at(args,1), at(args,2)); return None; };
  
#define lambda_void_uncurry4(name, func_ptr) \
  lambda(name, args) { func_ptr(at(args,0), at(args,1), at(args,2), at(args,3)); return None; };
  
#define lambda_void_uncurry5(name, func_ptr) \
  lambda(name, args) { func_ptr(at(args,0), at(args,1), at(args,2), at(args,3), at(args,4)); return None; };
  
#define lambda_void_uncurry6(name, func_ptr) \
  lambda(name, args) { func_ptr(at(args,0), at(args,1), at(args,2), at(args,3), at(args,4), at(args,5)); return None; };
  
#define lambda_void_uncurry7(name, func_ptr) \
  lambda(name, args) { func_ptr(at(args,0), at(args,1), at(args,2), at(args,3), at(args,4), at(args,5), at(args,6)); return None; };
  
#define lambda_void_uncurry8(name, func_ptr) \
  lambda(name, args) { func_ptr(at(args,0), at(args,1), at(args,2), at(args,3), at(args,4), at(args,5), at(args,6), at(args,7)); return None; };
  
#define lambda_void_uncurry9(name, func_ptr) \
  lambda(name, args) { func_ptr(at(args,0), at(args,1), at(args,2), at(args,3), at(args,4), at(args,5), at(args,6), at(args,7), at(args,8)); return None; };
  

#endif
