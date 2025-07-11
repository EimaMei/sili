# Parameter explanations:
#   CC - compiler used by the Makefile. Can be whatever you desire.
#
#   PLATFORM - sets the platform target to compile for. Current values: WIN32_GNU,
# WIN32_MSVC, OS_X, LINUX, WASM_WASI, WASM_EMCC, DEFAULT. Selecting 'DEFAULT'
# makes the Makefile automatically guess which platform to target.
#
# 	MODE - sets the release mode to compile for. Current values: FAST, DEBUG,
# RELEASE. 'FAST' disables all flags and enables ones requires for fast compilation.
# 'DEBUG' turns on all warnings as well as flags to help with debugging/finding
# problematic code. 'RELEASE' turns on all optimisations as well as warnings.
#
# 	LANGUAGE - selects which programming language (and standard) to use. Available
# values: c99, c11, c17, c2x/c23, c++11, c++14, c++17, c++20, etc.

CC        = clang
PLATFORM  = DEFAULT
MODE      = FAST
LANGUAGE  = c23


# General building options:
#	EXTRA_FLAGS - any additional compiler flags to use.
#
#	OUTPUT      - the directory where all of the output goes to.

EXTRA_FLAGS =
OUTPUT      = build


# Building executable options:
#	NAME - the executable name.
#
#	SRC  - source file to target.

NAME   = basic
SRC    = examples/sili/array.c


# Building library options:
#	LIB_NAME   - the library name.
#
#   LIB_LINKER - exexcutable used for creating a static library. If this
# argument is set to 'DEFAULT', a program will be automatically picked out.

LIB_NAME   = sili
LIB_LINKER = DEFAULT



ifeq ($(PLATFORM),DEFAULT)
	ifneq (,$(filter %-mingw32-gcc %-mingw32-g++ w64gcc w32gcc,$(CC)))
		PLATFORM = WIN32_GNU
	else ifneq (,$(filter $(CC),cl cl.exe))
		PLATFORM = WIN32_MSVC
	else ifneq (,$(filter $(CC), wasi))
		PLATFORM = WASM_WASI
	else ifneq (,$(filter $(CC), emcc))
		PLATFORM = WASM_EMCC
	else
		DETECTED_OS := $(shell uname 2>/dev/null || echo Unknown)

		ifeq ($(DETECTED_OS),Darwin)
			PLATFORM = OS_X
		else ifeq ($(DETECTED_OS),Linux)
			PLATFORM = LINUX
		else
			$(error Unsupported platform. Please refer to the Makefile for supported platofmrs.)
		endif
	endif
endif


ifneq (,$(filter cpp%,$(LANGUAGE)))
	GNU_FLAGS = -std=$(LANGUAGE) -x c++ -fno-exceptions
else
	GNU_FLAGS = -std=$(LANGUAGE) -x c -Wvla
endif

ifeq ($(MODE),FAST)
	GNU_FLAGS += -O0

	ifneq ($(MAKECMDGOALS),dynamic)
	GNU_FLAGS += -flto
	endif

else
	GNU_FLAGS += \
		-Wall -Wextra -Wpedantic \
		-Wconversion -Wsign-conversion -Wdouble-promotion \
		\
		-Wpointer-arith \
		\
		-Wcast-align \
		\
		\
		-Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition \
		\
		-Wswitch-enum -Wnested-externs \
		\
		-Werror=format-security -Wformat=2 -Wformat-signedness \
		\
		-Winit-self -Wshadow -Wredundant-decls \
		\
		-Wmissing-noreturn \
		\
		-fwrapv -fstrict-aliasing \
		-fno-omit-frame-pointer

	ifneq (,$(filter $(PLATORM),OS_X WIN32_GNU))
		GNU_FLAGS += -fstrict-flex-arrays
	endif

	ifneq (,$(filter $(CC),gcc g++))
		GNU_FLAGS += -Wcast-align=strict -Wlogical-op
	endif

	ifeq ($(MODE),DEBUG)
		GNU_FLAGS += \
			-fstack-clash-protection \
			-ftrivial-auto-var-init=pattern

		ifneq (,$(filter $(PLATFORM), OS_X LINUX WASM_WASI WASM_EMCC))
		GNU_FLAGS += \
			-fstack-protector-strong \
			-fsanitize=undefined -fsanitize=address
		endif
	else ifeq ($(MODE),RELEASE)
		GNU_FLAGS += \
			-O3 \
			-D SI_RELEASE_MODE \
			-fno-delete-null-pointer-checks \
			-fno-strict-aliasing \
			-ftrivial-auto-var-init=zero

		ifneq ($(MAKECMDGOALS),dynamic)
		GNU_FLAGS += -flto
		endif
	endif
