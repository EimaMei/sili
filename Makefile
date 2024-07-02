CC = clang
AR = ar
OUTPUT = build

NAME = sili
FLAGS = -std=c99 -Wall -Wextra -Wpedantic -Wconversion -Wno-float-conversion -Wno-sign-conversion
EXTRA_FLAGS =
EXTRA_LIBS =
INCLUDE = -I"." -I"include"

# NOTE(EimaMei): Original source is from 'https://github.com/ColleagueRiley/RGFW'
ifneq (,$(filter $(CC),winegcc x86_64-w64-mingw32-gcc))
	DETECTED_OS := Windows
else
	ifeq '$(findstring ;,$(PATH))' ';'
		DETECTED_OS := Windows
	else
		DETECTED_OS := $(shell uname 2>/dev/null || echo Unknown)
		DETECTED_OS := $(patsubst CYGWIN%,Cygwin,$(DETECTED_OS))
		DETECTED_OS := $(patsubst MSYS%,MSYS,$(DETECTED_OS))
		DETECTED_OS := $(patsubst MINGW%,MSYS,$(DETECTED_OS))
	endif
endif

ifeq ($(DETECTED_OS),Windows)
	EXE = $(OUTPUT)/test.exe
	DLL_EXT = .dll
endif
ifeq ($(DETECTED_OS),Darwin)
	LIBS = -lpthread
	EXE = $(OUTPUT)/test
	DLL_EXT = .so
endif
ifeq ($(DETECTED_OS),Linux)
	LIBS = -lpthread
	EXE = $(OUTPUT)/test
	DLL_EXT = .so
endif

# For testing
SRC = examples/array.c

# 'make'
all: $(OUTPUT) $(EXE) run

# 'make static'
static:
	$(CC) -x c $(FLAGS) $(EXTRA_FLAGS) $(INCLUDE) -D SI_IMPLEMENTATION -D SI_STATIC -c sili.h -o $(OUTPUT)/$(NAME).o
	$(AR) rcs $(OUTPUT)/lib$(NAME).a $(OUTPUT)/$(NAME).o

dynamic:
	$(CC) -x c $(FLAGS) $(EXTRA_FLAGS) -fPIC $(INCLUDE) -D SI_IMPLEMENTATION -c sili.h -o $(OUTPUT)/$(NAME).o
	$(CC) $(FLAGS) $(EXTRA_FLAGS) $(INCLUDE) $(LIBS) $(EXTRA_LIBS) -shared $(OUTPUT)/$(NAME).o -o $(OUTPUT)/lib$(NAME)$(DLL_EXT)


# Run the executable.
run: $(EXE)
	./$(EXE)

# Clean the 'build' folder.
clean:
	rm $(OUTPUT)/**


# Compile each time the main file or `sili.h` is changed.
$(EXE): $(SRC) sili.h
	$(CC) $(FLAGS) $(SRC) $(INCLUDE) $(LIBS) -o $@

# Check the assembly output.
asm:
	$(CC) -g $(FLAGS) $(INCLUDE) $(LIBS) -masm=intel -S $(SRC) -o sili.S


# Compiles and runs every example.
compile_examples:
	@for f in $(shell ls examples/*.c); do make SRC=$${f}; rm -rf $(EXE); done

# Compiles and runs every test.
compile_tests:
	@for f in $(shell ls tests/*.c); do make SRC=$${f}; rm -rf $(EXE); done


# If 'build' doesn't exist, create it
$(OUTPUT):
	mkdir $(OUTPUT)
