
#include "ptest.h"

void core_suite(void);
void functional_suite(void);
void data_suite(void);

int main(int argc, char** argv) {
  
  pt_add_suite(core_suite);
  pt_add_suite(functional_suite);
  pt_add_suite(data_suite);
  
  pt_run();
  
}
