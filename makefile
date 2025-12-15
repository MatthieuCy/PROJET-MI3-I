# Makefile - Compilation du programme C

CC=gcc
# Options de compilation : -Wall (Warnings), -Wextra (Extra Warnings), -std=c99 (Standard C)

CFLAGS=-Wall -Wextra -std=c99

# Liste des fichiers source C du projet
SRC=avl_usine.c leak.c arbre.c
OBJ=$(SRC:.c=.o)
TARGET=wildwater

# Règle par défaut : construire l'exécutable
$(TARGET): $(OBJ)
	@echo "-> Compilation de l'executable $(TARGET)..."
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) -lm

# Règle pour compiler les fichiers objet
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Règle pour le nettoyage (à lancer avec 'make clean')
.PHONY: clean
clean:
	@echo "Nettoyage des fichiers objets et de l'executable..."
	rm -f $(TARGET) $(OBJ)
	rm -rf histo_outputs
	rm -f leaks_history.dat
	rm -f c-wildwater_v3.dat
	rm -f histo_data_*.dat
