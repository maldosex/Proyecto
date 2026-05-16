CC=gcc
CFLAGS=-Wall -Wextra

BIN=bin
SRC=src

all: $(BIN)/cliente $(BIN)/servidor

$(BIN)/cliente:
	mkdir -p $(BIN)
	$(CC) $(SRC)/cliente/cliente.c \
	      $(SRC)/cliente/gui/menu.c \
	      -lncurses \
		  -lmenu \
		  -pthread\
	      -o $(BIN)/cliente

$(BIN)/servidor:
	mkdir -p $(BIN)
	$(CC) $(SRC)/servidor/servidor.c \
		  -pthread\
	      -o $(BIN)/servidor