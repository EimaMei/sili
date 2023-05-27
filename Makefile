CC = clang
OUTPUT = build
NAME = test
EXE = $(OUTPUT)/$(NAME)

SRC = example.c
FLAGS = -O2 -std=c99 -Wall -Wextra -Wpedantic
LIBS =
INCLUDE = -I"."


all: $(OUTPUT) $(EXE) run

run: $(EXE)
	./$(EXE)

clean:
	rm $(OUTPUT)/**

$(EXE): $(SRC) sili.h
	$(CC) $(FLAGS) $(SRC) $(INCLUDE) $(LIBS) -o $@

$(OUTPUT):
	mkdir $(OUTPUT)