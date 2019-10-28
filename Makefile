CC=gcc
LD=gcc
CFLAGS=-g -Wall -pedantic -std=c99
GTKCFLAGS:=-g $(shell pkg-config --cflags gtk+-3.0)
GTKLIBS:=$(shell pkg-config --libs gtk+-3.0)

all: gtktetris

gtktetris: gtktetris.o tetris.o
	$(LD) gtktetris.o tetris.o -o gtktetris $(GTKLIBS)

gtktetris.o: gtktetris.c
	$(CC) -c $(CFLAGS) $(GTKCFLAGS) gtktetris.c

tetris.o: tetris.h tetris.c
	$(CC) -c $(CFLAGS) tetris.c

clean:
	rm -f gtktetris gtktetris.o tetris.o
