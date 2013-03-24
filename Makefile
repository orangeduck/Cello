-include config.mk

PLATFORM?= $(shell uname)

CC=gcc
AR?=ar

LAC_CPPFLAGS= -I./include
LAC_CFLAGS= -std=gnu99 -Wall -Werror -Wextra -Wno-unused -g 
LAC_LDFLAGS= -g -rdynamic
LAC_LIBS= -lm 

LIB_C_FILES= $(wildcard src/*.c)
LIB_OBJ_FILES= $(addprefix obj/,$(notdir $(LIB_C_FILES:%.c=%.o)))

DEMO_C_FILES= $(wildcard demos/*.c)
DEMO_OBJ_FILES= $(addprefix obj/,$(notdir $(DEMO_C_FILES:%.c=%.o)))
DEMO_TARGETS= $(DEMO_C_FILES:%.c=%$(EXE_SUFFIX))

TEST_C_FILES= $(wildcard tests/*.c)
TEST_OBJ_FILES= $(addprefix obj/,$(notdir $(TEST_C_FILES:%.c=%.o)))
TEST_TARGET= tests/test$(EXE_SUFFIX)

SHARED_LIB= $(SHARED_LIB_PREFIX)C+$(SHARED_LIB_SUFFIX)
STATIC_LIB= $(STATIC_LIB_PREFIX)C+$(STATIC_LIB_SUFFIX)

ifeq ($(findstring Linux,$(PLATFORM)),Linux)
	LAC_CFLAGS+= -fPIC
	LAC_LDFLAGS+= -fPIC
	SHARED_LIB_PREFIX:=lib
	SHARED_LIB_SUFFIX:=.so
	STATIC_LIB_PREFIX:=lib
	STATIC_LIB_SUFFIX:=.a
	EXE_SUFFIX:=
else ifeq ($(findstring Darwin,$(PLATFORM)),Darwin)
	LAC_CFLAGS+= -fPIC
	LAC_LDFLAGS+= -fPIC
	SHARED_LIB_PREFIX:=lib
	SHARED_LIB_SUFFIX:=.so
	STATIC_LIB_PREFIX:=lib
	STATIC_LIB_SUFFIX:=.a
else ifeq ($(findstring MINGW,$(PLATFORM)),MINGW)
	LAC_LDFLAGS+= 
	LAC_LIBS+= -lmingw32
	SHARED_LIB_PREFIX:=
	SHARED_LIB_SUFFIX:=.dll
	STATIC_LIB_PREFIX:=
	STATIC_LIB_SUFFIX:=.lib
	EXE_SUFFIX:=.exe
endif

all: $(SHARED_LIB) $(STATIC_LIB)

$(SHARED_LIB): $(LIB_OBJ_FILES)
	$(CC) $(LAC_LDFLAGS) $(LDFLAGS) -shared -o $@ $^ $(LAC_LIBS)

	
$(STATIC_LIB): $(LIB_OBJ_FILES)
	$(AR) rcs $@ $^
  
# Demos

demos: $(DEMO_TARGETS)

$(DEMO_TARGETS): demos/%$(EXE_SUFFIX): obj/%.o $(STATIC_LIB)
	$(CC) $(LAC_LDFLAGS) $(LDFLAGS) -o $@ $^ $(LAC_LIBS)
 
tests: $(TEST_TARGET)
	
$(TEST_TARGET): $(LIB_OBJ_FILES) $(TEST_OBJ_FILES)
	$(CC) $(LAC_LDFLAGS) $(LDFLAGS) -o $@ $^ $(LAC_LIBS)
	./$@
 
# Clean

clean:
	$(RM) $(LIB_OBJ_FILES) $(DEMO_OBJ_FILES) $(TEST_OBJ_FILES) $(TEST_TARGETS) $(DEMO_TARGETS) $(SHARED_LIB) $(STATIC_LIB)

obj:
	mkdir obj

obj/%.o: src/%.c | obj
	$(CC) $(LAC_CPPFLAGS) $(LAC_CFLAGS) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

obj/%.o: demos/%.c | obj
	$(CC) $(LAC_CPPFLAGS) $(LAC_CFLAGS) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
  
obj/%.o: tests/%.c | obj
	$(CC) $(LAC_CPPFLAGS) $(LAC_CFLAGS) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
