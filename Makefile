CC=gcc
CFLAGS=-Wextra -Werror -Wall -std=c99 -pedantic -ggdb
DEBUG=-DDEBUG

all: main

main: main.c graphics.o graph_math.o
	$(CC) $(CFLAGS) $(DEBUG) -o main main.c graphics.o graph_math.o

graphics.o: graphics.c graphics.h
	$(CC) $(CFLAGS) $(DEBUG) -c graphics.c

graph_math.o: graph_math.c graph_math.h
	$(CC) $(CFLAGS) $(DEBUG) -c graph_math.c -lm

clean:
	rm main.exe main graphics.o

.PHONY: all clean
