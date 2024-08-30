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

MSVC_FLAGS = /nologo /std:c11 /Wall /wd4668 /wd4820 /wd5045

EXTRA_FLAGS =
EXTRA_LIBS =
INCLUDE = -I"." -I"include"


DETECTED_OS := $(shell uname 2>/dev/null || echo Unknown)
ifneq (,$(filter $(CC),winegcc x86_64-w64-mingw32-gcc w64gcc w32gcc i686-w64-mingw32-gcc x86_64-w64-mingw32-g++))
	FLAGS = $(GNU_FLAGS)
	LIBS = -lkernel32 -lole32 -lopengl32
	EXE = $(OUTPUT)/test.exe
	DLL_EXT = .dll
else ifneq (,$(filter $(CC),cl /opt/msvc/bin/x64/cl.exe /opt/msvc/bin/x86/cl.exe, cl.exe))
	FLAGS = $(MSVC_FLAGS)
# LIBS = 	-lkernel32 -lole32 -lopengl32
	EXE = $(OUTPUT)/test.exe
	DLL_EXT = .dll
else ifeq ($(DETECTED_OS),Darwin)
	FLAGS = $(GNU_FLAGS)
	LIBS = -lpthread -ldl -framework CoreAudio -framework AudioUnit
	EXE = $(OUTPUT)/test
	DLL_EXT = .so
else ifeq ($(DETECTED_OS),Linux)
	FLAGS = $(GNU_FLAGS)
	LIBS = -lpthread -ldl -lasound -lX11 -lXrandr -lGL -lm -lvulkan
	EXE = $(OUTPUT)/test
	DLL_EXT = .so
endif

# For testing
SRC = examples/bit.c

# 'make'
all: $(OUTPUT) $(EXE) run

# 'make static'
static:
	$(CC) -x c $(FLAGS) $(EXTRA_FLAGS) $(INCLUDE) -D SI_IMPLEMENTATION -c sili.h -o $(OUTPUT)/$(NAME).o
	$(AR) rcs $(OUTPUT)/lib$(NAME).a $(OUTPUT)/$(NAME).o

dynamic:
	$(CC) -x c $(FLAGS) $(EXTRA_FLAGS) -fPIC $(INCLUDE) -D SI_IMPLEMENTATION -c sili.h -o $(OUTPUT)/$(NAME).o
	$(CC) $(FLAGS) $(EXTRA_FLAGS) $(INCLUDE) $(LIBS) $(EXTRA_LIBS) -shared $(OUTPUT)/$(NAME).o -o $(OUTPUT)/lib$(NAME)$(DLL_EXT)


# Run the executable.
run: $(EXE)
	./$(EXE) -h --render="OpenGL" -l=50 filename.txt

# Clean the 'build' folder.
clean:
	rm $(OUTPUT)/**


# Compile each time the main file or `sili.h` is changed.
$(EXE): $(SRC) sili.h sigar.h siapp.h
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
