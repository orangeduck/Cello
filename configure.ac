#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([libCello], [0.9], [contact@theorangeduck.com])
AM_INIT_AUTOMAKE
LT_INIT

AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDC
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor memmove memset pow sqrt strchr strstr])

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile demos/Makefile])
AC_OUTPUT
