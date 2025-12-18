CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g
LDFLAGS=-lm

# On inclut bien main.c
SRC=main.c avl_usine.c leak.c
OBJ=$(SRC:.c=.o)
DEPS=avl.h
TARGET=wildwater

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ)
	rm -f histo_data_*.dat
	# On garde le fichier .png et le .dat source !
