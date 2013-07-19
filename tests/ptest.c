#include "ptest.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

/* Globals */

#define MAX_NAME 512
#define MAX_ERROR 2048
#define MAX_TESTS 2048

static bool test_passing = false;
static bool suite_passing = false;

/* Colors */

enum {
  BLACK   = 0x0,
  BLUE    = 0x1,
  GREEN   = 0x2,
  AQUA    = 0x3,
  RED     = 0x4,
  PURPLE  = 0x5,
  YELLOW  = 0x6,
  WHITE   = 0x7,
  GRAY    = 0x8,
  LIGHT_BLUE    = 0x9,
  LIGHT_GREEN   = 0xA,
  LIGHT_AQUA    = 0xB,
  LIGHT_RED     = 0xC,
  LIGHT_PURPLE  = 0xD,
  LIGHT_YELLOW  = 0xE,
  LIGHT_WHITE   = 0xF,
};

#ifdef _WIN32

  #include <windows.h>

  static void pt_color(int color) {
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon, color);
  }

#else

static const char* colors[] = {
  "\x1B[0m",
  "\x1B[34m",
  "\x1B[32m",
  "\x1B[36m",
  "\x1B[31m",
  "\x1B[35m",
  "\x1B[33m",
  "\x1B[37m",
  "",
  "\x1B[34m",
  "\x1B[32m",
  "\x1B[36m",
  "\x1B[31m",
  "\x1B[35m",
  "\x1B[33m",
  "\x1B[37m"
};

  static void pt_color(int color) {
    
    printf("%s", colors[color]);
  
  }

#endif

/* Asserts */

static int num_asserts = 0;
static int num_assert_passes = 0;
static int num_assert_fails  = 0;

static char assert_err[MAX_ERROR];
static char assert_err_buff[MAX_ERROR];
static int assert_err_num = 0;

void pt_assert_run(bool result, const char* expr, const char* func, const char* file, int line) {
  
  num_asserts++;
  test_passing = test_passing && result;
  
  if (result) {
    num_assert_passes++;
  } else {
    sprintf(assert_err_buff, "        %i. Assert [ %s ] (%s:%i)\n", assert_err_num+1, expr, file, line );
    strcat(assert_err, assert_err_buff);
    assert_err_num++;
    num_assert_fails++;
  }
  
}

static void ptest_signal(int sig) {
  
  test_passing = false;
  
  switch( sig ) {
    case SIGFPE:  sprintf(assert_err_buff, "        %i. Division by Zero\n", assert_err_num+1); break;
    case SIGILL:  sprintf(assert_err_buff, "        %i. Illegal Instruction\n", assert_err_num+1); break;
    case SIGSEGV: sprintf(assert_err_buff, "        %i. Segmentation Fault\n", assert_err_num+1); break;
  }
  
  assert_err_num++;
  strcat(assert_err, assert_err_buff);
  
  pt_color(RED); printf("Failed! \n\n%s\n", assert_err); pt_color(WHITE);
  
  printf("    | Stopping Execution.\n");
  fflush(stdout);
  exit(0);
  
}

/* Tests */

static void pt_title_case(char* output, const char* input) {
  
  bool space = true;
  
  strcpy(output, input);
  
  for(unsigned int i = 0; i < strlen(output); i++) {
    
    if (output[i] == '_') {
      space = true;
      output[i] = ' ';
      continue;
    } 
    
    if (space) {
      space = false;
      output[i] = output[i] - 32;
      continue;
    }
    
  }
  
}

typedef struct {
  void (*func)(void);
  char name[MAX_NAME];
  char suite[MAX_NAME];
} test_t;

static test_t tests[MAX_TESTS];

static int num_tests = 0;
static int num_tests_passes = 0;
static int num_tests_fails  = 0;

