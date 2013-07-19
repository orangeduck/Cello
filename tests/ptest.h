#ifndef ptest_h
#define ptest_h

#include <stdbool.h>
#include <string.h>

#define PT_SUITE(name) void name()
#define PT_TEST(name) auto void name(void); pt_add_test(name, #name, __func__); void name(void) 

#define PT_ASSERT(expr) pt_assert_run((bool)(expr), #expr, __func__, __FILE__, __LINE__)
#define PT_ASSERT_STR_EQ(fst, snd) pt_assert_run(strcmp(fst, snd) == 0, "strcmp( " #fst ", " #snd " ) == 0", __func__, __FILE__, __LINE__)

void pt_assert_run(bool result, const char* expr, const char* func, const char* file, int line);

void pt_add_test(void (*func)(void), const char* name, const char* suite);
void pt_add_suite(void (*func)(void));
void pt_run();

#endif