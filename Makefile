CC = gcc
CFLAGS = -g -Wall 
all:
	$(CC) $(CFLAGS) -o chess.out chess.c
