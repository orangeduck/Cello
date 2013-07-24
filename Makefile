CC = gcc
AR = ar

VERSION = 0.9.7
PACKAGE = libCello-$(VERSION)

SRC = $(wildcard src/*.c)
OBJ = $(addprefix obj/,$(notdir $(SRC:.c=.o)))

TESTS = $(wildcard tests/*.c)
TESTS_OBJ = $(addprefix obj/,$(notdir $(TESTS:.c=.o)))

DEMOS = $(wildcard demos/*.c)
DEMOS_OBJ = $(addprefix obj/,$(notdir $(DEMOS:.c=.o)))
DEMOS_EXE = $(DEMOS:.c=)

CFLAGS = -I ./include -std=gnu99 -Wall -Werror -Wno-unused -O3 -g
LFLAGS = -shared -g

PLATFORM = $(shell uname)

ifeq ($(findstring Linux,$(PLATFORM)),Linux)
	DYNAMIC = libCello.so
	STATIC = libCello.a
	CFLAGS += -fPIC
	LIBS = -lpthread -lm
	INSTALL_LIB = cp $(STATIC) /usr/local/lib/$(STATIC)
	INSTALL_INC = cp -r include/* /usr/local/include/
endif

ifeq ($(findstring Darwin,$(PLATFORM)),Darwin)
	DYNAMIC = libCello.so
	STATIC = libCello.a
	CFLAGS += -fPIC -fnested-functions
	LIBS = -lpthread -lm
	INSTALL_LIB = cp $(STATIC) /usr/local/lib/$(STATIC)
	INSTALL_INC = cp -r include/* /usr/local/include/
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
	mkdir obj

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
	mkdir $(PACKAGE)
 
# Clean
  
clean:
	rm $(OBJ) $(TESTS_OBJ) $(DEMOS_OBJ) $(STATIC) $(DYNAMIC)
  
# Install
  
install: all
	$(INSTALL_LIB)
	$(INSTALL_INC)
