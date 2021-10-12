CC = gcc
CFLAGS = -g -Wall 
all:
	$(CC) $(CFLAGS) -o chess chess.c
