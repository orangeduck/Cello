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
libCello_la_CFLAGS = -std=gnu99 -Wall -Wextra -Wno-unused
libCello_la_LDFLAGS = -lm

include_HEADERS = $(top_srcdir)/include/Cello.h

pkgincludedir = $(includedir)/Cello
pkginclude_HEADERS = \
  $(top_srcdir)/include/Cello/Array.h \
  $(top_srcdir)/include/Cello/Bool.h \
  $(top_srcdir)/include/Cello/Char.h \
  $(top_srcdir)/include/Cello/Dictionary.h \
  $(top_srcdir)/include/Cello/Exception.h \
  $(top_srcdir)/include/Cello/File.h \
  $(top_srcdir)/include/Cello/Format.h \
  $(top_srcdir)/include/Cello/Function.h \
  $(top_srcdir)/include/Cello/FunctionAuto.h \
  $(top_srcdir)/include/Cello/Lambda.h \
  $(top_srcdir)/include/Cello/LambdaCurry.h \
  $(top_srcdir)/include/Cello/List.h \
  $(top_srcdir)/include/Cello/Map.h \
  $(top_srcdir)/include/Cello/None.h \
  $(top_srcdir)/include/Cello/Number.h \
  $(top_srcdir)/include/Cello/Pool.h \
  $(top_srcdir)/include/Cello/Prelude.h \
  $(top_srcdir)/include/Cello/Reference.h \
  $(top_srcdir)/include/Cello/String.h \
  $(top_srcdir)/include/Cello/Table.h \
  $(top_srcdir)/include/Cello/Tree.h \
  $(top_srcdir)/include/Cello/Type.h \
  $(top_srcdir)/include/Cello/Value.h
  
