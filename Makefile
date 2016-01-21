CC ?= gcc
AR ?= ar

VERSION = 2.0.3
PACKAGE = libCello-$(VERSION)

BINDIR = ${PREFIX}/bin
INCDIR = ${PREFIX}/include
LIBDIR = ${PREFIX}/lib

SRC := $(wildcard src/*.c)
OBJ := $(addprefix obj/,$(notdir $(SRC:.c=.o)))

TESTS := $(wildcard tests/*.c)
TESTS_OBJ := $(addprefix obj/,$(notdir $(TESTS:.c=.o)))

EXAMPLES := $(wildcard examples/*.c)
EXAMPLES_OBJ := $(addprefix obj/,$(notdir $(EXAMPLES:.c=.o)))
EXAMPLES_EXE := $(EXAMPLES:.c=)

CFLAGS = -I ./include -std=gnu99 -Wall -Wno-unused -g -ggdb

PLATFORM := $(shell uname)
COMPILER := $(shell $(CC) -v 2>&1 )

ifeq ($(findstring CYGWIN,$(PLATFORM)),CYGWIN)
	PREFIX ?= /usr/local
	
	DYNAMIC = libCello.so
	STATIC = libCello.a
	LIBS = -lpthread -lm
	
	ifneq (,$(wildcard ${LIBDIR}/libdbghelp.a))
		LIBS += -lDbgHelp
	else
		CFLAGS += -DCELLO_NSTRACE
	endif
  
	INSTALL_LIB = mkdir -p ${LIBDIR} && cp -f ${STATIC} ${LIBDIR}/$(STATIC)
	INSTALL_INC = mkdir -p ${INCDIR} && cp -r include/Cello.h ${INCDIR}
	UNINSTALL_LIB = rm -f ${LIBDIR}/$(STATIC)
	UNINSTALL_INC = rm -f ${INCDIR}/Cello.h
else ifeq ($(findstring MINGW,$(PLATFORM)),MINGW)
	# MSYS2
	ifeq ($(findstring MINGW32,$(MSYSTEM)),MINGW32)
		CC = i686-w64-mingw32-gcc
		PREFIX ?= /mingw32
	else ifeq ($(findstring MINGW64,$(MSYSTEM)),MINGW64)
		CC = x86_64-w64-mingw32-gcc
		PREFIX ?= /mingw64
	else ifeq ($(findstring MSYS,$(MSYSTEM)),MSYS)
		CC = gcc
		PREFIX ?= /usr
	else
		# MinGW64 mingw-builds
		prefix ?= c:/mingw64/x86_64-w64-mingw32
	endif

	DYNAMIC = libCello.dll
	STATIC = libCello.a
  
	ifneq (,$(wildcard ${LIBDIR}/libdbghelp.a))
		LIBS += -lDbgHelp
	else
		CFLAGS += -DCELLO_NSTRACE
	endif
	
	INSTALL_LIB = cp $(STATIC) $(LIBDIR)/$(STATIC); cp $(DYNAMIC) $(BINDIR)/$(DYNAMIC)
	INSTALL_INC = cp -r include/Cello.h $(INCDIR)
	UNINSTALL_LIB = rm -f ${LIBDIR}/$(STATIC); rm -f ${BINDIR}/$(DYNAMIC)
	UNINSTALL_INC = rm -f ${INCDIR}/Cello.h
else ifeq ($(findstring FreeBSD,$(PLATFORM)),FreeBSD)
	PREFIX ?= /usr/local

	DYNAMIC = libCello.so
	STATIC = libCello.a
	LIBS = -lpthread -lm

	CFLAGS += -fPIC

	ifneq (,$(wildcard ${LIBDIR}/libexecinfo.a))
		LIBS += -lexecinfo
		LFLAGS += -rdynamic
	else
		CFLAGS += -DCELLO_NSTRACE
	endif
  
	INSTALL_LIB = mkdir -p ${LIBDIR} && cp -f ${STATIC} ${LIBDIR}/$(STATIC)
	INSTALL_INC = mkdir -p ${INCDIR} && cp -r include/Cello.h ${INCDIR}
	UNINSTALL_LIB = rm -f ${LIBDIR}/$(STATIC)
	UNINSTALL_INC = rm -f ${INCDIR}/Cello.h
else
	PREFIX ?= /usr/local

	DYNAMIC = libCello.so
	STATIC = libCello.a
	LIBS = -lpthread -lm

	CFLAGS += -fPIC
  
	ifneq (,$(wildcard ${LIBDIR}/libexecinfo.a))
		LIBS += -lexecinfo
		LFLAGS += -rdynamic
	else
		CFLAGS += -DCELLO_NSTRACE
	endif
  
	INSTALL_LIB = mkdir -p ${LIBDIR} && cp -f ${STATIC} ${LIBDIR}/$(STATIC)
	INSTALL_INC = mkdir -p ${INCDIR} && cp -r include/Cello.h ${INCDIR}
	UNINSTALL_LIB = rm -f ${LIBDIR}/$(STATIC)
	UNINSTALL_INC = rm -f ${INCDIR}/Cello.h
endif

# Libraries

all: $(DYNAMIC) $(STATIC)

$(DYNAMIC): $(OBJ)
	$(CC) $(OBJ) -shared $(LFLAGS) $(LIBS) -o $@

$(STATIC): $(OBJ)
	$(AR) rcs $@ $(OBJ)

obj/%.o: src/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

obj:
	mkdir -p obj

# Tests

check: CFLAGS += -Werror -g -ggdb
check: $(TESTS_OBJ) $(STATIC)
	$(CC) $(TESTS_OBJ) $(STATIC) $(LIBS) $(LFLAGS) -o ./tests/test
	./tests/test
	rm -f ./tests/test.bin ./tests/test.txt

obj/%.o: tests/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

# Benchmarks

bench: CFLAGS += -DCELLO_NDEBUG -pg
bench: clean $(STATIC)
	cd benchmarks; ./benchmark; cd ../

# Examples

examples: $(EXAMPLES_EXE)

examples/%: CFLAGS += -Werror
examples/%: examples/%.c $(STATIC) | obj
	$(CC) $< $(STATIC) $(CFLAGS) $(LIBS) $(LFLAGS) -o $@

# Dist

dist: all | $(PACKAGE)
	cp -R examples include src tests LICENSE.md Makefile README.md $(PACKAGE)
	tar -czf $(PACKAGE).tar.gz $(PACKAGE) --exclude='*.exe' --exclude='*.pdb'

$(PACKAGE):
	mkdir -p $(PACKAGE)

# Clean

clean:
	rm -f $(OBJ) $(TESTS_OBJ) $(EXAMPLES_OBJ) $(STATIC) $(DYNAMIC) tests/test

# Install

install: all
	$(INSTALL_LIB)
	$(INSTALL_INC)
	
# Uninstall

uninstall:
	$(UNINSTALL_LIB)
	$(UNINSTALL_INC)

