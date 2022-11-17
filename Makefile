CC=gcc
CFLAGS=-Wall -std=c11 -pedantic
FILE=ifj.c


all: compile run 

compile:
	$(CC) $(CFLAGS) $(FILE) -o ifj 

run:
	./ifj
#sem pridat nejaky text file streamovany do ./ifj
clean:
	rm -f ifj