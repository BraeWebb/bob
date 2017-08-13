CC=gcc
FLAGS=-Wall -pedantic --std=c99

all: build

grid.o: grid.c
	$(CC) $(FLAGS) -c grid.c -o grid.o

game.o: game.c grid.o
	$(CC) $(FLAGS) -c game.c grid.o -o game.o

validate.o: validate.c
	$(CC) $(FLAGS) -c validate.c -o validate.o

build: bob.c game.o validate.o
	$(CC) $(FLAGS) bob.c game.o grid.o validate.o -o bob

run: build
	./bob

clean:
	rm -f bob grid.o validate.o
