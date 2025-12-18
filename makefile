CC=gcc
# -g : essentiel pour valgrind (fuites mémoire)
# -lm : nécessaire pour la bibliothèque mathématique si vous utilisez des arrondis
CFLAGS=-Wall -Wextra -std=c99 -g
LDFLAGS=-lm

# Liste de TOUS les modules (ajoutez main.o ici)
SRC=main.c avl_usine.c leak.c
OBJ=$(SRC:.c=.o)

# Fichiers d'en-tête pour surveiller les modifications
DEPS=avl_usine.h leak.h

TARGET=wildwater

# Règle principale (doit être la première)
all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "-> Création de l'exécutable $(TARGET)..."
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compilation des fichiers .o en vérifiant les .h
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers temporaires uniquement
.PHONY: clean
clean:
	@echo "Nettoyage des fichiers objets et de l'exécutable..."
	rm -f $(TARGET) $(OBJ)
	rm -rf histo_outputs
	# On ne supprime PAS le fichier .dat source ici par sécurité
