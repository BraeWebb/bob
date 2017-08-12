CC=gcc
FLAGS=-Wall -pedantic --std=c99

all: build

build: bob.c
	$(CC) $(FLAGS) bob.c -o bob

run: build
	./bob

clean:
	rm -f bob
