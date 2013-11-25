CC = gcc
AR = ar

VERSION = 1.1.6
PACKAGE = libCello-$(VERSION)

PREFIX=/usr/local
DESTDIR=
INCDIR=${PREFIX}/include
LIBDIR=${PREFIX}/lib
I=${DESTDIR}/${INCDIR}
L=${DESTDIR}/${LIBDIR}

SRC = $(wildcard src/*.c)
OBJ = $(addprefix obj/,$(notdir $(SRC:.c=.o)))

TESTS = $(wildcard tests/*.c)
TESTS_OBJ = $(addprefix obj/,$(notdir $(TESTS:.c=.o)))

DEMOS = $(wildcard demos/*.c)
DEMOS_OBJ = $(addprefix obj/,$(notdir $(DEMOS:.c=.o)))
DEMOS_EXE = $(DEMOS:.c=)

CFLAGS = -I ./include -std=gnu99 -Wall -Werror -Wno-unused -O3 -g
LFLAGS = -shared -g -ggdb

PLATFORM = $(shell uname)

ifeq ($(findstring Linux,$(PLATFORM)),Linux)
	DYNAMIC = libCello.so
	STATIC = libCello.a
	CFLAGS += -fPIC
	LIBS = -lpthread -ldl -lm
	INSTALL_LIB = mkdir -p ${L} && cp -f ${STATIC} ${L}/$(STATIC)
	INSTALL_INC = mkdir -p ${I} && cp -r include/* ${I}
endif

ifeq ($(findstring Darwin,$(PLATFORM)),Darwin)
	DYNAMIC = libCello.so
	STATIC = libCello.a
	CFLAGS += -fPIC -fblocks
	LIBS = -lpthread -ldl -lm -lBlocksRuntime
	INSTALL_LIB = mkdir -p ${L} && cp -f $(STATIC) ${L}/$(STATIC)
	INSTALL_INC = mkdir -p ${I} && cp -r include/* ${I}
endif

ifeq ($(findstring MINGW,$(PLATFORM)),MINGW)
	DYNAMIC = libCello.dll
	STATIC = libCello.a
	LIBS = 
	INSTALL_LIB = cp $(STATIC) C:/MinGW64/x86_64-w64-mingw32/lib/$(STATIC); cp $(DYNAMIC) C:/MinGW64/x86_64-w64-mingw32/bin/$(DYNAMIC)
	INSTALL_INC = cp -r include/* C:/MinGW64/x86_64-w64-mingw32/include/
endif

# Libraries

all: $(DYNAMIC) $(STATIC)

$(DYNAMIC): $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $@

$(STATIC): $(OBJ)
	$(AR) rcs $@ $(OBJ)

obj/%.o: src/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@
  
obj:
	mkdir -p obj

# Tests

check: $(TESTS_OBJ) $(STATIC)
	$(CC) $(TESTS_OBJ) $(STATIC) $(LIBS) -o test
	./test
  
obj/%.o: tests/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

# Demos

demos: $(DEMOS_EXE)

demos/%: demos/%.c $(STATIC) | obj
	$(CC) $< $(STATIC) $(CFLAGS) $(LIBS) -o $@

# Dist

dist: all | $(PACKAGE)
	cp -R demos include src tests INSTALL.md LICENSE.md Makefile README.md $(PACKAGE)
	tar -czf $(PACKAGE).tar.gz $(PACKAGE)
 
$(PACKAGE):
	mkdir -p $(PACKAGE)
 
# Clean
  
clean:
	rm -f $(OBJ) $(TESTS_OBJ) $(DEMOS_OBJ) $(STATIC) $(DYNAMIC)
	rm -f test
  
# Install
  
install: all
	$(INSTALL_LIB)
	$(INSTALL_INC)