endif

GNU_INCLUDES = -I"." -I"include"


ifeq ($(PLATFORM),WIN32_GNU)
	FLAGS = $(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS = -lkernel32 -lole32 -lopengl32
	EXE_OUT = .exe

	ifeq ($(LIB_LINKER),DEFAULT)
	LIB_LINKER = ar
	endif
	DLL_OUT = .dll

else ifeq ($(PLATFORM),OS_X)
	FLAGS = $(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS    = -lpthread -ldl
	EXE_OUT =

	ifeq ($(LIB_LINKER),DEFAULT)
	LIB_LINKER = ar
	endif
	DLL_FLAGS = -fPIC
	DLL_OUT   = .dylib

else ifeq ($(PLATFORM),LINUX)
	FLAGS = $(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS    = -lpthread -ldl -lX11 -lXrandr -lGL
	EXE_OUT =

	ifeq ($(LIB_LINKER),DEFAULT)
	LIB_LINKER = ar
	endif
	DLL_FLAGS = -fPIC
	DLL_OUT   = .so

else ifeq ($(PLATFORM),WASM_WASI)
	FLAGS = --target=wasm32-wasi $(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS =
	EXE_OUT = .wasm

	ifeq ($(LIB_LINKER),DEFAULT)
	LIB_LINKER = ar
	endif
	DLL_FLAGS = -fPIC

else ifeq ($(PLATFORM),WASM_EMCC)
	FLAGS = --target=wasm32-unknown-emscripten -s WASM=1 -s ASYNCIFY -s PTHREAD_POOL_SIZE=4 \
		$(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS    = -pthread
	EXE_OUT = .html

	ifeq ($(LIB_LINKER),DEFAULT)
	LIB_LINKER = ar

	endif
	DLL_FLAGS = -fPIC
	DLL_OUT   = .so

else ifeq ($(PLATFORM),WIN32_MSVC)
    $(error To compile for MSVC, please call 'nmake /nologo /f Makefile.win $(MAKECMDGOALS)'.)

else
    $(error Unsupported platform. Please refer to the Makefile for supported platforms.)

endif

EXE = $(OUTPUT)/$(NAME)$(EXE_OUT)


# 'make'
all: $(OUTPUT) $(EXE) run

# Run the executable.
run: $(EXE)
	./$(EXE)

# Clean the 'build' folder.
clean:
	rm -rf $(OUTPUT)/*


$(EXE): $(SRC) sili.h Makefile examples/*
	$(CC) $(FLAGS) $(EXTRA_FLAGS) $(SRC) $(INCLUDES) $(LIBS) -o "$@"

static:
	$(CC) $(INCLUDES) $(FLAGS) $(EXTRA_FLAGS) -x c -D SI_IMPLEMENTATION -c sili.h -o "$(OUTPUT)/$(LIB_NAME).o"
	$(LIB_LINKER) rcs "$(OUTPUT)/lib$(LIB_NAME).a" "$(OUTPUT)/$(LIB_NAME).o"

dynamic:
	$(CC) $(INCLUDES) $(FLAGS) $(EXTRA_FLAGS) $(DLL_FLAGS) -x c -D SI_IMPLEMENTATION -c sili.h -o "$(OUTPUT)/$(LIB_NAME).o"
	$(CC) $(LIBS) $(EXTRA_FLAGS) $(DLL_FLAGS) -shared -o "$(OUTPUT)/lib$(LIB_NAME)$(DLL_OUT)" "$(OUTPUT)/$(LIB_NAME).o"


# Compile and run every example.
compile_examples:
	@for f in $(shell ls examples/*/*.c); do make SRC=$${f}; rm -rf $(EXE); done

# Compile and run every test.
compile_tests:
	@for f in $(shell ls tests/*.c); do make SRC=$${f}; rm -rf $(EXE); done


# If 'build' doesn't exist, create it
$(OUTPUT):
	mkdir $(OUTPUT)
