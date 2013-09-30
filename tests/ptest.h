#ifndef ptest_h
#define ptest_h

#include <stdbool.h>
#include <string.h>


#define PT_SUITE(name) void name(void)

#define PT_DECL(name) auto void name(void);
#define PT_FUNC(name) static void name(void)
#define PT_REG(name) pt_add_test(name, #name, __func__)
#define PT_TEST(name) PT_DECL(name); PT_REG(name); void name(void)

#define PT_ASSERT(expr) pt_assert_run((bool)(expr), #expr, __func__, __FILE__, __LINE__)
#define PT_ASSERT_STR_EQ(fst, snd) pt_assert_run(strcmp(fst, snd) == 0, "strcmp( " #fst ", " #snd " ) == 0", __func__, __FILE__, __LINE__)

void pt_assert_run(bool result, const char* expr, const char* func, const char* file, int line);

void pt_add_test(void (*func)(void), const char* name, const char* suite);
void pt_add_suite(void (*func)(void));
void pt_run();

#endif