CC = clang
AR = ar

OUTPUT = build
NAME = sili

GNU_FLAGS = -std=c11 -Wall -Wextra -Wpedantic \
	-Wconversion -Wno-float-conversion -Wno-sign-conversion \
	-Wshadow -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes \
	-Wvla -Wcast-align -Wcast-align=strict \
	\
	-Wno-missing-braces  -Wno-missing-field-initializers -Wswitch-enum -Wcast-align -Wstrict-overflow=5 \
	-Wstrict-prototypes -Winline -Wnested-externs -Wlogical-op -Wstrict-aliasing  -Wredundant-decls \
	-Wold-style-definition \
	\
	-fno-omit-frame-pointer -ffloat-store -fstrict-aliasing \
	\
	-Wformat=2 -Wformat-signedness -Wuninitialized -Winit-self -Wunsafe-loop-optimizations -Wmissing-noreturn \
	-D _GNU_SOURCE -D _LARGEFILE64_SOURCE -D __USE_POSIX199506
GNU_INCLUDES = -I"." -I"include"

GNU_STATIC_FLAGS = -x c -D SI_IMPLEMENTATION -c sili.h -o "$(OUTPUT)/$(NAME).o"
GNU_AR_FLAGS = rcs $(OUTPUT)/lib$(NAME).a
GNU_DLL_FLAGS = -shared "$(OUTPUT)/$(NAME).o" -o "$(OUTPUT)/lib$(NAME)$(DLL_EXT)"

GNU_CC_OUT = -o


MSVC_FLAGS = /nologo /std:c11 /Wall /wd4668 /wd4820 /wd5045
MSVC_INCLUDES = /I"." /I"include"

MSVC_STATIC_FLAGS = /c /D SI_IMPLEMENTATION /Tc sili.h /Fo"$(OUTPUT)/$(NAME).o"
MSVC_AR_FLAGS = /nologo /out:$(OUTPUT)/lib$(NAME).lib
MSVC_DLL_FLAGS = /nologo /DLL /out:"$(OUTPUT)/lib$(NAME)$(DLL_EXT)" "$(OUTPUT)/$(NAME).o"

MSVC_CC_OUT = /Fe


DETECTED_OS := $(shell uname 2>/dev/null || echo Unknown)
ifneq (,$(filter $(CC),winegcc x86_64-w64-mingw32-gcc w64gcc w32gcc i686-w64-mingw32-gcc x86_64-w64-mingw32-g++))
	FLAGS = $(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS = -lkernel32 -lole32 -lopengl32
	EXE = $(OUTPUT)/test.exe
	LINKER = $(CC)

	CC_OUT = $(GNU_CC_OUT)
	STATIC_FLAGS = $(GNU_STATIC_FLAGS)
	DLL_FLAGS = $(GNU_DLL_FLAGS)
	AR_FLAGS = $(GNU_AR_FLAGS)
	DLL_EXT = .dll

else ifneq (,$(filter $(CC),cl /opt/msvc/bin/x64/cl.exe /opt/msvc/bin/x86/cl.exe, cl.exe))
	FLAGS = $(MSVC_FLAGS)
	INCLUDES = $(MSVC_INCLUDES)

	LIBS =
	EXE = $(OUTPUT)/test.exe
	LINKER = link

	CC_OUT = $(MSVC_CC_OUT)
	STATIC_FLAGS = $(MSVC_STATIC_FLAGS)
	DLL_FLAGS = $(MSVC_DLL_FLAGS)
	AR_FLAGS = $(MSVC_AR_FLAGS)
	DLL_EXT = .dll

else ifeq ($(DETECTED_OS),Darwin)
	FLAGS = $(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS = -lpthread -ldl -framework CoreAudio -framework AudioUnit
	EXE = $(OUTPUT)/test
	LINKER = $(CC)

	CC_OUT = $(GNU_CC_OUT)
	STATIC_FLAGS = $(GNU_STATIC_FLAGS)
	DLL_FLAGS = $(GNU_DLL_FLAGS)
	AR_FLAGS = $(GNU_AR_FLAGS)
	DLL_EXT = .so

else ifeq ($(DETECTED_OS),Linux)
	FLAGS = $(GNU_FLAGS)
	INCLUDES = $(GNU_INCLUDES)

	LIBS = -lpthread -ldl -lasound -lX11 -lXrandr -lGL -lm -lvulkan
	EXE = $(OUTPUT)/test
	LINKER = $(CC)

	CC_OUT = $(GNU_CC_OUT)
	STATIC_FLAGS = $(GNU_STATIC_FLAGS)
	DLL_FLAGS = $(GNU_DLL_FLAGS)
	AR_FLAGS = $(GNU_AR_FLAGS)
	DLL_EXT = .so

endif

# For testing
SRC = tests/system.c

# 'make'
all: $(OUTPUT) $(EXE) run

# 'make static'
static:
	$(CC) $(FLAGS) $(INCLUDES) $(EXTRA_FLAGS) $(STATIC_FLAGS)
	$(AR) $(AR_FLAGS) $(OUTPUT)/$(NAME).o

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
$(EXE): $(SRC) sili.h sigar.h siapp.h
	$(CC) $(FLAGS) $(SRC) $(INCLUDES) $(LIBS) $(CC_OUT)"$@"

# Compiles and runs every example.
compile_examples:
	@for f in $(shell ls examples/*.c); do make SRC=$${f}; rm -rf $(EXE); done

# Compiles and runs every test.
compile_tests:
	@for f in $(shell ls tests/*.c); do make SRC=$${f}; rm -rf $(EXE); done


# If 'build' doesn't exist, create it
$(OUTPUT):
	mkdir $(OUTPUT)
