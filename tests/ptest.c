#include "ptest.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

/* Globals */

enum {
  MAX_NAME = 512
};

enum {
  MAX_ERROR = 2048
};

enum {
  MAX_TESTS = 2048
};

static int test_passing = 0;
static int suite_passing = 0;

/* Colors */

enum {
  BLACK   = 0,
  BLUE    = 1,
  GREEN   = 2,
  AQUA    = 3,
  RED     = 4,
  PURPLE  = 5,
  YELLOW  = 6,
  WHITE   = 7,
  GRAY    = 8,
  
  LIGHT_BLUE   = 9,
  LIGHT_GREEN  = 10,
  LIGHT_AQUA   = 11,
  LIGHT_RED    = 12,
  LIGHT_PURPLE = 13,
  LIGHT_YELLOW = 14,
  LIGHT_WHITE  = 15,
  
  DEFAULT      = 16,
};

#ifdef _WIN32

#include <windows.h>

static WORD defaults;
static int defaults_loaded = 0;

static void pt_color(int color) {
  
  HANDLE cnsl = GetStdHandle(STD_OUTPUT_HANDLE);
  
  if (!defaults_loaded) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(cnsl, &info);
    defaults = info.wAttributes;
    defaults_loaded = 1;
  }
  
  SetConsoleTextAttribute(cnsl, color == DEFAULT ? defaults : color);
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
  "\x1B[37m",
  "\x1B[39m",
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

void pt_assert_run(
  int result, const char* expr, const char* func, const char* file, int line) {
  
  num_asserts++;
  test_passing = test_passing && result;
  
  if (result) {
    num_assert_passes++;
  } else {
    sprintf(assert_err_buff, 
      "        %i. Assert [ %s ] (%s:%i)\n", 
      assert_err_num+1, expr, file, line );
    strcat(assert_err, assert_err_buff);
    assert_err_num++;
    num_assert_fails++;
  }
  
}

static void ptest_signal(int sig) {

  test_passing = 0;
  
  switch( sig ) {
    case SIGFPE:  sprintf(assert_err_buff,
      "        %i. Division by Zero\n", assert_err_num+1);
    break;
    case SIGILL:  sprintf(assert_err_buff,
      "        %i. Illegal Instruction\n", assert_err_num+1);
    break;
    case SIGSEGV: sprintf(assert_err_buff,
      "        %i. Segmentation Fault\n", assert_err_num+1);
    break;
  }
  
  assert_err_num++;
  strcat(assert_err, assert_err_buff);
  
  pt_color(RED); 
  printf("Failed! \n\n%s\n", assert_err);
  pt_color(DEFAULT);
  
  puts("    | Stopping Execution.");
  fflush(stdout);
  exit(0);
  
}

/* Tests */

static void pt_title_case(char* output, const char* input) {

  int space = 1;
  unsigned int i;
  
  strcpy(output, input);
  
  for(i = 0; i < strlen(output); i++) {

    if (output[i] == '_' || output[i] == ' ') {
      space = 1;
      output[i] = ' ';
      continue;
    } 
    
    if (space && output[i] >= 'a' && output[i] <= 'z') {
      space = 0;
      output[i] = output[i] - 32;
      continue;
    }
    
    space = 0;
    
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

  test_t test;

  if (num_tests == MAX_TESTS) {
    printf("ERROR: Exceeded maximum test count of %i!\n", 
      MAX_TESTS); abort();
  }
  
  if (strlen(name) >= MAX_NAME) {
    printf("ERROR: Test name '%s' too long (Maximum is %i characters)\n", 
      name, MAX_NAME); abort();
  }
  
  if (strlen(suite) >= MAX_NAME) {
    printf("ERROR: Test suite '%s' too long (Maximum is %i characters)\n", 
      suite, MAX_NAME); abort();
  }
  
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

int pt_run(void) {
  
  unsigned int i;
  double total;
  test_t test;

  puts("");
  puts("    +-------------------------------------------+");
  puts("    | ptest          MicroTesting Magic for C   |");
  puts("    |                                           |");
  puts("    | http://github.com/orangeduck/ptest        |");
  puts("    |                                           |");
  puts("    | Daniel Holden (contact@theorangeduck.com) |");
  puts("    +-------------------------------------------+");
  
  signal(SIGFPE,  ptest_signal);
  signal(SIGILL,  ptest_signal);
  signal(SIGSEGV, ptest_signal);
  
  start = clock();
  strcpy(current_suite, "");
  
  for(i = 0; i < num_tests; i++) {

    test = tests[i];
    
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
    
      suite_passing = 1;
      strcpy(current_suite, test.suite);
      printf("\n\n  ===== %s =====\n\n", current_suite);
    }
    
    /* Run Test */
    
    test_passing = 1;
    strcpy(assert_err, "");
    strcpy(assert_err_buff, "");
    assert_err_num = 0;
    printf("    | %s ... ", test.name);
    fflush(stdout);
    
    test.func();
    
    suite_passing = suite_passing && test_passing;
    
    if (test_passing) {
      num_tests_passes++;
      pt_color(GREEN);
      puts("Passed!");
      pt_color(DEFAULT);
    } else {
      num_tests_fails++;
      pt_color(RED); 
      printf("Failed! \n\n%s\n", assert_err);
      pt_color(DEFAULT);
    }
    
  }
  
  if (suite_passing) {
    num_suites_passes++;
  } else {
    num_suites_fails++;
  }
  
  end = clock();
  
  puts("");
  puts("  +---------------------------------------------------+");
  puts("  |                      Summary                      |");
  puts("  +---------++------------+-------------+-------------+");
  
  printf("  | Suites  ||");
  pt_color(YELLOW);  printf(" Total %4d ",  num_suites);        
  pt_color(DEFAULT); putchar('|');
  pt_color(GREEN);   printf(" Passed %4d ", num_suites_passes); 
  pt_color(DEFAULT); putchar('|');
  pt_color(RED);     printf(" Failed %4d ", num_suites_fails);  
  pt_color(DEFAULT); puts("|");
  
  printf("  | Tests   ||");
  pt_color(YELLOW);  printf(" Total %4d ",  num_tests);         
  pt_color(DEFAULT); putchar('|');
  pt_color(GREEN);   printf(" Passed %4d ", num_tests_passes);  
  pt_color(DEFAULT); putchar('|');
  pt_color(RED);     printf(" Failed %4d ", num_tests_fails);   
  pt_color(DEFAULT); puts("|");
  
  printf("  | Asserts ||");
  pt_color(YELLOW);  printf(" Total %4d ",  num_asserts);       
  pt_color(DEFAULT); putchar('|');
  pt_color(GREEN);   printf(" Passed %4d ", num_assert_passes); 
  pt_color(DEFAULT); putchar('|');
  pt_color(RED);     printf(" Failed %4d ", num_assert_fails);  
  pt_color(DEFAULT); puts("|");
  
  puts("  +---------++------------+-------------+-------------+");
  puts("");
  
  total = (double)(end - start) / CLOCKS_PER_SEC;
  
  printf("      Total Running Time: %0.3fs\n\n", total);
  
  if (num_suites_fails > 0) { return 1; } else { return 0; }
}
