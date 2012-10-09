#ifndef test_h
#define test_h

#include "C+.h"

#include <CUnit/Basic.h>

#undef CU_ASSERT
#define CU_ASSERT(X) CU_TEST((bool)(X));

int init_core_suite(void);
int init_data_suite(void);
int init_function_suite(void);

int build_core_suite(CU_pSuite suite); 
int build_data_suite(CU_pSuite suite); 
int build_function_suite(CU_pSuite suite); 

#endif