
check_PROGRAMS = tests

tests_SOURCES = \
  core.c \
  data.c \
  exception.c \
  functional.c \
  memory.c \
  ptest.c \
  test.c
  
tests_CPPFLAGS = -I$(top_srcdir)/tests
tests_CFLAGS = -std=gnu99 -Wall -Werror -Wextra -Wno-unused 
tests_LDFLAGS = -lCello

TESTS = tests
