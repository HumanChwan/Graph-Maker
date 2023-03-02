CC=gcc
CFLAGS=-Wextra -Werror -Wall -std=c99 -pedantic

all: main

main: main.c
	$(CC) $(CFLAGS) -o main main.c

