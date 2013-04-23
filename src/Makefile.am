
lib_LTLIBRARIES = libCello.la

libCello_la_SOURCES = \
  Array.c \
  Bool.c \
  Char.c \
  Dictionary.c \
  Exception.c \
  File.c \
  Format.c \
  Function.c \
  Lambda.c \
  List.c \
  Map.c \
  Number.c \
  Pool.c \
  Prelude.c \
  Reference.c \
  String.c \
  Table.c \
  Tree.c \
  Type.c
  
libCello_la_CPPFLAGS = -I$(top_srcdir)/include
libCello_la_CFLAGS = -std=gnu99 -Wall -Werror -Wextra -Wno-unused
libCello_la_LDFLAGS = -lm

include_HEADERS = $(top_srcdir)/include/Cello.h

pkgincludedir = $(includedir)/Cello
pkginclude_HEADERS = \
  $(top_srcdir)/include/Array.h \
  $(top_srcdir)/include/Bool.h \
  $(top_srcdir)/include/Char.h \
  $(top_srcdir)/include/Dictionary.h \
  $(top_srcdir)/include/Exception.h \
  $(top_srcdir)/include/File.h \
  $(top_srcdir)/include/Format.h \
  $(top_srcdir)/include/Function.h \
  $(top_srcdir)/include/FunctionAuto.h \
  $(top_srcdir)/include/Lambda.h \
  $(top_srcdir)/include/LambdaCurry.h \
  $(top_srcdir)/include/List.h \
  $(top_srcdir)/include/Map.h \
  $(top_srcdir)/include/None.h \
  $(top_srcdir)/include/Number.h \
  $(top_srcdir)/include/Pool.h \
  $(top_srcdir)/include/Prelude.h \
  $(top_srcdir)/include/Reference.h \
  $(top_srcdir)/include/String.h \
  $(top_srcdir)/include/Table.h \
  $(top_srcdir)/include/Tree.h \
  $(top_srcdir)/include/Type.h \
  $(top_srcdir)/include/Value.h
  
