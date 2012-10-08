#include "test.h"

int main(int argc, char** argv) {
  
  if (CU_initialize_registry() != CUE_SUCCESS) return CU_get_error();

  CU_pSuite core_suite = CU_add_suite("C+ Core Suite", init_core_suite, NULL);
  CU_pSuite function_suite = CU_add_suite("C+ Functional Suite", init_function_suite, NULL);
  
  int err = 0;
  
  err |= build_core_suite(core_suite);
  err |= build_function_suite(function_suite);
  
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  
  CU_cleanup_registry();
  return CU_get_error();
}
