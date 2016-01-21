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

PCF = Cello.pc
PCFDIR = ${LIBDIR}/pkgconfig
PCFLIBS = -lCello
PCFCFLAGS = -std=gnu99

PLATFORM := $(shell uname)
COMPILER := $(shell $(CC) -v 2>&1 )

ifeq ($(findstring CYGWIN,$(PLATFORM)),CYGWIN)
	PREFIX ?= /usr/local
	
	DYNAMIC = libCello.so
	STATIC = libCello.a
	LIBS = -lpthread -lm
	
	ifneq (,$(wildcard ${LIBDIR}/libdbghelp.a))
		LIBS += -lDbgHelp
		PCFLIBS += -lDbgHelp
	else
		CFLAGS += -DCELLO_NSTRACE
		PCFCFLAGS += -DCELLO_NSTRACE
	endif
  
	INSTALL_LIB = mkdir -p ${LIBDIR} && cp -f ${STATIC} ${LIBDIR}/$(STATIC)
	INSTALL_INC = mkdir -p ${INCDIR} && cp -r include/Cello.h ${INCDIR}
	INSTALL_PCF = mkdir -p ${PCFDIR} && cp -f $(PCF) ${PCFDIR}/$(PCF)
	UNINSTALL_LIB = rm -f ${LIBDIR}/$(STATIC)
	UNINSTALL_INC = rm -f ${INCDIR}/Cello.h
	UNINSTALL_PCF = rm -f ${PCFDIR}/$(PCF)
else ifeq ($(findstring MINGW,$(PLATFORM)),MINGW)
	# MSYS2
	DBGHELPDIR = ${LIBDIR}
	ifeq ($(findstring MINGW32,$(MSYSTEM)),MINGW32)
		CC = i686-w64-mingw32-gcc
		PREFIX ?= /mingw32
		DBGHELPDIR = /mingw32/i686-w64-mingw32/lib
	else ifeq ($(findstring MINGW64,$(MSYSTEM)),MINGW64)
		CC = x86_64-w64-mingw32-gcc
		PREFIX ?= /mingw64
		DBGHELPDIR = /mingw64/x86_64-w64-mingw32/lib
	else ifeq ($(findstring MSYS,$(MSYSTEM)),MSYS)
		CC = gcc
		PREFIX ?= /usr
		DBGHELPDIR = /usr/lib/w32api
	else
		# MinGW64 mingw-builds
		prefix ?= c:/mingw64/x86_64-w64-mingw32
	endif

	DYNAMIC = libCello.dll
	STATIC = libCello.a
  
	ifneq (,$(wildcard ${DBGHELPDIR}/libdbghelp.a))
		LIBS += -lDbgHelp
		PCFLIBS += -lDbgHelp
	else
		CFLAGS += -DCELLO_NSTRACE
		PCFCFLAGS += -DCELLO_NSTRACE
	endif
	
	INSTALL_LIB = cp $(STATIC) $(LIBDIR)/$(STATIC); cp $(DYNAMIC) $(BINDIR)/$(DYNAMIC)
	INSTALL_INC = cp -r include/Cello.h $(INCDIR)
	INSTALL_PCF = mkdir -p ${PCFDIR} && cp -f $(PCF) ${PCFDIR}/$(PCF)
	UNINSTALL_LIB = rm -f ${LIBDIR}/$(STATIC); rm -f ${BINDIR}/$(DYNAMIC)
	UNINSTALL_INC = rm -f ${INCDIR}/Cello.h
	UNINSTALL_PCF = rm -f ${PCFDIR}/$(PCF)
else ifeq ($(findstring FreeBSD,$(PLATFORM)),FreeBSD)
	PREFIX ?= /usr/local

	DYNAMIC = libCello.so
	STATIC = libCello.a
	LIBS = -lpthread -lm
	PCFLIBS += -lpthreads -lm

	CFLAGS += -fPIC

	ifneq (,$(wildcard ${LIBDIR}/libexecinfo.a))
		LIBS += -lexecinfo
		LFLAGS += -rdynamic
	else
		CFLAGS += -DCELLO_NSTRACE
	endif
  
	INSTALL_LIB = mkdir -p ${LIBDIR} && cp -f ${STATIC} ${LIBDIR}/$(STATIC)
	INSTALL_INC = mkdir -p ${INCDIR} && cp -r include/Cello.h ${INCDIR}
	INSTALL_PCF = mkdir -p ${PCFDIR} && cp -f $(PCF) ${PCFDIR}/$(PCF)
	UNINSTALL_LIB = rm -f ${LIBDIR}/$(STATIC)
	UNINSTALL_INC = rm -f ${INCDIR}/Cello.h
	UNINSTALL_PCF = rm -f ${PCFDIR}/$(PCF)
else
	PREFIX ?= /usr/local

	DYNAMIC = libCello.so
	STATIC = libCello.a
	LIBS = -lpthread -lm
	PCFLIBS += -lpthreads -lm

	CFLAGS += -fPIC
  
	ifneq (,$(wildcard ${LIBDIR}/libexecinfo.a))
		LIBS += -lexecinfo
		LFLAGS += -rdynamic
	else
		CFLAGS += -DCELLO_NSTRACE
	endif
  
	INSTALL_LIB = mkdir -p ${LIBDIR} && cp -f ${STATIC} ${LIBDIR}/$(STATIC)
	INSTALL_INC = mkdir -p ${INCDIR} && cp -r include/Cello.h ${INCDIR}
	INSTALL_PCF = mkdir -p ${PCFDIR} && cp -f $(PCF) ${PCFDIR}/$(PCF)
	UNINSTALL_LIB = rm -f ${LIBDIR}/$(STATIC)
	UNINSTALL_INC = rm -f ${INCDIR}/Cello.h
	UNINSTALL_PCF = rm -f ${PCFDIR}/$(PCF)
endif

# Libraries

all: $(DYNAMIC) $(STATIC) $(PCF)

$(DYNAMIC): $(OBJ)
	$(CC) $(OBJ) -shared $(LFLAGS) $(LIBS) -o $@

$(STATIC): $(OBJ)
	$(AR) rcs $@ $(OBJ)

obj/%.o: src/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

obj:
	mkdir -p obj

$(PCF):
	echo "# GENERATED Cello pkg-config metadata file" >$@
	echo "prefix=$(PREFIX)" >>$@
	echo "exec_prefix=$(PREFIX)" >>$@
	echo "bindir=$(BINDIR)" >>$@
	echo "libdir=$(LIBDIR)" >>$@
	echo "includedir=$(INCDIR)" >>$@
	echo "" >>$@
	echo "Name: Cello" >>$@
	echo "Description: Cello is a library that brings higher level programming to C" >>$@
	echo "Version: $(VERSION)" >>$@
	echo "URL: http://libcello.org/" >>$@
	echo "Libs: -L$(LIBDIR) $(PCFLIBS)" >>$@
	echo "Cflags: -I$(INCDIR) $(PCFCFLAGS)" >>$@

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
	rm -f $(OBJ) $(TESTS_OBJ) $(EXAMPLES_OBJ) $(STATIC) $(DYNAMIC) tests/test $(PCF)

# Install

install: all
	$(INSTALL_LIB)
	$(INSTALL_INC)
	$(INSTALL_PCF)
	
# Uninstall

uninstall:
	$(UNINSTALL_LIB)
	$(UNINSTALL_INC)
	$(UNINSTALL_PCF)

