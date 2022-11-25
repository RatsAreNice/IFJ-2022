CC=gcc
CFLAGS=-Wall -std=c11 -pedantic
FILES= Source/bottom_up.c Source/Parser.c Source/scanner.c Source/symtable.c Source/ASS.c ifj.c Source/symdll.c


all: compile 

cleanpile: compile

compile:
	$(CC) $(CFLAGS) $(FILES) -o ifj 


