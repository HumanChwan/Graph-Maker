CC=gcc
CFLAGS=-Wextra -Werror -Wall -std=c99 -pedantic
DEBUG=-DDEBUG

all: main

main: main.c graphics.o
	$(CC) $(CFLAGS) $(DEBUG) -o main main.c graphics.o

graphics.o: graphics.c graphics.h
	$(CC) $(CFLAGS) $(DEBUG) -c graphics.c

clean:
	rm main.exe main graphics.o

.PHONY: all clean
