
#include "ptest.h"

void suite_core(void);
void suite_functional(void);
void suite_data(void);

int main(int argc, char** argv) {
  
  pt_add_suite(suite_core);
  pt_add_suite(suite_functional);
  pt_add_suite(suite_data);
  
  pt_run();
  
}
