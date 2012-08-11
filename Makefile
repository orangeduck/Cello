CC = gcc
LIBS = -lm
INCS = -I ./include
C_FILES = $(wildcard src/*.c)
EXAMPLE_FILES = $(wildcard examples/*.c)
PLATFORM = $(shell uname)
NAME = C+

ifeq ($(findstring Linux,$(PLATFORM)),Linux)
	OUT=lib$(NAME).so
	CFLAGS= $(INCS) $(LIBS) -std=gnu99 -Wall -Werror -Wno-unused -g -fPIC
	LFLAGS= -shared
	OBJ_FILES= $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
endif

ifeq ($(findstring Darwin,$(PLATFORM)),Darwin)
	OUT=lib$(NAME).so
	CFLAGS= $(INCS) -std=gnu99 -Wall -Werror -Wno-unused -g -fPIC
	LFLAGS= -shared
	OBJ_FILES= $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
endif

ifeq ($(findstring MINGW,$(PLATFORM)),MINGW)
	OUT=$(NAME).dll
	CFLAGS= $(INCS) -std=gnu99 -Wall -Werror -Wno-unused -g
	LFLAGS= -g -L ./lib -shared
	OBJ_FILES= $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
endif

$(OUT): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(LFLAGS) -o $@
	
examples: $(OBJ_FILES) $(EXAMPLE_FILES)
	$(CC) examples/files.c $(CFLAGS) $(OBJ_FILES) -o examples/files $(LIBS)
	$(CC) examples/types.c $(CFLAGS) $(OBJ_FILES) -o examples/types $(LIBS)
	$(CC) examples/lists.c $(CFLAGS) $(OBJ_FILES) -o examples/lists $(LIBS)
	$(CC) examples/functions.c $(CFLAGS) $(OBJ_FILES) -o examples/functions $(LIBS)
	$(CC) examples/example.c $(CFLAGS) $(OBJ_FILES) -o examples/example $(LIBS)
  
test: $(OBJ_FILES) tests/test.c
	$(CC) tests/test.c -lcunit $(CFLAGS) $(OBJ_FILES) -o test $(LIBS)
	./test
  
obj/%.o: src/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@
	
obj:
	mkdir obj
	
clean:
	rm $(OBJ_FILES)
