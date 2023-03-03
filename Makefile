CC=gcc
CFLAGS=-Wextra -Werror -Wall -std=c99 -pedantic
DEBUG=-DDEBUG -ggdb

all: main

main: main.c graphics.o graph_math.o
	$(CC) $(CFLAGS) $(DEBUG) -o $@ $^

sdl_console: sdl_console.c graphics.o graph_math.o
	$(CC) $^ $(CFLAGS) $(DEBUG) -I./thirdparty/SDL2/include -L./thirdparty/SDL2/lib -lSDL2 -lSDL2main -o $@

graph_math.o: graph_math.c graph_math.h
	$(CC) $(CFLAGS) $(DEBUG) -c graph_math.c -lm

graphics.o: graphics.c graphics.h
	$(CC) $(CFLAGS) $(DEBUG) -c graphics.c


clean:
	rm *.exe *.o

.PHONY: all clean
