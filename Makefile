CC = gcc
INCS = -I ./include
C_FILES = $(wildcard src/*.c) $(wildcard src/*/*.c)
PLATFORM = $(shell uname)
NAME = C+

ifeq ($(findstring Linux,$(PLATFORM)),Linux)
	OUT=lib$(NAME).so
	CFLAGS= $(INCS) -std=gnu99 -Wall -Werror -Wno-unused -O3 -g -fPIC
	LFLAGS= -shared
	OBJ_FILES= $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
endif

ifeq ($(findstring Darwin,$(PLATFORM)),Darwin)
	OUT=lib$(NAME).so
	CFLAGS= $(INCS) -std=gnu99 -Wall -Werror -Wno-unused -O3 -g -fPIC
	LFLAGS= -shared
	OBJ_FILES= $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
endif

ifeq ($(findstring MINGW,$(PLATFORM)),MINGW)
	OUT=$(NAME).dll
	CFLAGS= $(INCS) -std=gnu99 -Wall -Werror -Wno-unused -O3 -g
	LFLAGS= -g -L ./lib -shared
	OBJ_FILES= $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
endif

$(OUT): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(LFLAGS) -o $@
	
test: $(OBJ_FILES) test.c
	$(CC) test.c -lcunit $(CFLAGS) $(OBJ_FILES) -o test
	./test
  
obj/%.o: src/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@
	
obj:
	mkdir obj
	
clean:
	rm $(OBJ_FILES)
