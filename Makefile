CC = clang
OUTPUT = build
NAME = test
EXE = $(OUTPUT)/$(NAME)

SRC = main.c
FLAGS = -O0 -g -std=c99 -Wall -Wextra -Wpedantic
LIBS =
INCLUDE = -I"."


all: $(OUTPUT) $(EXE) run

run: $(EXE)
	./$(EXE)

clean:
	rm $(OUTPUT)/**

$(EXE): $(SRC) sili.h
	$(CC) $(FLAGS) $(SRC) $(INCLUDE) $(LIBS) -o $@

dump_asm:
	$(CC) -g $(FLAGS) $(INCLUDE) $(LIBS) -S $(SRC) -o sili.S

$(OUTPUT):
	mkdir $(OUTPUT)