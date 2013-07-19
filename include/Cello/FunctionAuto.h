#ifndef CelloFunctionAuto_h
#define CelloFunctionAuto_h


#define auto_lambda0(func) var __Plus_##func(var self, var args) { return func(); }; \
__Plus_##func

#define auto_lambda1(func) \
var __Plus_##func(var self, var args) { var a0 = at(args, 0) return func(a0); }; \


#endif
