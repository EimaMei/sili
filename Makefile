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
# 	LANGUAGE - selects which programming language to target. Currently C and C++ 
# are the only valid options.

CC        = clang
PLATFORM  = DEFAULT
MODE      = FAST
LANGUAGE  = C


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
#	LIB_AR     - the executable used for creating a static library. If this 
# argument is set to 'DEFAULT', a program will be automatically picked out.
#
#   LIB_LINKER - exexcutable used for creating a dynamic library. If this 
# argument is set to 'DEFAULT', a program will be automatically picked out.

LIB_NAME   = sili
LIB_AR     = DEFAULT
LIB_LINKER = DEFAULT



ifeq ($(PLATFORM),DEFAULT)
	ifneq (,$(filter $(CC),mingw32-gcc x86_64-w64-mingw32-g++ w64gcc w32gcc))
		PLATFORM = WIN32_GNU
	else ifneq (,$(filter $(CC),cl))
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


ifeq ($(LANGUAGE),C)
	ifeq ($(or $(filter OS_X,$(PLATFORM)),$(USE_C2X_FLAG)),)
		GNU_FLAGS = -std=c23 -x c -Wvla
	else 
		GNU_FLAGS = -std=c2x -x c -Wvla
	endif
else ifeq ($(LANGUAGE),CPP)
	GNU_FLAGS = -std=c++11 -x c++ -fno-exceptions
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

	ifneq ($(PLATFORM),OS_X) 
		GNU_FLAGS += -fstrict-flex-arrays=3
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


ifeq ($(PLATFORM),DEFAULT)
	ifneq (,$(filter $(CC),mingw32-gcc x86_64-w64-mingw32-g++ w64gcc w32gcc))
		PLATFORM = WIN32_GNU
	else ifneq (,$(filter $(CC),cl))
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


ifeq ($(PLATFORM),WIN32_GNU)
	FLAGS = $(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS = -lkernel32 -lole32 -lopengl32
	EXE_OUT = .exe

	ifeq ($(LIB_AR),DEFAULT)
	LIB_AR     = ar
	LIB_LINKER = $(CC)
	endif
	DLL_OUT = .dll

else ifeq ($(PLATFORM),WIN32_MSVC)
	FLAGS = /nologo /std:c17 /utf-8 /Wall /wd4668 /wd4820 /wd5045 /wd5110 /wd4189
	INCLUDES = -I"." -I"include"
	ifeq ($(MODE),0)
		FLAGS += -O2 -wd4711 -D SI_RELEASE_MODE
	endif

	LIBS =
	EXE_OUT = .exe

	ifeq ($(LIB_AR),DEFAULT)
	LIB_AR     = lib
	LIB_LINKER = $(CC)
	endif
	DLL_OUT = .dll

else ifeq ($(PLATFORM),OS_X)
	FLAGS = $(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS    = -lpthread -ldl
	EXE_OUT =

	ifeq ($(LIB_AR),DEFAULT)
	LIB_AR     = ar
	LIB_LINKER = $(CC)
	endif 
	DLL_FLAGS = -fPIC
	DLL_OUT   = .dylib

else ifeq ($(PLATFORM),LINUX)
	FLAGS = $(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS    = -lpthread -ldl -lX11 -lXrandr -lGL
	EXE_OUT =

	ifeq ($(LIB_AR),DEFAULT)
	LIB_AR     = ar
	LIB_LINKER = $(CC)
	endif
	DLL_FLAGS = -fPIC
	DLL_OUT   = .so

else ifeq ($(PLATFORM),WASM_WASI)
	FLAGS = --target=wasm32-wasi $(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS =
	EXE_OUT = .wasm

	ifeq ($(LIB_AR),DEFAULT)
	LIB_AR     = ar
	LIB_LINKER = $(CC)
	endif
	DLL_FLAGS = -fPIC

else ifeq ($(PLATFORM),WASM_EMCC)
	FLAGS = --target=wasm32-unknown-emscripten -s WASM=1 -s ASYNCIFY -s PTHREAD_POOL_SIZE=4 \
		$(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS    = -pthread
	EXE_OUT = .html

	ifeq ($(LIB_AR),DEFAULT)
	LIB_AR     = ar
	LIB_LINKER = $(CC)
	endif
	DLL_FLAGS = -fPIC
	DLL_OUT   = .so

else
	$(error Unsupported platform. Please refer to the Makefile for supported platofmrs.)

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
    ifneq (,$(filter $(PLATFORM),WIN32_GNU OS_X LINUX WASM_WASI WASM_EMCC))
		$(CC) $(INCLUDES) $(FLAGS) $(EXTRA_FLAGS) -x c -D SI_IMPLEMENTATION -c sili.h -o "$(OUTPUT)/$(LIB_NAME).o"
		$(LIB_AR) rcs "$(OUTPUT)/lib$(LIB_NAME).a" "$(OUTPUT)/$(LIB_NAME).o"
    else ifneq (,$(filter $(PLATFORM),WIN32_MSVC))
		$(CC) $(FLAGS) $(INCLUDES) $(EXTRA_FLAGS) /c /D SI_IMPLEMENTATION /Tc sili.h /Fo"$(OUTPUT)/$(LIB_NAME).obj"
		$(LIB_AR) /nologo /out:"$(OUTPUT)/lib$(LIB_NAME).lib" "$(OUTPUT)/$(LIB_NAME).obj"
    endif

dynamic:
    ifneq (,$(filter $(PLATFORM),WIN32_GNU OS_X LINUX WASM_WASI WASM_EMCC))
		$(CC) $(INCLUDES) $(FLAGS) $(EXTRA_FLAGS) $(DLL_FLAGS) -x c -D SI_IMPLEMENTATION -c sili.h -o "$(OUTPUT)/$(LIB_NAME).o"
		$(LIB_LINKER) $(LIBS) $(EXTRA_FLAGS) $(DLL_FLAGS) -shared -o "$(OUTPUT)/lib$(LIB_NAME)$(DLL_OUT)" "$(OUTPUT)/$(LIB_NAME).o"
    else ifneq (,$(filter $(PLATFORM),WIN32_MSVC))
		$(CC) $(FLAGS) $(INCLUDES) $(EXTRA_FLAGS) /c /D SI_IMPLEMENTATION /Tc sili.h /Fo"$(OUTPUT)/$(LIB_NAME).obj"
		$(CC) /nologo /LD /Fe:"$(OUTPUT)/lib$(LIB_NAME)$(DLL_OUT)" "$(OUTPUT)/$(LIB_NAME).obj"
    endif


# Compile and run every example.
compile_examples:
	@for f in $(shell ls examples/*/*.c); do make SRC=$${f}; rm -rf $(EXE); done

# Compile and run every test.
compile_tests:
	@for f in $(shell ls tests/*.c); do make SRC=$${f}; rm -rf $(EXE); done


# If 'build' doesn't exist, create it
$(OUTPUT):
	mkdir $(OUTPUT)
