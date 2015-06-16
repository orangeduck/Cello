#ifndef ptest_h
#define ptest_h

#include <string.h>

#define PT_SUITE(name) void name(void)

#define PT_FUNC(name) static void name(void)
#define PT_REG(name) pt_add_test(name, #name, __func__)
#define PT_TEST(name) auto void name(void); PT_REG(name); void name(void)

#define PT_ASSERT(expr) pt_assert_run((int)((expr) != 0), #expr, __func__, __FILE__, __LINE__)
#define PT_ASSERT_STR_EQ(fst, snd) pt_assert_run(strcmp(fst, snd) == 0, "strcmp( " #fst ", " #snd " ) == 0", __func__, __FILE__, __LINE__)

void pt_assert_run(int result, const char* expr, const char* func, const char* file, int line);

void pt_add_test(void (*func)(void), const char* name, const char* suite);
void pt_add_suite(void (*func)(void));
int pt_run(void);

#endif