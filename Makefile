CC=gcc
FLAGS=-Wall -pedantic --std=c99

all: build

grid.o: grid.c
	$(CC) $(FLAGS) -c grid.c -o grid.o

build: bob.c grid.o
	$(CC) $(FLAGS) bob.c grid.o -o bob

run: build
	./bob

clean:
	rm -f bob grid.o
