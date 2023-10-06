CC = clang
OUTPUT = build
NAME = test
EXE = $(OUTPUT)/$(NAME)

SRC = examples/benchmarking.c
FLAGS = -g -std=c99 -Wall -Wextra -Wpedantic
LIBS = -L"lib"
INCLUDE = -I"." -I"include"

# 'make'
all: $(OUTPUT) $(EXE) run

# Run the exe.
run: $(EXE)
	./$(EXE)

# Clean the 'build' folder.
clean:
	rm $(OUTPUT)/**


# Compile each time the main file or `sili.h` is changed.
$(EXE): $(SRC) sili.h
	$(CC) $(FLAGS) $(SRC) $(INCLUDE) $(LIBS) -o $@

# Sometimes need to check the assembly output.
dump_asm:
	$(CC) -g $(FLAGS) $(INCLUDE) $(LIBS) -S $(SRC) -o sili.S


# Runs and compiles every example.
run_examples:
	@for f in $(shell ls examples/*.c); do make SRC=$${f}; rm -rf $(EXE); done


# If 'build' doesn't exist, create it
$(OUTPUT):
	mkdir $(OUTPUT)
