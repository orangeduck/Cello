
#include "ptest.h"

void suite_core(void);
void suite_data(void);

#if !defined(_MSC_VER)
void suite_functional(void);
void suite_threading(void);
#endif

void suite_exception(void);

int main(int argc, char** argv) {
  
  pt_add_suite(suite_core);
  pt_add_suite(suite_data);

#if !defined(_MSC_VER)
  pt_add_suite(suite_functional);
  pt_add_suite(suite_threading);
#endif

  pt_add_suite(suite_exception);
  
  pt_run();
  
}
