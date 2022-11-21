CC=gcc
CFLAGS=-Wall -std=c11 -pedantic
FILES= Source/bottom_up.c Source/Parser.c Source/scanner.c Source/symtable.c ifj.c


all: compile run 

compile:
	$(CC) $(CFLAGS) $(FILES) -o ifj 

run:
	./ifj
#sem pridat nejaky text file streamovany do ./ifj
clean:
	rm -f ifj