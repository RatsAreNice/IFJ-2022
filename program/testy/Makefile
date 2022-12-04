CC=gcc
CFLAGS=-Wall -std=c11 -pedantic 

all: compile run 

compile:
	$(CC) $(CFLAGS) test.c -o test

run:
	./test < input.txt

clean:
	rm -f test