void pt_add_test(void (*func)(void), const char* name, const char* suite) {
  
  if (num_tests == MAX_TESTS) {
    printf("ERROR: Exceeded maximum test count of %i!\n", MAX_TESTS); abort();
  }
  
  if (strlen(name) >= MAX_NAME) {
    printf("ERROR: Test name '%s' too long (Maximum is %i characters)\n", name, MAX_NAME); abort();
  }
  
  if (strlen(suite) >= MAX_NAME) {
    printf("ERROR: Test suite '%s' too long (Maximum is %i characters)\n", suite, MAX_NAME); abort();
  }
  
  test_t test;
  test.func = func;
  pt_title_case(test.name, name);
  pt_title_case(test.suite, suite);
  
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

static clock_t start, end;
static char current_suite[MAX_NAME];

void pt_run() {
  
  printf("    \n");
  printf("    +-------------------------------------------+\n");
  printf("    | ptest          MicroTesting Magic for C   |\n");
  printf("    |                                           |\n");
  printf("    | Daniel Holden (contact@theorangeduck.com) |\n");
  printf("    +-------------------------------------------+\n");
  
  signal(SIGFPE,  ptest_signal);
  signal(SIGILL,  ptest_signal);
  signal(SIGSEGV, ptest_signal);
  
  start = clock();
  strcpy(current_suite, "");
  
  for(int i = 0; i < num_tests; i++) {
    
    test_t test = tests[i];
    
    /* Check for transition to a new suite */
    if (strcmp(test.suite, current_suite)) {
      
      /* Don't increment any counter for first entrance */
      if (strcmp(current_suite, "")) {
        if (suite_passing) {
          num_suites_passes++;
        } else {
          num_suites_fails++;
        }
      }
    
      suite_passing = true;
      strcpy(current_suite, test.suite);
      printf("\n\n  ===== %s =====\n\n", current_suite);
    }
    
    /* Run Test */
    
    test_passing = true;
    strcpy(assert_err, "");
    strcpy(assert_err_buff, "");
    assert_err_num = 0;
    printf("    | %s ... ", test.name);
    
    test.func();
    
    suite_passing = suite_passing && test_passing;
    
    if (test_passing) {
      num_tests_passes++;
      pt_color(GREEN);  printf("Passed! \n"); pt_color(WHITE);
    } else {
      num_tests_fails++;
      pt_color(RED);    printf("Failed! \n\n%s\n", assert_err); pt_color(WHITE);
    }
    
  }
  
  if (suite_passing) {
    num_suites_passes++;
  } else {
    num_suites_fails++;
  }
  
  end = clock();
  
  printf("  \n");
  printf("  +---------------------------------------------------+\n");
  printf("  |                      Summary                      |\n");
  printf("  +---------++------------+-------------+-------------+\n");
  
  printf("  | Suites  ||");
  pt_color(YELLOW); printf(" Total %4d ",  num_suites);        pt_color(WHITE); printf("|");
  pt_color(GREEN);  printf(" Passed %4d ", num_suites_passes); pt_color(WHITE); printf("|");
  pt_color(RED);    printf(" Failed %4d ", num_suites_fails);  pt_color(WHITE); printf("|\n");
  
  printf("  | Tests   ||");
  pt_color(YELLOW); printf(" Total %4d ",  num_tests);         pt_color(WHITE); printf("|");
  pt_color(GREEN);  printf(" Passed %4d ", num_tests_passes);  pt_color(WHITE); printf("|");
  pt_color(RED);    printf(" Failed %4d ", num_tests_fails);   pt_color(WHITE); printf("|\n");
  
  printf("  | Asserts ||");
  pt_color(YELLOW); printf(" Total %4d ",  num_asserts);       pt_color(WHITE); printf("|");
  pt_color(GREEN);  printf(" Passed %4d ", num_assert_passes); pt_color(WHITE); printf("|");
  pt_color(RED);    printf(" Failed %4d ", num_assert_fails);  pt_color(WHITE); printf("|\n");
  
  printf("  +---------++------------+-------------+-------------+\n");
  printf("  \n");
  
  double total = (double)(end - start) / CLOCKS_PER_SEC;
  
  printf("      Total Running Time: %0.3fs\n\n", total);
}
