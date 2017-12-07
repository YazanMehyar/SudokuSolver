HEAD=./head
CC=gcc
CFLAGS=-g -std=c99 -Wall -I$(HEAD)
#LIBS=-lm

BIN=./bin/
SRC=./src/
TEST=./tests/

PROG=$(basename $(shell ls $(SRC) | grep '.*\.c$$'))
LIST=$(addprefix $(BIN), $(addsuffix .o, $(PROG)))
R_OBJ=$(addprefix $(BIN), $(shell ls $(BIN) | grep '.*\.o$$'))
R_TST=$(addprefix $(TEST), $(shell ls $(TEST) | grep '.*\.exe$$'))

.SECONDARY:

.PHONY: clear

all: $(LIST)

main: $(BIN)*.o
	$(CC) $(CFLAGS) -o main $^ $(LIBS)

$(BIN)%.o: $(SRC)%.c
	$(CC) -c -o $@ $< $(CFLAGS)

%.test: $(TEST)%.exe ;

$(TEST)%.exe: $(BIN)%.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clear:
	$(if $(or $(R_OBJ), $(R_TST)), rm $(R_OBJ) $(R_TST))
