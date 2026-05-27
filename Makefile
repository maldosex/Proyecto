CC=gcc
CFLAGS=-Wall -Wextra

BIN=bin
SRC=src

all: $(BIN)/cliente $(BIN)/servidor

$(BIN)/cliente:
	mkdir -p $(BIN)
	$(CC) $(SRC)/cliente/cliente.c \
	      $(SRC)/cliente/gui/menu.c \
	      $(SRC)/cliente/api/api.c \
	      $(SRC)/cliente/gui/form_login.c \
	      $(SRC)/cliente/gui/form_register.c \
	      $(SRC)/cliente/gui/menu_available_habits.c \
	      $(SRC)/cliente/gui/men_myhabits.c \
	      $(SRC)/cliente/gui/hp.c \
	      include/cJSON.c \
	      modelo/modelo.c \
	      -lncurses -lmenu -lform -pthread \
	      -o $(BIN)/cliente

$(BIN)/servidor:
	mkdir -p $(BIN)
	$(CC) $(SRC)/servidor/servidor.c \
	      $(SRC)/servidor/bd/file_manager.c \
	      $(SRC)/servidor/router.c \
	      $(SRC)/servidor/auth_handler.c \
	      $(SRC)/servidor/habit_handler.c \
	      $(SRC)/servidor/bd/db.c \
	      modelo/modelo.c \
	      include/cJSON.c \
	      -pthread \
	      -o $(BIN)/servidor

# Objetivo para limpiar los archivos generados
clean:
	rm -rf $(BIN)

# PHONY evita conflictos si existen archivos llamados 'all' o 'clean'
.PHONY: all clean