CC=gcc
CFLAGS=-Wall -Wextra

BIN=bin
SRC=src

all: $(BIN)/cliente $(BIN)/servidor

$(BIN)/cliente:
	mkdir -p $(BIN)
	$(CC) $(SRC)/cliente/cliente.c \
	      $(SRC)/cliente/gui/menu.c \
		  $(SRC)/cliente/api/api.c\
		  $(SRC)/cliente/gui/form_login.c\
		  $(SRC)/cliente/gui/hp.c\
		  include/cJSON.c\
	      -lncurses \
		  -lmenu \
		  -lform\
		  -pthread\
	      -o $(BIN)/cliente

$(BIN)/servidor:
	mkdir -p $(BIN)
	$(CC) $(SRC)/servidor/servidor.c \
			$(SRC)/servidor/bd/file_manager.c \
			$(SRC)/servidor/router.c \
			$(SRC)/servidor/auth_handler.c\
			$(SRC)/servidor/bd/db.c\
			$ include/cJSON.c\
		  -pthread\
	      -o $(BIN)/servidor