/* ex: set tabstop=2 shiftwidth=2 softtabstop=2 expandtab: */

#include "test.h"

/* Yeah I know... */
#undef CU_ASSERT
#define CU_ASSERT(X) CU_TEST((bool)(X));


// To Test:
//  
//  Array
//  Table
//
//  List
//  Dictionary
//  Map
//
//  Bool
//  Char
//  Int
//  Real
//  String
//
//  File
//


int init_data_suite(void) {
  return 0;
}

int build_data_suite(CU_pSuite suite) {
  return 0;
}