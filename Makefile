CC = clang
AR = ar

OUTPUT = build
NAME = sili

GNU_FLAGS = -O3 -std=c11 -Wall -Wextra -Wpedantic \
	-Wconversion -Wsign-conversion \
	-Wshadow -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes \
	-Wvla -Wcast-align -Wcast-align=strict \
	\
	-Wswitch-enum -Wcast-align -Wstrict-overflow \
	-Wstrict-prototypes -Wnested-externs -Wlogical-op -Wstrict-aliasing  -Wredundant-decls \
	-Wold-style-definition \
	\
	-fno-omit-frame-pointer -ffloat-store -fstrict-aliasing \
	\
	-Wformat=2 -Wformat-signedness -Wuninitialized -Winit-self -Wunsafe-loop-optimizations -Wmissing-noreturn
GNU_INCLUDES = -I"." -I"include"

GNU_STATIC_FLAGS = -x c -D SI_IMPLEMENTATION -c sili.h -o "$(OUTPUT)/$(NAME).o"
GNU_AR_FLAGS = rcs $(OUTPUT)/lib$(NAME).a "$(OUTPUT)/$(NAME).o"
GNU_DLL_FLAGS = -shared "$(OUTPUT)/$(NAME).o" -o "$(OUTPUT)/lib$(NAME)$(DLL_EXT)"

DETECTED_OS := $(shell uname 2>/dev/null || echo Unknown)
ifneq (,$(filter $(CC),winegcc x86_64-w64-mingw32-gcc w64gcc w32gcc i686-w64-mingw32-gcc x86_64-w64-mingw32-g++))
	FLAGS = $(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS = -lkernel32 -lole32 -lopengl32
	LINKER = $(CC)

	EXE_OUT = .exe
	STATIC_FLAGS = $(GNU_STATIC_FLAGS)
	DLL_FLAGS = $(GNU_DLL_FLAGS)
	AR_FLAGS = $(GNU_AR_FLAGS)
	DLL_EXT = .dll
	CC_OUT = -o

else ifneq (,$(filter $(CC),cl /opt/msvc/bin/x64/cl.exe /opt/msvc/bin/x86/cl.exe, cl.exe))
	FLAGS = -nologo -std:c11 -Wall -wd4668 -wd4820 -wd5045
	INCLUDES = -I"." -I"include"

	LIBS =
	LINKER = $(CC)

	EXE_OUT = .exe
	STATIC_FLAGS = -c -D SI_IMPLEMENTATION -Tc sili.h -Fo"$(OUTPUT)\$(NAME).obj"
	DLL_FLAGS = -LD -nologo -Fe"$(OUTPUT)\lib$(NAME)$(DLL_EXT)" "$(OUTPUT)\$(NAME).obj"
	AR_FLAGS = -nologo -out:"$(OUTPUT)\lib$(NAME).lib" "$(OUTPUT)/$(NAME).obj"
	DLL_EXT = .dll
	CC_OUT = -Fe

else ifneq (,$(filter $(CC), wasm32-wasi-clang))
	FLAGS = --target=wasm32-wasi $(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS =
	LINKER =

	EXE_OUT = .wasm
	CC_OUT = -o

else ifneq (,$(filter $(CC), emcc))
	FLAGS = --target=wasm32-unknown-emscripten $(GNU_FLAGS) -s WASM=1 -s ASYNCIFY \
		-s PTHREAD_POOL_SIZE=4 -pthread
	INCLUDES = $(GNU_INCLUDES)

	LIBS =
	LINKER =

	EXE_OUT = .html
	CC_OUT = -o

else ifeq ($(DETECTED_OS),Darwin)
	FLAGS = $(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS = -lpthread -ldl
	LINKER = $(CC)

	EXE_OUT =
	STATIC_FLAGS = $(GNU_STATIC_FLAGS)
	DLL_FLAGS = $(GNU_DLL_FLAGS)
	AR_FLAGS = $(GNU_AR_FLAGS)
	DLL_EXT = .so
	CC_OUT = -o

else ifeq ($(DETECTED_OS),Linux)
	FLAGS = $(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS = -lpthread -ldl
	LINKER = $(CC)

	EXE_OUT =
	STATIC_FLAGS = $(GNU_STATIC_FLAGS)
	DLL_FLAGS = $(GNU_DLL_FLAGS)
	AR_FLAGS = $(GNU_AR_FLAGS)
	DLL_EXT = .so
	CC_OUT = -o

endif

EXE = $(OUTPUT)/$(NAME)$(EXE_OUT)

# For testing
SRC = examples/sili/array.c

# 'make'
all: $(OUTPUT) $(EXE) run

# 'make static'
static:
	$(CC) $(FLAGS) $(INCLUDES) $(EXTRA_FLAGS) $(STATIC_FLAGS)
	$(AR) $(AR_FLAGS)

dynamic:
	$(CC) $(FLAGS) $(INCLUDES) $(EXTRA_FLAGS) $(STATIC_FLAGS)
	$(LINKER) $(LIBS) $(EXTRA_LIBS) $(EXTRA_FLAGS) $(DLL_FLAGS)


# Run the executable.
run: $(EXE)
	./$(EXE)

# Clean the 'build' folder.
clean:
	rm $(OUTPUT)/**


# Compile each time the main file or `sili.h` is changed.
$(EXE): $(SRC) sili.h
	$(CC) $(FLAGS) $(SRC) $(INCLUDES) $(LIBS) $(CC_OUT)"$@"

# Compiles and runs every example.
compile_examples:
	@for f in $(shell ls examples/*/*.c); do make SRC=$${f}; rm -rf $(EXE); done

# Compiles and runs every test.
compile_tests:
	@for f in $(shell ls tests/*.c); do make SRC=$${f}; rm -rf $(EXE); done


# If 'build' doesn't exist, create it
$(OUTPUT):
	mkdir $(OUTPUT)
