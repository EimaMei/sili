CC = gcc
#x86_64-w64-mingw32-gcc
OUTPUT = build
NAME = test
EXE = $(OUTPUT)/$(NAME)

SRC = examples/array.c
FLAGS = -g -std=c99 -Wall -Wextra -Wpedantic
LIBS = -L"lib"
INCLUDE = -I"." -I"include"

# 'make'
all: $(OUTPUT) $(EXE) run

# Run the exe.
run: $(EXE)
	./$(EXE)
#.exe

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


# If 'build' doesn't exist, create it
$(OUTPUT):
	mkdir $(OUTPUT)
