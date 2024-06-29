CC = clang
AR = ar
OUTPUT = build

FLAGS = -std=c99 -Wall -Wextra -Wpedantic -Wconversion -Wno-float-conversion -Wno-sign-conversion
EXTRA_FLAGS = 
LIBS =
INCLUDE = -I"." -I"include"

STATIC_NAME = libsili.a
DYNAMIC_NAME = libsili.so

# For testing
NAME = test
EXE = $(OUTPUT)/$(NAME)
SRC = examples/benchmarking.c

# 'make'
all: $(OUTPUT) $(EXE) run

# 'make static'
static:
	$(CC) -x c $(FLAGS) $(EXTRA_FLAGS) $(INCLUDE) $(LIBS) -D SI_IMPLEMENTATION -c sili.h -o $(OUTPUT)/sili.o
	$(AR) rcs $(OUTPUT)/$(STATIC_NAME) $(OUTPUT)/sili.o

dynamic:
	$(CC) -x c $(FLAGS) $(EXTRA_FLAGS) -fPIC $(INCLUDE) $(LIBS) -D SI_IMPLEMENTATION -c sili.h -o $(OUTPUT)/sili.o
	$(CC) $(FLAGS) $(INCLUDE) $(LIBS) -shared $(OUTPUT)/sili.o -o $(OUTPUT)/$(DYNAMIC_NAME)


# Run the exe.
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
