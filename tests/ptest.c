#include "ptest.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/* Asserts */

static int num_asserts = 0;
static int num_assert_passes = 0;
static int num_assert_fails  = 0;

static bool test_passing = false;
static bool suite_passing = false;

#define MAX_ERROR 2048

static char assert_error[MAX_ERROR];
static char assert_error_buffer[MAX_ERROR];
static int assert_error_counter = 0;

void pt_assert_run(bool result, const char* expr, const char* func, const char* file, int line) {
  
  num_asserts++;
  test_passing = test_passing && result;
  
  if (result) {
    num_assert_passes++;
  } else {
    sprintf(assert_error_buffer, "        %i. Assert Failed - %s - (%s:%i)\n", assert_error_counter+1, expr, file, line );
    strcat(assert_error, assert_error_buffer);
    assert_error_counter++;
    num_assert_fails++;
  }
  
}

static void ptest_signal(int sig) {
  
  test_passing = false;
  
  switch( sig ) {
    case SIGFPE:  sprintf(assert_error_buffer, "        %i. Division by Zero\n", assert_error_counter+1); break;
    case SIGILL:  sprintf(assert_error_buffer, "        %i. Illegal Instruction\n", assert_error_counter+1); break;
    case SIGSEGV: sprintf(assert_error_buffer, "        %i. Segmentation Fault\n", assert_error_counter+1); break;
  }
  
  strcat(assert_error, assert_error_buffer);
  assert_error_counter++;
  
  printf("Failed! \n");
  printf("\n%s\n", assert_error);
  printf("    | Stopping Execution.\n");
  fflush(stdout);
  exit(0);
  
}

/* Tests */

#define MAX_NAME_LEN 512

typedef struct {
  void (*func)(void);
  char name[MAX_NAME_LEN];
  char suite[MAX_NAME_LEN];
} test_t;

#define MAX_TESTS 2048

static test_t tests[MAX_TESTS];

static int num_tests = 0;
static int num_tests_passes = 0;
static int num_tests_fails  = 0;

void pt_add_test(void (*func)(void), const char* name, const char* suite) {
  
  if (num_tests == MAX_TESTS) {
    printf("ERROR: Exceeded maximum test count of %i!\n", MAX_TESTS); abort();
  }
  
  if (strlen(name) >= MAX_NAME_LEN) {
    printf("ERROR: Test name '%s' too long (Maximum is %i characters)\n", name, MAX_NAME_LEN); abort();
  }
  
  if (strlen(suite) >= MAX_NAME_LEN) {
    printf("ERROR: Test suite '%s' too long (Maximum is %i characters)\n", suite, MAX_NAME_LEN); abort();
  }
  
  test_t test;
  test.func = func;
  strcpy(test.name, name);
  strcpy(test.suite, suite);
  
  tests[num_tests] = test;
  num_tests++;
  
}

/* Suites */

static int num_suites = 0;
static int num_suites_passes = 0;
static int num_suites_fails  = 0;

void pt_add_suite(void (*func)(void)) {
  num_suites++;
  func();
}

/* Running */

static char current_suite[MAX_NAME_LEN];

void pt_run() {
  
  printf("    \n");
  printf("    +-------------------------------------------+\n");
  printf("    | ptest          MicroTesting Magic for C   |\n");
  printf("    |                                           |\n");
  printf("    | Daniel Holden (contact@theorangeduck.com) |\n");
  printf("    +-------------------------------------------+\n");
  printf("    \n");
  
  signal(SIGFPE, ptest_signal);
  signal(SIGILL, ptest_signal);
  signal(SIGSEGV, ptest_signal);
  
  strcpy(current_suite, "");
  
  for(int i = 0; i < num_tests; i++) {
    
    test_t test = tests[i];
    
    /* Check for transition to a new suite */
    
    if (strcmp(test.suite, current_suite)) {
      
      if (strcmp(current_suite, "")) {
        if (suite_passing) {
          num_suites_passes++;
        } else {
          num_suites_fails++;
        }
      }
    
      strcpy(current_suite, test.suite);
      printf("\n\n  ===== Running '%s' =====\n\n", current_suite);
      suite_passing = true;
    }
    
    /* Run Test */
    
    printf("    | Running '%s' ... ", test.name);
    test_passing = true;
    strcpy(assert_error, "");
    strcpy(assert_error_buffer, "");
    assert_error_counter = 0;
    
    test.func();
    
    suite_passing = suite_passing && test_passing;
    
    if (test_passing) {
      num_tests_passes++;
      printf("Passed! \n");
    } else {
      num_tests_fails++;
      printf("Failed! \n");
      printf("\n%s\n", assert_error);
    }
    
  }
  
  if (suite_passing) {
    num_suites_passes++;
  } else {
    num_suites_fails++;
  }
  
  printf("  \n");
  printf("  +---------------------------------------------------+\n");
  printf("  |                      Summary                      |\n");
  printf("  +---------++------------+-------------+-------------+\n");
  printf("  | Suites  || Total %4d | Passed %4d | Failed %4d |\n", num_suites,  num_suites_passes, num_suites_fails);
  printf("  | Tests   || Total %4d | Passed %4d | Failed %4d |\n", num_tests,   num_tests_passes,  num_tests_fails );
  printf("  | Asserts || Total %4d | Passed %4d | Failed %4d |\n", num_asserts, num_assert_passes, num_assert_fails);
  printf("  +---------++------------+-------------+-------------+\n");
  printf("  \n");
  
}