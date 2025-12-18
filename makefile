# Makefile - Compilation du programme C

CC=gcc
# -g est ajouté pour permettre le débogage (Valgrind/GDB)
CFLAGS=-Wall -Wextra -std=c99 -g

# Liste des fichiers
SRC=avl_usine.c leak.c
OBJ=$(SRC:.c=.o)
# Ajoute tes fichiers .h ici pour que Make surveille les changements
DEPS=avl_usine.h leak.h
TARGET=wildwater

# Règle par défaut
all: $(TARGET)

# Lien de l'exécutable
$(TARGET): $(OBJ)
	@echo "-> Lien de l'executable $(TARGET)..."
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) -lm

# Règle pour compiler les fichiers objet
# $< représente le fichier .c et $@ le fichier .o
%.o: %.c $(DEPS)
	@echo "-> Compilation de $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour tout recompiler proprement
re: clean all

.PHONY: clean re all

clean:
	@echo "Nettoyage des fichiers..."
	rm -f $(TARGET) $(OBJ)
	rm -rf histo_outputs
	rm -f leaks_history.dat c-wildwater_v3.dat histo_data_*.dat
