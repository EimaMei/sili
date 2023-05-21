CC = clang
OUTPUT = build
NAME = test
EXE = $(OUTPUT)/$(NAME)

SRC = example.c
FLAGS = -O2 -std=c89
LIBS =
INCLUDE = -I""


all: $(OUTPUT) $(EXE) run

run: $(EXE)
	./$(EXE)

clean:
	rm $(OUTPUT)/**

$(EXE): $(SRC) sili.h
	$(CC) $(FLAGS) $(INCLUDE) $(SRC) $(LIBS) -o $@

$(OUTPUT):
	mkdir $(OUTPUT)