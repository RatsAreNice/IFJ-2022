CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -Wall
FILE=testtable.c


all: compile run clean



compile:
	$(CC) $(CFLAGS) $(FILE) -o testtable 

run:
	./testtable
#sem pridat nejaky text file streamovany do ./ifj
clean:
	rm -f testtable

valgrind: compile memtest clean
memtest:
	valgrind ./testtable